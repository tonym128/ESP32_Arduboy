//----------------------------------------------------------------------------
// ROOFTOP RESCUE 
// (c) 2019 Bert vt Veer
//----------------------------------------------------------------------------


#include <ESP8266WiFi.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
//#include <EEPROM.h>
#include "classes.h"
#include "bitmaps.h"
#include "sounds.h"

constexpr uint8_t STORE_COOKIE = 71;
constexpr uint16_t STORE_INIT_ADDR = 91;
constexpr uint16_t STORE_SCORE_ADDR = 92;

Arduboy2 ardu;
ArduboyTones sound(ardu.audio.enabled);
Game game;

bool credits_active = false;
bool menu_active = false;
uint8_t menu_index = 0;
uint16_t scene_timer = 0;


//----------------------------------------------------------------------------
// General drawing routines
//----------------------------------------------------------------------------

void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool invert = false) {
    ardu.fillRect(x+1, y+1, w-2, h-2, invert ? BLACK : WHITE);
    ardu.drawRect(x, y, w, h, invert ? WHITE : BLACK);    
}

void drawCircle(uint8_t x, uint8_t y, uint8_t r, bool invert = false) {
    ardu.fillCircle(x, y, r, invert ? BLACK : WHITE); 
    ardu.drawCircle(x, y, r+1, invert ? WHITE : BLACK);   
}

template <typename T> void drawText(uint8_t x, uint8_t y, T src, bool invert = false) {
    ardu.setTextBackground(invert ? BLACK : WHITE);
    ardu.setTextColor(invert ? WHITE : BLACK);
    ardu.setCursor(x,y); 
    ardu.print(src);
}


//----------------------------------------------------------------------------
// Draw background / buildings / fires
//----------------------------------------------------------------------------

void drawBackground() 
{
    ardu.fillScreen(WHITE);
    Sprites::drawOverwrite(0,0, img_sky, 0);
    Sprites::drawOverwrite(0, 64-img_building_bg[1], img_building_bg, 0); 
}
                
void drawFires(const Building& b)
{
    for (uint8_t i=0; i<BUILDING_MAX_FIRES; i++) {
        if (b.fires[i].active) {
            uint8_t frame = b.fires[i].current_frame;
            uint8_t fire_x = b.fires[i].x + b.offset_x;
            uint8_t fire_y = b.fires[i].y - img_flames[1] + b.offset_y;
            if (frame < 6) Sprites::drawExternalMask(fire_x, fire_y, img_flames, img_flame_masks, frame, frame);
        }
    }
}

void drawBuilding(uint8_t idx, t_bitmap* img) {
    const Building& b = game.getBuilding(idx);
    uint8_t bx = b.x + b.offset_x;
    uint8_t by = b.y + b.offset_y;
    Sprites::drawOverwrite(bx, by, img, 0);
    // Roof decoration
    if (idx == 3) {
        ardu.drawFastHLine(bx+20, by-2, 6, BLACK);
        ardu.drawPixel(bx+20, by-1, BLACK);
        ardu.drawPixel(bx+23, by-1, BLACK);
        ardu.drawPixel(bx+25, by-1, BLACK);
    }
    drawFires(b);
}

void drawBuildingMasked(uint8_t idx, t_bitmap* img, t_bitmap* msk) {
    const Building& b = game.getBuilding(idx);
    uint8_t bx = b.x + b.offset_x;
    uint8_t by = b.y + b.offset_y;
    Sprites::drawExternalMask(bx, by, img, msk, 0,0);
    // Roof decoration
    switch(idx) {
        case 0: ardu.drawFastHLine(bx+12, by-1, 3, BLACK); break;
        case 1: ardu.fillRect(bx+4, by-2, 2, 2, BLACK); break;
        case 2: ardu.drawFastVLine(bx+15, by-3, 3, BLACK); 
                ardu.drawFastVLine(bx+17, by-2, 2, BLACK);
                break;
    }
    drawFires(b);
}

void drawBuildings()
{
    drawBuildingMasked(0, img_building1, img_building1_mask);  
    drawBuildingMasked(1, img_building2, img_building2_mask); 
    drawBuildingMasked(2, img_building3, img_building3_mask);    
    drawBuilding(3, img_building4);   
    drawBuilding(4, img_building5); 
    // Foreground buildings
    Sprites::drawOverwrite(0, 64-img_building_fg1[1]+3, img_building_fg1, 0); 
    Sprites::drawOverwrite(22, 64-img_building_fg2[1]+2, img_building_fg2, 0); 
    Sprites::drawOverwrite(65, 64-img_building_fg3[1]+2, img_building_fg3, 0); 
}


