#include <DHT11.h>
#include <Parse.h>
#include <Process.h>


#define TIME_DELAY 10 /* Seconnd */
#define PLANT_ID "B4218AF00821" /* From WiFi MAC Address */

/* Code for Water Level Sensor Circuit Built with an Arduin */
const int WaterLevel_Pin= 0; // sensor pin connected to analog pin A0
int water_level;
const int WaterLedPin = 6;

/* Code for Moisture Sensor Circuit Built with an Arduin */
const int Moisture_Pin= 1; // sensor pin connected to analog pin A1
int moisture_level;
const int MotorRelayPin = 7;
int MoistLimit = 0;

/* Code for Illumination Sensor Circuit Built with an Arduin */
const int Light_Pin= 2; // sensor pin connected to analog pin A2
int light_level;

/* Code for Temperature Sensor Circuit Built with an Arduin */
const int Temp_Pin = 12;
float humi_level;
float temp_level;
DHT11 dht11(Temp_Pin);

#define MOIST_LV1 20
#define MOIST_LV2 30
#define MOIST_LV3 40
#define MOIST_LV4 50
#define MOIST_LV5 60

bool Water_empty_tmp;

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600); 
  //delay(3000);  
  
  Show_Project_Info();

  Serial.println("Starting bridge...\n");
  pinMode(13,OUTPUT);  
  digitalWrite(13, LOW);  
  Bridge.begin();
  digitalWrite(13, HIGH);  // Led on pin 13 turns on when the bridge is ready

  delay(2000); 

  /* Initialize Sensors */
  Init_Waterlevel_Sensor();
  Init_Moisture_Sensor();
  Init_Light_Sensor();

  Show_WiFi_Info();

  Parse.begin("atDE32qTfF0GLErm2GCmuDQBxmb4IJ90pcDZyXQX", "zIQwwB64gNZl1L3BDgTJYclR1tBs7zNnHIsA8tDx");
}

void loop() {  
  // put your main code here, to run repeatedly:
  water_level= analogRead(WaterLevel_Pin); //arduino reads the value from the water level sensor
  moisture_level= analogRead(Moisture_Pin); //arduino reads the value from the moisture sensor
  light_level= analogRead(Light_Pin); //arduino reads the value from the illumination sensor
  dht11.read(humi_level, temp_level);

  moisture_level = (1050 - moisture_level)/10;
  water_level = water_level / 10;
  light_level = light_level / 3;

  Serial.print("[");
  Serial.print(millis()/1000);
  Serial.print("] Water Level : ");
  Serial.print(water_level);
  Serial.print("\tMoisture : ");
  Serial.print(moisture_level);
  Serial.print("\t\tLight : ");
  Serial.print(light_level);
  Serial.print("\tTemperature : ");
  Serial.print(temp_level);
  Serial.print("\tHumidity : ");
  Serial.print(humi_level);

if(true) {
/* Insert Data to Parse */
  ParseObjectCreate qObject;
  qObject.setClassName("History");
  qObject.add("PlantID", PLANT_ID);
  qObject.add("Moisture", moisture_level);
  qObject.add("WaterLevel", water_level);
  qObject.add("Light", light_level);
  qObject.add("Temperature", temp_level);
  qObject.add("Humidity", humi_level);
  ParseResponse response = qObject.send();
  if (!response.getErrorCode()) {
      // The object has been saved
      Serial.println("    ::: Save data at Parse :::");
  } else {
      // There was a problem, check response.
      Serial.print("    !!! Fail to Save at Parse !!!  ");  
      Serial.println(response.getErrorCode());
  }  
  response.close(); // Free the resource  
}
/* 
 * Check Moisture of Soil  
 * Dry : about 1,000
 * Wet : 0
 */ 

bool Water_Supply = false;
bool Empty_Warning = false;
 
  if (moisture_level >= MoistLimit || water_level <10) {
    digitalWrite(MotorRelayPin, LOW);
    Serial.println("\tMotor OFF...");
    digitalWrite(13, LOW);
  }
  else {
    /* Enter Code to Enable Water Motor */
    digitalWrite(MotorRelayPin, HIGH);
    digitalWrite(13, HIGH);
       
    Serial.println("\t\t<Soil is Dried. Motor will Run> ");
    Water_Supply = true;
    delay(9000);
    water_level = 5;
  }

/* 
 * Check Water Level  
 * Empty : 0 
 */
  if (water_level < 10) {
    digitalWrite(MotorRelayPin, LOW);       
    
    Serial.println("\t\t<Water is Empty. Please Fill the Water> ");
    Empty_Warning = true;    
    Water_empty_tmp = true;
  }

  if (Water_Supply || Empty_Warning)
    send_event(Water_Supply, Empty_Warning);
  if (Water_empty_tmp && water_level >= 10) {
    send_event(Water_Supply, false); 
    Water_empty_tmp = false;
  }
    
  delay(TIME_DELAY*1000);
}

/* Information of Project */
void Show_Project_Info() {
  Serial.println("***********************************************************************");
  Serial.println("** HWA-SOO-BUN Project                                               **");
  Serial.println("**            (Automatic Flowerpot Environment Management System)    **");
  Serial.println("**                                                                   **");
  Serial.println("** Team Member : Kim Sung Soo, Park Min Woo, Lim Soon-il             **");
  Serial.println("** Program Ver : 0.96                                                **");
  Serial.println("***********************************************************************");
  Serial.println("");
}

/* Information of WiFi Connection */
void Show_WiFi_Info() {
  Process wifiCheck;
  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");
  while (wifiCheck.available() > 0) {
    char c = wifiCheck.read();
    Serial.print(c);
  }
  Serial.println();
  delay(5000);
}

void Init_Waterlevel_Sensor(void) {
  /* the water level sensor will be an input to the arduino */
  pinMode(WaterLevel_Pin, INPUT);
  pinMode(WaterLedPin, OUTPUT);
  Water_empty_tmp = false;
}

void Init_Moisture_Sensor(void) {
  pinMode(Moisture_Pin, INPUT);
  pinMode(MotorRelayPin, OUTPUT);
  digitalWrite(MotorRelayPin, LOW);
  MoistLimit = MOIST_LV3;
}

void Init_Light_Sensor(void) {
  /* the water level sensor will be an input to the arduino */
  pinMode(Light_Pin, INPUT);
}

void send_event(bool Water_Supply, bool Empty_Warning) {
  ParseObjectCreate eventObject;
  eventObject.setClassName("EventHistory");
  eventObject.add("WaterSupply", Water_Supply);
  eventObject.add("EmptyWarning", Empty_Warning);
  
  ParseResponse event_rsp = eventObject.send();
  if (!event_rsp.getErrorCode()) {
      // The object has been saved
      Serial.println("    ::: Save data at Parse :::");
  } else {
      // There was a problem, check response.
      Serial.println("    !!! Fail to Save at Parse !!!");      
  }
  event_rsp.close(); // Free the resource
}
