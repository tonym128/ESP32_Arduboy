#include <Arduboy2.h>

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void gameOver_Init() {

    gameState = GameState::GameOver;
    
}   


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void gameOver() {

    draw(false, true);

    renderPlayerCar();
    renderHud();
    renderGameOver();


    if (arduboy.justPressed(A_BUTTON)) { 

        gameState = GameState::SaveScore;

    }

    houseKeeping();

    #ifdef CACTII
        moveCactii();
    #endif
    
    cameraPos.setZ(cameraPos.getZ() + gameOverVars.finalSpeed);
    cameraPos.setY(world.roadHeightAt(cameraPos.getZ() + 2 * UPM) + UPM);

    gamePlayVars.timeOfDay = 700;

}