//----------------------------------------------------------------------------
// Draw interface elements
//----------------------------------------------------------------------------

void drawScorePanel()
{
    uint16_t display_val = game.state == Game::TITLE ? game.hiscore : game.score;
    uint8_t scorex = display_val<100 ? 108 : 102;
    uint8_t scorew = display_val<100 ? 17 : 23;
    drawBox(scorex,52, scorew,13);
    if (display_val < 10)
        Sprites::drawOverwrite(scorex+6,55, digits[display_val], 0);          
    else if (display_val < 100) {
        Sprites::drawOverwrite(scorex+3, 55, digits[display_val/10], 0);
        Sprites::drawOverwrite(scorex+9, 55, digits[display_val%10], 0);
    }
    else {
        Sprites::drawOverwrite(scorex+3, 55, digits[display_val/100], 0);
        Sprites::drawOverwrite(scorex+9, 55, digits[(display_val/10)%10], 0);
        Sprites::drawOverwrite(scorex+15, 55, digits[display_val%10], 0);                
    }
}


//----------------------------------------------------------------------------
// Game state initializers
//----------------------------------------------------------------------------

void startTitle()
{
    game.buildings.init();
    game.setState(Game::TITLE);
}

void startGame()
{
    srand(ardu.frameCount);
    game.setState(Game::PLAYING);
    game.chopper.moveTo(game.stops[game.chopper.current_stop]);
    game.chopper.timeout = 90;
}

//----------------------------------------------------------------------------
// Credits screen
//----------------------------------------------------------------------------

void runCredits() 
{
    int16_t top_y = 80-scene_timer/2;
    if (top_y > 13) scene_timer++;
    
    ardu.fillScreen(BLACK);
    drawText(25,top_y, "ROOFTOP RESCUE", true);
    drawText(34,top_y+12, "VERSION 1.0", true);
    drawText(31,top_y+32, "2019 BERT VEER", true);
    
    // (C) sign
    drawCircle(24, top_y+35, 2, true);
    ardu.drawFastHLine(23, top_y+34, 3, WHITE);
    ardu.drawPixel(23, top_y+35, WHITE);
    ardu.drawFastHLine(23, top_y+36, 3, WHITE);

    if (ardu.justPressed(A_BUTTON)) {
        credits_active = false;
        menu_active = false; 
        scene_timer = 0;
    }
}

//----------------------------------------------------------------------------
// Handle options screen
//----------------------------------------------------------------------------

void handleMenu() 
{   
    drawBox(7,4, 115,58);
    drawText(26,13, ardu.audio.enabled() ? "SOUND: ON" : "SOUND: OFF");
    drawText(26,24, game.state==Game::TITLE ? "START GAME" : "END GAME");
    drawText(26,35, "RESET");
    drawText(26,46, "CREDITS");
    ardu.drawChar(16,13+menu_index*11, 16, BLACK, WHITE, 1);

    if (ardu.justPressed(A_BUTTON)) {
        sound.tones(sfx_menuclick);
        switch(menu_index) {
            case 0: 
                ardu.audio.toggle();
                ardu.audio.saveOnOff();
                break;
            case 1: 
                if (game.state==Game::TITLE) {
                    scene_timer = 0;
                    game.setState(Game::FOREPLAY);
                }
                else {
                    scene_timer = 0; 
                    game.setState(Game::GAMEOVER); 
                }
                menu_active = false; 
                break;
            case 2:
                EEPROM.put(STORE_INIT_ADDR, 255);
                EEPROM.commit();
                ardu.audio.on();
                game.setState(Game::INIT);
                menu_active = false; 
                break;
            case 3:
                credits_active = true;
                scene_timer = 0;
                break;
        }
    }
    else if (ardu.justPressed(B_BUTTON)) {
        sound.tones(sfx_menutoggle);
        menu_active = false; 
    }
    else if (ardu.justPressed(UP_BUTTON)) {
        sound.tones(sfx_menuselect);
        menu_index = menu_index == 0 ?  3 : menu_index-1;
    }
    else if (ardu.justPressed(DOWN_BUTTON)) {
        sound.tones(sfx_menuselect);
        menu_index = (menu_index+1) % 4;
    }
}

//----------------------------------------------------------------------------
// Input during game
//----------------------------------------------------------------------------

