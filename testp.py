from parse_rest.connection import register
register('xEbHOPVnzdwHkPMRwlt883YIZF5mkIPi2nOL3cl8', 'vwxGBKzHYzJnFlo7VrKj7JUpSIaZ4xDEwG8sjtbx')



from parse_rest.datatypes import Object

class GameScore(Object):
    pass

gameScore = GameScore(score=1337, player_name='MinwooJohn Doe', cheat_mode=False)
gameScore.save()
