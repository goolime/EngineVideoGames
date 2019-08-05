# EngineVideoGames
Video Games programming repository
comes with 2 maps
named csvMap2
named csvMap3
to use a map copy it and change its name to "csvMap.csv" in folder
...\EngineVideoGames\Game
--------------------------------------------------------
[feature]
keys:
Left arrow: turn left
Right arrow: turn right
Up arrow: speed up
Down arrow: speed down

Key_O (not zero) : change camera
Key_space_bar: stop motion

Key_1: Hide/unhide walls 
Key_2: Hide/unhide ramps
Key_3: Hide/unhide apples
Key_4: Hide/unhide mines
note:  you cant hit hidden objects 
turn off walls and ramps for faster program
the collision slows down the game by a lot
---------------------------------------------------------
warning! the next two keys are just for example
to explain why we rather not move in 3D
Key_Y: turn snake down
Key_H: turn snake up
use map 3 the test with this-
[bug]
- the skinning breaks when using this
[bug\feature]
- also, something weird happens at max values that makes the snake do a barrel roll
-----------------------------------------------------------
[feature]
objects in-game
[apples]: increase your points by 1 - play sound when hit
[mines]: if at 0 points game over else set points to 0 - plays a sound when hit
[ramps] when hitting a ramp your snake will go up 1 LVL: as seen in map2 
(do to collision bug when scaling the shape by a lot collision stop working as intended)
to fix this we add small shapes in-between (you will see\unsee them when pressing Key_2)
[walls] small box that when hit lose game has the same bug as ramps (you will see\unsee them when pressing Key_1)
(the intention was to place 1 big wall at the needed place but the collision cant detect the hit)
[goal] when hit you win the game - plays Microsoft sound
-----------------------------------------------------------
[feature]
excel map creator in folder
..\EngineVideoGames
file "CSV makerv2.xlsx" is a map creator
page 1 is the keywords
page 2 is the map 
example of placing an apple at LVL 1
=lvl1_ + apple
it will change this to a int value
-----------------------------------------------------------
how we did the snake move: basically case the next part of the body
when creating the snake parts we find the 3D_vector_offset of each part to the next part at creation
then we split the movement into 3 steps
[step1] moving to the right location: - this uses shapetransformation(xglobaltranslate,?) for XYZ
part1 location: (1,2,3)
part2 location: (0,0,1)
3D_vector_offset (2,0,0) 
we need to move part2 to location (3,2,3) so we do part2 +=(glm::vec3(3,2,2) * speed)

[step2] turning to the right location:
let's say part 1 has a 90-degree angle around Y-axis
so
before finding where we need to move we turn 3D_vector_offset
so 3D_vector_offset = (0,0,2)

[step3] turning the shape to the right angle
if part 1 is at a 90-degree angle around Y-axis
we need part 2 to also be at a 90-degree angle around Y-axis
so we figure out the angle of part 1 (90) and find the current angle of part2 (let's say 60)
and then we find that 90 -60 = 30 so we turn part2 30*speed degree
we do this to XYZ
the hard part is to find this 90\60 degree angle

the trick to all this is to make speed = 0.04 or so
that way the movement looks very good


-----------------------------------------------------------
[feature]
all items have a texure but we did not change the base color
some items have sound when hitting
[bugs\feature]
do to the fact that walls dont work as intended you can float in air if you go to a lvl higher then 1 then fall down
do not have time to implament a psychic engine - but the snake has a simple value "need lvl" and he will get to that lvl 
-----------------------------------------------------------------