void handleInput()
{
    static bool wasNoroomSoundPlayed = false;
    bool rope_extended = false;
    
    if (ardu.pressed(LEFT_BUTTON)) game.moveChopper(DIRECTION_LEFT);
    else if (ardu.pressed(RIGHT_BUTTON)) game.moveChopper(DIRECTION_RIGHT); 
    
    if (ardu.pressed(DOWN_BUTTON) || ardu.pressed(A_BUTTON)) {
        rope_extended = game.commandRope(ROPE_EXTEND);
        if (!rope_extended && game.chopper.dudes_onboard >= CHOPPER_CAPACITY && !wasNoroomSoundPlayed) {
            sound.tones(sfx_noroom);
            wasNoroomSoundPlayed = true;
        }
    }
    else wasNoroomSoundPlayed = false;
    
    if (ardu.pressed(UP_BUTTON) || (ardu.pressed(A_BUTTON) && !rope_extended)) {
        game.commandRope(ROPE_RETRACT);
    }
}


//----------------------------------------------------------------------------
// Main loop
//----------------------------------------------------------------------------

void nextFrame()
{
    if (credits_active) runCredits();
    else if (menu_active) handleMenu();
    else switch(game.state)
    {
        case Game::INIT:
            menu_active = false;
            menu_index = 0;
            game.hiscore = 0;
            scene_timer = 0;
            game.setState(Game::SPLASH);
            break;
            
        case Game::SPLASH:
            if (scene_timer++ < 120) {
                ardu.fillScreen(BLACK);
                Sprites::drawOverwrite(54,16, img_hocuspocus, 0);
            }
            else {
                scene_timer = 0;
                startTitle();
                return;
            }
            break;
            
        case Game::TITLE:
            if (scene_timer < 74) scene_timer++;
            if (scene_timer >= 25) {
                
                drawBackground();

                // Draw logo                   
                int8_t logo_y = 10-cos((scene_timer-25)/3.0)*(37-scene_timer/2);
                Sprites::drawOverwrite(21, logo_y, img_logo, 0);
                
                drawBuildings(); 
                drawScorePanel();

                // Draw 'press key'
                if (ardu.frameCount % 110 < 70 && scene_timer > 70) {
                    Sprites::drawExternalMask(20, 49, img_press_key, img_press_key_mask, 0, 0);
                }
                                          
                if (ardu.justPressed(A_BUTTON)) {
                    scene_timer = 0;
                    sound.tones(sfx_menuclick);
                    game.setState(Game::FOREPLAY); 
                    return;
                }  
                if (ardu.justPressed(B_BUTTON)) { 
                    sound.tones(sfx_menutoggle);
                    menu_active = true; 
                    return; 
                }  
            }
            else {
                ardu.fillScreen(BLACK);
            }
            break;
            
        case Game::FOREPLAY:
            ardu.fillScreen(BLACK);
            if (scene_timer++ > 60) { 
                scene_timer = 0; 
                sound.tones(sfx_startgame);
                startGame(); 
            }
            break;

        case Game::GAMEOVER_WAIT:
        case Game::GAMEOVER:             
        case Game::PLAYING:
        {    
            // Update game
            Chopper& chopper = game.chopper;
            if (game.state < Game::GAMEOVER_WAIT) {
                if (!chopper.rope.moving || chopper.timeout==0) handleInput();
            }           
            switch(game.update()) {
                case Game::END_GAME: startTitle(); return;
                case Game::POINT_SCORED: if (!sound.playing()) sound.tones(sfx_score); break;
                case Game::DUDE_SPAWNED: sound.tones(sfx_spawn); break;
                case Game::UFO_SPAWNED: sound.tones(sfx_ufo); break;
                case Game::BUILDING_COLLAPSING: sound.tone(80 + rand()%60); break;
                case Game::BUILDING_COLLAPSED:
                case Game::PLAYER_DIED: sound.noTone(); break;
                default: break;
            }
            if (game.spawn_timeout == 0) {
                if (game.dude_grabbed) sound.tones(sfx_dudegrab);
                else if (chopper.rope.dude_enter) sound.tones(sfx_dudeenter);
                else if (chopper.rope.stopped_moving) sound.noTone();   
                else if (chopper.rope.moving && !sound.playing()) sound.tones(chopper.rope.target_length==0 ? sfx_ropeup : sfx_ropedown);             
            }
            
            // Start drawing
            drawBackground();

            // Draw ufo
            if (game.ufo.active) {
                Sprites::drawExternalMask(game.ufo.x, game.ufo.y, img_ufo_bg, img_ufo_masks, 0, ufo_frames[game.ufo.current_frame]);
            }

            // Draw buildinfs/score
            drawBuildings();  
            drawScorePanel();
            
            // Draw dudes
            for (uint8_t i=0; i<MAX_DUDES; i++) {
                Dude& dude = game.dudes.items[i];
                if (dude.active && dude.state > DudeInfo::IN_CHOPPER) {
                    uint8_t dude_x = dude.global_x + dude.local_x;
                    uint8_t dude_y = dude.global_y;
                    uint8_t dude_frame = dude.current_frame + dude.dir*4;
                    if (dude.lifetime > DUDE_ENTER_TIME || dude.lifetime % 6 > 2) {
                        Sprites::drawExternalMask(dude_x, dude_y, img_dude_bg, img_dude_masks, 0, dude_frames[dude_frame]);
                    }
                }
            }                                           

            // Draw chopper
            if (chopper.active) {
                if (!chopper.exploding || chopper.counter < 95) {
                    // Draw rotor
                    Sprites::drawExternalMask(chopper.x, chopper.y, img_rotor_bg, img_rotor_masks, 0, rotor_frames[(chopper.counter/2)%6]);
                }
                if (!chopper.exploding) {
                    // Draw body
                    Sprites::drawExternalMask(chopper.x, chopper.y+4, img_chopper_bg, img_chopper_masks, 0, chopper_frames[chopper.current_frame]);
                    // Draw rope
                    for (uint8_t i=0; i<round(chopper.rope.length); i++) {
                        if (i%3 > 0) ardu.drawPixel(chopper.rope.x, chopper.rope.y+i, BLACK);
                        if (i>0 && i==round(chopper.rope.length)-1) ardu.drawFastHLine(chopper.rope.x-1, chopper.rope.y+i, 3, BLACK);
                    } 
                }
                else {
                    // Draw explosion
                    float fac = maxVal(0.1, 1.3-chopper.counter/120.0);
                    if (fac > 0.2) {
                        int8_t dir = chopper.current_frame < 3 ? 8 : -8;
                        int8_t offsy = chopper.counter/14;
                        drawCircle(chopper.x+10+(-1+rand()%3), chopper.y+4+4+offsy, abs(sin(chopper.counter/2.0)*6*fac));
                        if (4*fac>1) drawCircle(chopper.x+10+dir+(-2+rand()%5), chopper.y+4+4, abs(sin(chopper.counter/4.0)*4*fac));
                        if (3*fac>1) drawCircle(game.ufo.x+6+(-1+rand()%3), game.ufo.y+6+offsy, abs(sin(chopper.counter/3.0)*7*fac));
                        sound.tone(80 + rand()%160);
                    }
                    else sound.noTone();
                }  
            }               
            
            // Draw game over message
            if (game.state == Game::GAMEOVER) {
                if (game.gameover_counter == 60) { 
                    sound.tones(sfx_gameover);                     
                    if (game.score > game.hiscore) {
                        game.hiscore = game.score;
                        EEPROM.put(STORE_SCORE_ADDR, game.hiscore);
                        EEPROM.commit();
                    }
                }
                if (game.gameover_counter > GAMEOVER_MSG_FRAME && game.gameover_counter < GAMEOVER_DURATION-60) {
                    drawText(40,23, "GAME");
                    drawText(67,23, "OVER");
                }
            }
            else if (ardu.justPressed(B_BUTTON)) { 
                sound.tones(sfx_menutoggle);
                menu_active = true; 
            }
            break;
        }
    
        default:
            break;               
    }
}

