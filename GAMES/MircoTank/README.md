![alt text](https://github.com/hartmann1301/MircoTank/blob/master/Readme/mapExample.png)

# MicroTank
It is a game I programmed for Arduboy

![][exampleGif1]         ![][exampleGif2]


Thanks to this https://felipemanga.github.io/ProjectABE/ great Emulator I could make these gifs.

[exampleGif1]: https://github.com/hartmann1301/MircoTank/blob/master/MicroTankExample1.gif
[exampleGif2]: https://github.com/hartmann1301/MircoTank/blob/master/MicroTankExample2.gif

# Description
You start with a tank level 1 with a weapon of your choice. Everything can be upgraded till level 5. 

# Tanks
With every updrade of the Chains the damage resistence increases a bit. The chain needs to be upgraded first to fit a next level Weapon. There are five different Weapons:
The thing that looks a bit like an arrow is the driving speed, every update increases speed by round about 10 percent.

Name | Level 1 | Level 2 | Level 3 | Level n | Info | Special
--- | --- | --- | ---| --- | --- | ---
Default | ![][tankDefault1] | ![][tankDefault2] | ![][tankDefault3] | ? | a normal Tank | extra damage resistance
Rocket | ![][tankRocket1] | ![][tankRocket2] | ![][tankRocket3] | ? | shoots a big Rocket | extra driving speed
Laser | ![][tankLaser1] | ![][tankLaser2] | ![][tankLaser3] | ? | weapons needs to be loaded | loading weapon shields damage if left is pressed
MG | ![][tankMG1] | ![][tankMG2] | ![][tankMG3] | ? | the weapon can overheat! | better special attacks
Flame | ![][tankFlame1] | ![][tankFlame2] | ![][tankFlame3] | ? | limited range, but lots of damage | great flame resistance, but a bit slow

[tankDefault1]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankDefault1.png
[tankDefault2]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankDefault2.png
[tankDefault3]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankDefault3.png
[tankRocket1]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankRocket1.png
[tankRocket2]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankRocket2.png
[tankRocket3]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankRocket3.png
[tankLaser1]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankLaser1.png
[tankLaser2]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankLaser2.png
[tankLaser3]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankLaser3.png
[tankMG1]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankMG1.png
[tankMG2]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankMG2.png
[tankMG3]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankMG3.png
[tankFlame1]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankFlame1.png
[tankFlame2]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankFlame2.png
[tankFlame3]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/tankFlame3.png

# Enemys
They spawn randomly at the right end of the screen.

Name | Graphic | Info
--- | --- | ---
Standard Enemy | ![][enemyStd] | nothing special about this one
Rocket Enemy | ![][enemyRocket] | those rockets aim at you, so better keep moving
Machine Gun Enemy | ![][enemyMG] | shoots fast lots of buttets, wait till they reload to fight them
Flamethrower Enemy | ? | the range is limited, but they can do lots of damage
Shielded Enemy | ? | has very high damage resistance
Mines | ![][enemyMine] | do not drive over those bad boys

[enemyStd]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/enemyStd.gif
[enemyRocket]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/enemyRocket.gif
[enemyMG]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/enemyMG.gif
[enemyMine]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/enemyMine.gif

# Items
If you kill enemys there is a chance that thy drop one of those items, pick them up.

Name | Graphic | Info
--- | --- | ---
Coin | ![][itemCoin] | you can use those to buy upgrades in the tank shop
Repair | ![][itemRepair] | it reapairs 50 hp of your tank
Heart | ![][itemHeart] | gives you an extra life, if you already have 3 lifes it repairs you
Shield | ![][itemShield] | with this shield you can crush everything without getting damage, even mines!
Slowmotion | ![][itemSlowmo] | makes everything around you slow for about 4 seconds
Special Attack | ![][itemSpecial] | gives you one special attack, it comes in 3 levels increasing in strength

[itemCoin]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/itemCoin.gif
[itemRepair]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/itemRepair.gif
[itemHeart]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/itemHeart.gif
[itemSlowmo]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/itemSlowmo.gif
[itemShield]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/itemShield.gif
[itemSpecial]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/itemSpecial.gif

# Controls
Button | Game | Shop
--- | --- | ---
![alt text][buttonUp] | drive with Tank up  | go up to next weapon
![alt text][buttonDown] | drive with Tank down  | go down to next weapon
![alt text][buttonLeft] | drive with Tank left  | toggle sound on/off
![alt text][buttonRight] | drive with Tank right  | upgrade current weapon
![alt text][buttonB] | switch to Shop  | switch to Game
![alt text][buttonA] | shoot the weapon  | set current weapon
![alt text][buttonA] + ![alt text][buttonB] | activate special attack | -

[buttonUp]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/buttonUp.png
[buttonDown]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/buttonDown.png
[buttonLeft]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/buttonLeft.png
[buttonRight]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/buttonRight.png
[buttonA]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/buttonA.png
[buttonB]: https://github.com/hartmann1301/MircoTank/blob/master/Readme/buttonB.png

# Map
This is actually the whole map. It is just for the variety of the background. It repeats infinitly.

![alt text](https://github.com/hartmann1301/MircoTank/blob/master/Readme/mapExample.png)
