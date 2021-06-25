#include <Arduboy2.h>

void tugOfWar_Init() {

    gameState = GameState::TugOfWar;

    mothership.reset(gameRotation, 0);
    mothership.setHeight(60);

    player1.reset(0);
    player2.reset(1);

    player1.setPos(45);

}   

void tugOfWar() {

// if (arduboy.justPressed(LEFT_BUTTON)) {
// mothership.explode(-Constants::TugOfWarRowAdjustment);
// }

// if (arduboy.justPressed(RIGHT_BUTTON)) {
// mothership.explode(Constants::TugOfWarRowAdjustment);
// }
    // Handle movements ..

    if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {

        bool fired = player1.fire(gameRotation, gameMode, nullptr);

        #ifdef SOUNDS
            if (fired) sound.tones(Sounds::Player_Fires_Bullet);
        #endif

    }

    if ((arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))) {

         bool fired = player2.fire(gameRotation, gameMode, nullptr);

        #ifdef SOUNDS
            if (fired) sound.tones(Sounds::Player_Fires_Bullet);
        #endif

    }
    
    movePlayer(player1, player2);
    movePlayer(player2, player1);
    mothership.moveTugOfWar(player1, player2);

    if (player1.getBulletActive())      moveBullet(player1); 
    if (player2.getBulletActive())      moveBullet(player2);



    // End of game?

    if ((mothership.getHeight() <= Constants::PlayerHeight) ||
        (mothership.getHeight() >= 124 - Constants::PlayerHeight)) {

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
       

    renderScenery(gameMode);
    renderScoreTugOfWar(player1.getScore(), player2.getScore());
    updateAndRenderParticles(GameRotation::Landscape);
    


    Sprites::drawExternalMask(0, player1.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

    if (player1.getExplosionCounter() > 0) {

        Sprites::drawSelfMasked(0, player1.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - player1.getExplosionCounter()) / 2);

    }

    Sprites::drawExternalMask(120, player2.getPos(), Images::Portrait::Rotated::Player, Images::Portrait::Rotated::Player_Mask, 0, 0);

    if (player2.getExplosionCounter() > 0) {

        Sprites::drawSelfMasked(120, player2.getPos() - 4, Images::Portrait::Rotated::Player_Explosion, (6 - player2.getExplosionCounter()) / 2);

    }


    if (mothership.getHeight() < 64) {

        switch (mothership.getExplosionCounter()) {

            case 0:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                break;

            case 1 ... Constants::MothershipExplosionMax / 2:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                break;

            case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax - 1:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                break;

            case Constants::MothershipExplosionMax:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                launchParticles(GameRotation::Landscape, mothership.getHeight() + (Constants::MothershipWidth / 2), mothership.getPosDisplay() + (Constants::MothershipHeight / 2));
                break;

        }

    }
    else {

        switch (mothership.getExplosionCounter()) {

            case 0:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Rotated::Mothership, Images::Portrait::Rotated::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                break;

            case 1 ... Constants::MothershipExplosionMax / 2:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                break;

            case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax - 1:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Rotated::Mothership, Images::Portrait::Rotated::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                break;

            case Constants::MothershipExplosionMax:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Rotated::Mothership, Images::Portrait::Rotated::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                launchParticles(GameRotation::Landscape, mothership.getHeight() + (Constants::MothershipWidth / 2), mothership.getPosDisplay() + (Constants::MothershipHeight / 2));
                break;

        }

    }

    if (player1.getBulletActive())  Sprites::drawExternalMask(player1.getBulletX(), player1.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
    if (player2.getBulletActive())  Sprites::drawExternalMask(player2.getBulletX(), player2.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);


}