//----------------------------------------------------------------------------
// Boot code
//----------------------------------------------------------------------------

void setup() 
{
    WiFi.mode(WIFI_OFF); //disable wifi to save some battery power
    ardu.begin();
    ardu.setFrameRate(60);
    sound.volumeMode(VOLUME_ALWAYS_NORMAL);

    // Read/prepare EEPROM store
    if (EEPROM.read(STORE_INIT_ADDR) == STORE_COOKIE) {
        game.hiscore = EEPROM.read(STORE_SCORE_ADDR);
    }
    else {
        //ardu.audio.on(); // first run always sound enabled (stick with system sound state)
        EEPROM.put(STORE_INIT_ADDR, STORE_COOKIE);
        EEPROM.put(STORE_SCORE_ADDR, (uint16_t)0);
        EEPROM.commit();
    } 
    
    // Add the buildings
    game.buildings.add(7, 64-img_building1[1]+4); 
    game.buildings.add(29, 64-img_building2[1]+4); 
    game.buildings.add(52, 64-img_building3[1]); 
    game.buildings.add(75, 64-img_building4[1]+1); 
    game.buildings.add(128-img_building5[0], 64-img_building5[1]-4); 
    game.setState(Game::SPLASH);
}

void loop() 
{
    if (ardu.nextFrame())
    {
        ardu.pollButtons();
        nextFrame();
        ardu.display();
    }    
}
