## Trolly Fish
Trolly fish is like an endless runner, where you need to dodge the enemies, collect stars and use powerups to progress as far as you can.

We need a good balance between stars to catch and enemies.

* the main character is a fish trying to dodge ... aquatic stuff
* 3 enemies
  * Eel: swimming straight at a fixed speed
  * Bad Fishy: swimming in small bursts
  * Fast Bad Fishy: swimming in small bursts
  * jellyfish: going up and than floats down again, go up and float down again, while moving towards the player.
* starfish will give you points
* 8 powerups (fits exact 1 byte)


## FISH LIST
| **SPRITE** | **SIZE** | **FRAMES** | **NAME** | **HP** | **SWIMMING** |
| ---        | ---      | ---        | ---      | ---    | ---          |
| **Trolly Fish** | 16x16 | 4 | trollyFish_16x16 | 1 or 2| 
| **Bad Fishy** | 12x12 | 4 | badFishy_16x16 | 1 |
| **Fast Bad Fishy** | 12x12 | 4 | badFishy_16x16 | 1 |
| **Jellyfish** | 16x24 | 4 | jellyFish_16x24 | 3 |
| **Eel** | 80x16 | 4 | eel_16x80 | 5 |


## POWER-UP LIST
| **SPRITE** | **SIZE** | **FRAMES** | **TIMER** | **Description** |
| ---        | ---      | ---        | ---       | ---             |
| **starFish** | 12x12 | 1 | | |
| **turnFish** | 12x12 | 1 | | all enemies on screen turn into stars |
| **stopFish** | 12x12 | 1 | yes | all enemies stop swimming, but stars and power-ups keep coming |
| **popFish** | 12x12 | 1 | | all enemies on screen pop like bubbles |
| **shootFish** | 12x12 | 1 | yes | player can shoot 1 enemie at a time |
| **protectFish** | 12x12 | 1 | yes | player is in a protective bubble |
| **lifeFish** | 12x12 | 1 | | player get's 1 continue, automaticly used when player dies |
| **shockFish** | 12x12 | 1 | yes | player can shock all fish for extra points |
| **magnetFish** | 12x12 | 1 | yes | all stars get pulled towards the player |


## COMBINATION LIST
|             | **turn** | **stop** | **pop** | **shoot** | **protect** | **life** | **shock** | **magnet** |
| ---         | ---      | ---      | ---     | ---       | ---         | ---      | ---       | ---        |
| **turn**    |          | V        |         | V         |             | V        |           | V          |
| **stop**    | V        |          |         |           |             | V        |           | V          |
| **pop**     |          |          |         | V         |             | V        | V         | V          |
| **shoot**   | V        |          | V       |           |             | V        |           | V          |
| **protect** |          |          |         |           |             | V        |           | V          |
| **life**    | V        | V        | V       | V         | V           |          | V         | V          |
| **shock**   |          |          | V       |           |             | V        |           | V          |
| **magnet**  | V        | V        | V       | V         | V           | V        | V         |            |


## BONUS LIST FOR KILLING ENEMIES
| 			  | **Bad Fishy** |  **Fast Bad Fishy** |  **Jelly Fish** |  **Eel**   |
| ---         | ---           | ---                 | ---       	  |  ---       | 
|  **shoot**  |      +10     |         +20        |       +30      |    +50    |
|  **shock**  |      +10     |         +20        |         X       |      X     |
|  **pop**    |      +10     |         +10        |       +20      |    +40    |


##BONUS LIST FOR COLLECTING
- collecting a power-up that can not be combined = +20
- collecting all stars of 1 stream = +30
- collecting a star of turned fish = +40
- collecting an extra live when you already have an extra life = +50
