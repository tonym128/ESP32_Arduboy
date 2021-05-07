#include <Arduboy2.h>

void game_Init() {

    gameState = GameState::Game;

    mothership.reset(gameRotation, gameRotation == GameRotation::Landscape ? -Constants::MothershipRowHeight : Constants::MothershipRowHeight);
    gamePlayVars.bombCounter = random(256, 1024);

    player1.reset(0);
    player2.reset(1);

    switch (gameRotation) {

        case GameRotation::Portrait:

            if (gameMode == GameMode::Single) {

                player1.setPos(26);
                player1.setMovement(Movement::Up);
                
            }
            else {

                player1.setPlayerIdx(0);
                player1.setPos(13);
                player1.setMovement(Movement::Up);

                player2.setPos(38);
                player2.setMovement(Movement::Down);

            }

            break;

        case GameRotation::Landscape:

            if (gameMode == GameMode::Single) {

                player1.setPos(59);
                player1.setMovement(Movement::Left);
                
            }
            else {

                player1.setPlayerIdx(0);
                player1.setPos(33);
                player1.setMovement(Movement::Left);

                player2.setPos(84);
                player2.setMovement(Movement::Right);

            }

            break;

    }            

}   

void game() {


    // DEBUG
    // if (arduboy.justPressed(DOWN_BUTTON)) {

    //     bomb.setActive(true);
    //     bomb.setPos(mothership.getPosDisplay() + 6);
    //     bomb.setHeight(mothership.getHeight() + 6);

    // }


    // Randomly drop a bomb ?

    if (!gamePlayVars.waveCleared) {

        if (player1.getScore() + player2.getScore() > 20) {

            if ((gameRotation == GameRotation::Landscape && mothership.getHeight() < 30) || (gameRotation == GameRotation::Portrait && mothership.getHeight() > 56)) {

                gamePlayVars.bombCounter--;

                if (gamePlayVars.bombCounter == 0) {

                    #ifdef SOUNDS
                        sound.tones(Sounds::Enemy_Drops_Bomb);
                    #endif

                    bomb.setActive(true);
                    bomb.setPos(mothership.getPosDisplay() + 6);
                    bomb.setHeight(mothership.getHeight() + 6);

                    gamePlayVars.bombCounter = random(256, 1024);

                }

            }

        }

        // Handle movements ..

        if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {

            bool fired = player1.fire(gameRotation, gameMode, (gameMode == GameMode::Double ? &player2 : nullptr));

            #ifdef SOUNDS
                if (fired) sound.tones(Sounds::Player_Fires_Bullet);
            #endif

        }

        if ((arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) && gameMode == GameMode::Double) {

            bool fired = player2.fire(gameRotation, gameMode, (gameMode == GameMode::Double ? &player1 : nullptr));

            #ifdef SOUNDS
                if (fired) sound.tones(Sounds::Player_Fires_Bullet);
            #endif

        }


        if (gameMode == GameMode::Double) {

            movePlayer(player1, player2);
            movePlayer(player2, player1);
            mothership.move(gameRotation, gameMode, player1, player2);

        }
        else {

            movePlayer1();
            mothership.move(gameRotation, gameMode, player1);

        }

        if (player1.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(player1); 
        if (player2.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(player2);
        if (bomb.getActive()) moveBomb();

        player1.decExplodeCounter();
        player2.decExplodeCounter();


        // End of game?

        switch (gameRotation) {

            case GameRotation::Portrait:

                if (mothership.getHeight() < Constants::PlayerHeight) {

                    switch (mothership.getMovement()) {

                        case Movement::Up:
                            if (mothership.getPosDisplay() < -Constants::MothershipHeight) {
                                gameState = GameState::GameOver_Init;
                            }
                            break;

                        case Movement::Down:
                            if (mothership.getPosDisplay() > HEIGHT) {
                                gameState = GameState::GameOver_Init;
                            }
                            break;

                        default: break;

                    }

                }

                break;
            
            case GameRotation::Landscape:

                if (mothership.getHeight() > 64 - Constants::PlayerHeight - Constants::MothershipHeight) {

                    switch (mothership.getMovement()) {

                        case Movement::Left:
                            if (mothership.getPosDisplay() < -Constants::MothershipHeight) {
                                gameState = GameState::GameOver_Init;
                            }
                            break;

                        case Movement::Right:
                            if (mothership.getPosDisplay() > WIDTH) {
                                gameState = GameState::GameOver_Init;
                            }
                            break;

                        default: break;

                    }

                }

                break;


        }    

    }    



    // Render screen ---------------------------------------------------------------------

    renderScores(false, false);
    renderScenery(gameMode);
    updateAndRenderParticles(gameRotation);

    switch (gameRotation) {

        case GameRotation::Portrait:

            Sprites::drawExternalMask(0, player1.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

            if (player1.getExplosionCounter() > 0) {

                Sprites::drawSelfMasked(0, player1.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - player1.getExplosionCounter()) / 2);

            }

            if (gameMode == GameMode::Double) {

                Sprites::drawExternalMask(0, player2.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

                if (player2.getExplosionCounter() > 0) {

                    Sprites::drawSelfMasked(0, player2.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - player2.getExplosionCounter()) / 2);

                }

            }

            switch (mothership.getExplosionCounter()) {

                case 0:
                    Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                    break;

                case 1 ... Constants::MothershipExplosionMax / 2:
                    Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                    break;

                case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax:
                    Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                    Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                    break;

            }

            if (player1.getBulletActive())  Sprites::drawExternalMask(player1.getBulletX(), player1.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
            if (player2.getBulletActive())  Sprites::drawExternalMask(player2.getBulletX(), player2.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
            
            if (bomb.getActive()) {

                if (bomb.getExplosionCounter() == 0) {

                    Sprites::drawExternalMask(bomb.getHeight(), bomb.getPos(), Images::Portrait::Bomb, Images::Portrait::Bomb_Mask, 0, 0);
                    
                }

                switch (bomb.getHeight()) {

                    case -2 ... -1:
                        Sprites::drawExternalMask(0, bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                        break;

                    case -4 ... -3:
                        Sprites::drawExternalMask(0, bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                        break;

                }        

                if (bomb.getExploding()) {

                    switch (bomb.getExplosionCounter()) {

                        case 1 ... 2:
                            Sprites::drawExternalMask(bomb.getHeight(), bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                            break;

                        case 3 ... 4:
                            Sprites::drawExternalMask(bomb.getHeight(), bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                            break;

                    }        

                }

            }    
            break;

        case GameRotation::Landscape:

            Sprites::drawExternalMask(player1.getPos(), 56, Images::Landscape::Player, Images::Landscape::Player_Mask, 0, 0);

            if (player1.getExplosionCounter() > 0) {

                Sprites::drawSelfMasked(player1.getPos() - 4, 52, Images::Landscape::Player_Explosion, (6 - player1.getExplosionCounter()) / 2);

            }

            if (gameMode == GameMode::Double) {

                Sprites::drawExternalMask(player2.getPos(), 56, Images::Landscape::Player, Images::Landscape::Player_Mask, 0, 0);

                if (player2.getExplosionCounter() > 0) {

                    Sprites::drawSelfMasked(player2.getPos() - 4, 52, Images::Landscape::Player_Explosion, (6 - player2.getExplosionCounter()) / 2);

                }

            }

            switch (mothership.getExplosionCounter()) {

                case 0:
                    Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Mothership, Images::Landscape::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                    break;

                case 1 ... Constants::MothershipExplosionMax / 2:
                    Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 1, 1);
                    break;

                case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax:
                    Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Mothership, Images::Landscape::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                    Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 0, 0);
                    break;

            }

            if (player1.getBulletActive())  Sprites::drawExternalMask(player1.getBulletX(), player1.getBulletY(), Images::Landscape::Laser, Images::Landscape::Laser_Mask, 0, 0);
            if (player2.getBulletActive())  Sprites::drawExternalMask(player2.getBulletX(), player2.getBulletY(), Images::Landscape::Laser, Images::Landscape::Laser_Mask, 0, 0);
            
            if (bomb.getActive()) {

                if (bomb.getExplosionCounter() == 0) {

                    Sprites::drawExternalMask(bomb.getPos(), bomb.getHeight(), Images::Landscape::Bomb, Images::Landscape::Bomb_Mask, 0, 0);

                }

                switch (bomb.getHeight()) {

                    case 60 ... 62:
                        Sprites::drawExternalMask(bomb.getPos() - 4, 58, Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 0, 0);
                        break;

                    case 63 ... 64:
                        Sprites::drawExternalMask(bomb.getPos() - 4, 58, Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 1, 1);
                        break;

                }        

                if (bomb.getExploding()) {

                    switch (bomb.getExplosionCounter()) {

                        case 1 ... 2:
                            Sprites::drawExternalMask(bomb.getPos() - 4, bomb.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 1, 1);
                            break;

                        case 3 ... 4:
                            Sprites::drawExternalMask(bomb.getPos() - 4, bomb.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 0, 0);
                            break;

                    }        

                }

            }        

            break;

    }

    if (gamePlayVars.waveCleared) {


        // Clear an bullets from the screen ..

        if (player1.getBulletActive()) player1.setBulletActive(false);
        if (player2.getBulletActive()) player2.setBulletActive(false);

        if (gamePlayVars.waveCounter == 8) {
            #ifdef SOUNDS
                sound.tones(Sounds::Wave_Cleared);
            #endif
        }

        if (gamePlayVars.waveCounter >= 55 && arduboy.isFrameCount(64)) { launchParticles(gameRotation); }

        if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {

            gamePlayVars.waveCounter = 0;
            gamePlayVars.waveCleared = false;

        }

        if (gameMode == GameMode::Double) {

            movePlayer(player1, player2);
            movePlayer(player2, player1);


            if (mothership.getExplosionCounter() > 0) mothership.move(gameRotation, gameMode, player1, player2);

        }
        else {

            movePlayer1();
            if (mothership.getExplosionCounter() > 0) mothership.move(gameRotation, gameMode, player1);

        }

        uint8_t idx = gamePlayVars.waveCounter / 8;

        updateAndRenderParticles(gameRotation);

        switch (gameRotation) {

            case GameRotation::Portrait:
                arduboy.drawRect(60, 32 - (idx * 4), 9, 4 + (idx * 8), BLACK);
                Sprites::drawSelfMasked(60, 32 - (idx * 4), Images::Portrait::WaveCleared[idx], 0);
                break;

            case GameRotation::Landscape:
                arduboy.drawRect(60 - (idx * 4), 24, 4 + (idx * 8), 9, BLACK);
                Sprites::drawSelfMasked(60 - (idx * 4), 24, Images::Landscape::WaveCleared[idx], 0);
                break;

        }         

        if (gamePlayVars.waveCounter < 55) gamePlayVars.waveCounter++;

    }

}


void movePlayer(Player &player, Player &otherPlayer) {

    if (arduboy.isFrameCount(2, player.getPlayerIdx())) {

        switch (player.getMovement()) {

            case Movement::Up:

                if (player.getPos() > Constants::Portrait::PlayerMinPos) {

                    player.decPos();
                    
                    if (gameMode == GameMode::TugOfWar) return;

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask ) {
                        
                        if (otherPlayer.getPos() > Constants::Portrait::PlayerMinPos && otherPlayer.getMovement() == Movement::Down) {
                            otherPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.incPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;

            case Movement::Down:

                if (player.getPos() < Constants::Portrait::PlayerMaxPos) {

                    player.incPos();
                    
                    if (gameMode == GameMode::TugOfWar) return;

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask) {

                        if (otherPlayer.getPos() < Constants::Portrait::PlayerMaxPos && otherPlayer.getMovement() == Movement::Up) {
                            otherPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.decPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;

            case Movement::Left:

                if (player.getPos() > Constants::Landscape::PlayerMinPos) {

                    player.decPos();

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask ) {
                        
                        if (otherPlayer.getPos() > Constants::Landscape::PlayerMinPos && otherPlayer.getMovement() == Movement::Right) {
                            otherPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.incPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;

            case Movement::Right:

                if (player.getPos() < Constants::Landscape::PlayerMaxPos) {

                    player.incPos();

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask) {

                        if (otherPlayer.getPos() < Constants::Landscape::PlayerMaxPos && otherPlayer.getMovement() == Movement::Left) {
                            otherPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.decPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;


        }

    }

}


void movePlayer1() {

    if (arduboy.isFrameCount(2, 0)) {

        switch (player1.getMovement()) {

            case Movement::Up:
                player1.decPos();
                if (player1.getPos() == Constants::Portrait::PlayerMinPos) {
                    player1.changeMovement(gameRotation);
                }
                break;

            case Movement::Left:
                player1.decPos();
                if (player1.getPos() == Constants::Landscape::PlayerMinPos) {
                    player1.changeMovement(gameRotation);
                }
                break;

            case Movement::Down:
                player1.incPos();
                if (player1.getPos() == Constants::Portrait::PlayerMaxPos) {
                    player1.changeMovement(gameRotation);
                }
                break;

            case Movement::Right:
                player1.incPos();
                if (player1.getPos() == Constants::Landscape::PlayerMaxPos) {
                    player1.changeMovement(gameRotation);
                }
                break;

        }

    }

}

void moveBullet(Player &player) {

    switch (gameRotation) {

        case GameRotation::Portrait:

            switch (gameMode) {
            
                case GameMode::Single ... GameMode::Double:
                    player.incBulletX();
                    break;
            
                case GameMode::TugOfWar:

                    switch (player.getPlayerIdx()) {

                        case 0:
                            player.incBulletX();
                            break;

                        case 1:
                            player.decBulletX();
                            break;

                    }
                    break;

            }

            break;

        case GameRotation::Landscape:
            if (arduboy.isFrameCount(2, 0)) {
                player.decBulletY();
            }
            break;

    }

    if (mothership.getExplosionCounter() != 0) return;
    if (!player.getBulletActive()) return;


    switch (gameRotation) {

        case GameRotation::Portrait:
            {
                Rect bulletRect = { player.getBulletX() + 1, player.getBulletY() +1, Constants::BulletHeight - 2, Constants::BulletWidth - 2 };
                Rect mothershipRect = { mothership.getHeight() + 1, mothership.getPosDisplay() + 1, Constants::MothershipWidth - 2, Constants::MothershipHeight - 2 };

                if (arduboy.collide(bulletRect, mothershipRect)) {

                    #ifdef SOUNDS
                        sound.tones(Sounds::Enemy_Explosion);
                    #endif


                    launchParticles(gameRotation, mothership.getPosDisplay() + (Constants::MothershipHeight / 2), mothership.getHeight() + (Constants::MothershipWidth / 2));


                    if (gameMode == GameMode::TugOfWar) {
                        mothership.explode(player.getPlayerIdx() == 0 ? -Constants::TugOfWarRowAdjustment : Constants::TugOfWarRowAdjustment);
                        gamePlayVars.waveCleared = false;
                        mothership.decCounter();
                    }
                    else {
                        mothership.explode(Constants::MothershipRowHeight);
                        gamePlayVars.waveCleared = mothership.decCounter();
                    }

                    player.incScore();
                    player.setBulletActive(false);

                }

                if (bomb.getActive()) {

                    Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombHeight - 2, Constants::BombWidth - 2 };

                    if (arduboy.collide(bulletRect, bombRect)) {

                        #ifdef SOUNDS
                            sound.tones(Sounds::Bomb_Explosion);
                        #endif

                        bomb.explode();
                        player.setBulletActive(false);

                    }
                    
                }

            }

            break;

        case GameRotation::Landscape:
            {        
                Rect bulletRect = { player.getBulletX() + 1, player.getBulletY() + 1, Constants::BulletWidth - 2, Constants::BulletHeight - 2 };
                Rect mothershipRect = { mothership.getPosDisplay() + 1, mothership.getHeight() + 1, Constants::MothershipWidth - 2, Constants::MothershipHeight - 2 };

                if (arduboy.collide(bulletRect, mothershipRect)) {

                    #ifdef SOUNDS
                        sound.tones(Sounds::Enemy_Explosion);
                    #endif

                    launchParticles(gameRotation, mothership.getPosDisplay() + (Constants::MothershipHeight / 2), mothership.getHeight() + (Constants::MothershipWidth / 2));

                    mothership.explode(-Constants::MothershipRowHeight);                    
                    gamePlayVars.waveCleared = mothership.decCounter();

                    player.incScore();
                    player.setBulletActive(false);

                }

                if (bomb.getActive()) {

                    Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombWidth - 2, Constants::BombHeight - 2 };

                    if (arduboy.collide(bulletRect, bombRect)) {

                        #ifdef SOUNDS
                            sound.tones(Sounds::Bomb_Explosion);
                        #endif

                        bomb.explode();
                        player.setBulletActive(false);

                    }
                    
                }

            }

            break;

    }

}

void moveBomb() {

    bomb.decHeight(gameRotation);

    if (!bomb.getActive() || bomb.getExplosionCounter() > 0) return;

    switch (gameRotation) {

        case GameRotation::Portrait:
            {
                Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombHeight - 2, Constants::BombWidth - 2 };
                Rect player1Rect = { 0, player1.getPos() + 1, Constants::PlayerHeight - 2, Constants::PlayerWidth - 2 };

                if (arduboy.collide(bombRect, player1Rect)) {

                    bool explode = player1.explode();

                    #ifdef SOUNDS
                        if (explode) {
                            sound.tones(Sounds::Player_Hit_By_Bomb);
                        }
                    #endif
                }

                if (gameMode != GameMode::Single) {

                    Rect player2Rect = { 0, player2.getPos() + 1, Constants::PlayerHeight - 2, Constants::PlayerWidth - 2 };

                    if (arduboy.collide(bombRect, player2Rect)) {

                        bool explode = player2.explode();

                        #ifdef SOUNDS
                            if (explode) {
                                sound.tones(Sounds::Player_Hit_By_Bomb);
                            }
                        #endif

                    }

                }

            }

            break;

        case GameRotation::Landscape:
            {        
                Rect bombRect = { bomb.getPos() + 1, bomb.getHeight() + 1, Constants::BombWidth - 2, Constants::BombHeight - 2 };
                Rect player1Rect = { player1.getPos() + 1, 64 - Constants::PlayerHeight + 1, Constants::PlayerWidth - 2, Constants::PlayerHeight - 1 };

                if (arduboy.collide(bombRect, player1Rect)) {

                    bool explode = player1.explode();

                    #ifdef SOUNDS
                        if (explode) {
                            sound.tones(Sounds::Player_Hit_By_Bomb);
                        }
                    #endif

                }

                if (gameMode != GameMode::Single) {

                    Rect player2Rect = { player2.getPos() + 1, 64 - Constants::PlayerHeight + 1, Constants::PlayerWidth - 2, Constants::PlayerHeight - 1 };

                    if (arduboy.collide(bombRect, player2Rect)) {

                        bool explode = player2.explode();

                        #ifdef SOUNDS
                            if (explode) {
                                sound.tones(Sounds::Player_Hit_By_Bomb);
                            }
                        #endif

                    }

                }

            }

            break;

    }

}
