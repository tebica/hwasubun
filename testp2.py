#-*- coding: utf-8 -*-

from parse_rest.connection import register
register('atDE32qTfF0GLErm2GCmuDQBxmb4IJ90pcDZyXQX', 'mDG7RMafnOak6woeYcqbu8Oys9NDOZ340iL34u2d')


from parse_rest.datatypes import Object

class plant(Object):
    pass

result = plant.Query.all().filter(name='딸기')

print result

#gameScore = GameScore(score=1337, player_name='MinwooJohn Doe', cheat_mode=False)
#gameScore.save()
