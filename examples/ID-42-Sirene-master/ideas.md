## Sirène


Weapons: 

The mermaid has 4 weapons. 

1. Trident. The first weapon. You fire 1 trident per button press with a delay allowing only 2 tridents on screen at once. Moves moderately fast, does moderate damage (2). Trident is destroyed when it makes contact with an enemy or is offscreen.

2. Bubble. The second weapon. The rapid fire weapon of the game. You fire a constant stream of bubbles by holding down the button. A quicker delay allowing 3 on screen at once. Moves fast does less damage (1). Bubble is destroyed when it makes contact with an enemy or is offscreen. Visually has a trail of tiny bubbles behind it. 

3. 3 Way Shells. The 3rd weapon of the game. You fire 3 shells (one diagonally up, one straight, one diagonally down) per button press with a delay allowing 2 sets 3 shells on screen at once. Moves moderately fast, each shell does less damage (1). Shells are destroyed when they make contact with an enemy or is offscreen.

4. Magic. The last weapon of the game. You can hold down the button to charge the magic. You can only fire one magic blast on screen at a time. Holding down the charge increases the damage. Magic blast travels through enemies but is destroyed when offscreen. Visually has a trail of magic behind it.


## WEAPONS LIST
| **SPRITE** | **SIZE** | **FRAMES** | **AMOUNT** | **DAMAGE** | **SPEED** | **LIFETIME** |
| ---        | ---      | ---        | ---       | ---             | ---             | ---             |
| **Trident** | 16x8 | 1 | 2 | -2 HP | 2 | 1 HIT |
| **Bubble** | 8x8 | 1 | 3 | -1 HP | 3 | 1 HIT |
| **3 Shells** | 8x8 | 1 | 2x3 | -1 HP | 2 | 1 HIT |
| **Magic** | 8x8 | 4 | 1 | -3 HP | 1 | CONTINUE HIT |

HUD

upper left corner LIFE, upper right corner POINTS

The remaining LIFE is displayed with 3 hearts. When the game starts, the mermaid has 3 full hearts. Everytime the mermaid is hit, she loses a heart (displayed with an empty heart). When you lose all 3 hearts, it's GAME OVER. When this happens GAME OVER screen is displayed with your current HIGHSCORE and when you press a button, you go back to the title screen. 

There's 7 digits displayed for the points. When the game starts, you have 0 000 000 points. Destroying enemies and collecting items can give you points. 

At the start of every level there's a quick message displaying what stage you're at. 


## ENEMIES
| **SPRITE** | **SIZE** | **FRAMES** | **NAME** | **HP** | **POINTS** | **MAX AMOUNT** |
| ---        | ---      | ---        | ---      | ---    | ---        | ---           |
| **Small Fish** | 8x8 | 4 | enemyFishy | 2 HP |   | 3  |
| **Big Fish** | 16x16 | 4 | enemyFish | 4 HP |   | 3  |
| **Eel** | 32x8 | 4 | enemyEel | 4 HP |   | 2  |
| **Jellyfish** | 16x16 | 5 | enemyJellyfish | 2 HP |  | 3  |
| **Octopus** | 16x16 | 4 | enemyOctopus | 4 HP |  | 2  |

Enemy behaviors

Small Fish:
Swims offscreen from right to left. Small and weak, usually they swim in packs. 
Nice to have depending on how difficult it is:
1. have a variant that swims faster
2. have the ability to swim in a path other than a straight line: arc, sinewave. This might not be necessary but good to bring it up to consider it

Big Fish:
Same behavior as small fish, but a larger target with higher HP. 

Eel:
Swims in a straight line from offscreen. At a fixed interval will shoot a zap projectile at the mermaid. 

Jellyfish:
Swims in a stop and go pattern from the bottom of the screen upwards. 

Octopus: 
Swims offscreen from right to left. At a fixed interval shoots a bullet (meant to be ink) at the mermaid. 
1. If variable swimming patterns are possible, would be nice to apply them to the octopus as well.

## BOSSES
| **SPRITE** | **SIZE** | **FRAMES** | **NAME** | **HP** | **POINTS** | 
| ---        | ---      | ---        | ---      | ---    | ---        | 
| **Shark** | 32x16 | 4 | enemyShark | ? HP |   |   
| **?** | ? | ? | ? | ? HP |   |   
| **Pirate Ship** | 64x64 | 1 | PirateshipShip | ? HP |     

Boss behaviors

