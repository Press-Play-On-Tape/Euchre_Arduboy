#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/utils/SpritesU.hpp"


void title_Init() {

    gameState = GameState::Title_OptPlay;
    titleCounter = 0;
    game.resetFrameCount();

}

void title_Update() {

    game.incFrameCount();

    uint8_t justPressed = getJustPressedButtons();
    uint8_t pressed = getPressedButtons();

    switch (gameState) {

        case GameState::Title_OptPlay:

            if ((titleCounter == 24) && (justPressed & A_BUTTON)) {

                a.initRandomSeed(); 
                gameState = GameState::Title_Assist;

            }

            if (titleCounter < 24) titleCounter++;

            break;

        case GameState::Title_Assist:
      
            if (justPressed & A_BUTTON) {

                playerAssist = true;
                gameState = GameState::Play_Init;

            }
      
            if (justPressed & B_BUTTON) {

                playerAssist = false;
                gameState = GameState::Play_Init;

            }

            if (titleCounter < 24) titleCounter++;

            break;

    }

}

void title(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {

    if (a.needsUpdate()) title_Update();

    uint8_t currentPlane = a.currentPlane();

    switch (gameState) {

        case GameState::Title_OptPlay:
            {
                uint8_t idx = 0;
                if ((game.getFrameCount() % 256) < 64) idx = game.getFrameCount() % 256 / 2;
                SpritesU::drawOverwriteFX(0, 0, Images::TitleEuchre, (idx * 3) + currentPlane);
            }

            break;

        case GameState::Title_Assist:

            SpritesU::drawOverwriteFX(0, 0, Images::TitleEuchre_Assist, currentPlane);

            break;

    }            

}