**Shark:**
This is the first boss and has 2 main phases to his attack. 
1. Seeking phase:
When the shark first comes on the screen he will seek the mermaid. He slowly moves horizonatally towards the mermaid and will also slightly move vertically to try and match her position. His speeds have to be enough to give a challenge to the player but not unfair so that it's impossible for the player to avoid his attacks. When the shark has moved passed the mermaid he will continue to swim offscreen, the repeat the patern going the other way 2 more times. After this the shark will move on screen and speed up his animation to give a signal he's going into his second phase, the charge. 
2. Charge phase. 
During this phase the shark moves horizontally much faster but cannot alter his vertical movement beyond the mermaid's position when he enters the screen of this phase. The speeds have to be faster but again not impossible for the mermaid to dodge his attacks. Once offscreen he repeats this patern going the other way 2 more times, then reverts back to seeking phase. He continues this patern until he's dead. 

In a chart it would look something like this:
Seeking 
<--
-->
<--

Animation to signal phase change

Charging
-->
<--
-->

Back to seeking... 

**Seahorse:**

The seahorse moves on the right side of the screen in a basic figure 8 patern. He fires tiny seahorses that can be destroyed in an up angled, straight, down angled, straight, etc... pattern. There is 3 phases to the seahorse related to how much HP he has left. Each phase affects his rate of fire. 

1. First phase. 100 - 67% HP. In this first phase the seahorse's rate of fire is quite slow. He's only firing one tiny seahorse at a time. 
2. Second phase. 67 - 33% HP. In this sencond phase the seahorse's rate of fire is increased. There should be on average 2 tiny seahorses on screen at all times. 
3. Third phase. 33 - 0% HP. In this third and last phase the seahorse's rate of fire in quite rapid. There should be many seahorses on screen at a time. The seahorse_example02 is something of how fast he should be firing tiny seahorses before reaching 0 HP. 


**Pirate Ship**

The last boss. Like the seahorse has different phases according to his current HP. 

1. First phase. 100 - 67%HP. Pirate Ship moves slowly in a basic up and down pattern on the right side of the screen. He fires skull projectiles very infrequently. The skulls cannot be destroyed. Every x seconds (12?) The Pirate Ship makes a charge. The charge is always random either on the top or bottom side of the screen. 

2. Second phase. 67 - 33% HP. During this second phase the pirate ship moves faster up and down and fires skulls more frequently. It must be possible to dodge the skulls at all times. Every x seconds (8?) the Pirate Ship makes a charge. 

3. Third phase. 33 - 0% HP. During the last phase the pirate ship moves even faster and fires even more frequently. Again however, it must be possible to dodge the skulls at all times. Every x seconds (4?) the Pirate Ship makes a charge. 


## ENEMIE WAVES
| **NUMBER** | **TYPE**  | **AMOUNT** | **SPEED** | **START** | **BEHAVIOR** |
| ---        | ---       | ---        | ---       | ---       | ---          |
| **000** | - | - | - |
| **001** | FISHY | 3 | 1 |start at the top | all swim in a straight Line |
| **002** | FISHY | 3 | 1 |start at the middle | all swim in a straight Line |
| **003** | FISHY | 3 | 1 |start at the bottom | all swim in a straight Line |
| **004** | FISHY | 3 | 1 |start at the top | start in a straight Line, swim to the middle |
| **005** | FISHY | 3 | 1 |start at the bottom | start in a straight Line, swim to the middle |
| **006** | FISHY | 3 | 1 |start at the middle | all swim in a sinewave Line |
| **007** | FISHY | 1 | 1 |start at the top | swims in a straight Line |
| **008** | FISHY | 1 | 1 |start at the middle | swims in a straight Line |
| **009** | FISHY | 1 | 1 |start at the bottom | swims in a straight Line |
| **010** | FISHY | 1 | 2 |start at the top | swims fast in a straight Line |
| **011** | FISHY | 1 | 2 |start at the middle | swims fast in a straight Line |
| **012** | FISHY | 1 | 2 |start at the bottom | swims fast in a straight Line |
| **013** | JELLYFISH | 3 | 1 |bottom right to left | swims upward each one slighty offset form each other |
| **014** | JELLYFISH | 3 | 1 |bottom left to right | swims upward each one slighty offset form each other |
| **015** | JELLYFISH | 3 | 1 |bottom  | swims upward simultaneously |
| **016** | JELLYFISH | 2 | 1 |bottom  | swims upward simultaneously |
| **017** | EEL | 1 | 1 |top  | swims in a straight line, shooting zap projectiles aimed towards mermaid at fixed intervals |
| **018** | OCTOPUS | 1 | 1 |top  | swims in a straight line, shooting bullet projectiles aimed towards mermaid at fixed intervals |
| **019** | FISH | 3 | 2 |start at the middle | all swim in a straight Line |
| **020** | FISH | 3 | 2 |start at the middle | all swim in a sinewave Line |
