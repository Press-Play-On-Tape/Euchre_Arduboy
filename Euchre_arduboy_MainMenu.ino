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

                switch (assistCursor) {
                
                    case 0:
                        playerAssist = !playerAssist;
                        break;
                    
                    case 1:
                        screwDealer = !screwDealer;
                        break;

                }

            }
      
            if (justPressed & UP_BUTTON) {

                switch (assistCursor) {
                    
                    case 1:
                        assistCursor = 0;
                        break;

                }

            }

            if (justPressed & DOWN_BUTTON) {

                switch (assistCursor) {
                    
                    case 0:
                        assistCursor = 1;
                        break;
                    
                    case 1:
                        gameState = GameState::Play_Init;
                        break;

                }

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

            SpritesU::drawOverwriteFX(0, 0, Images::TitleEuchre_Assist, (assistCursor * 3) + currentPlane);

            if (playerAssist)   SpritesU::drawPlusMaskFX(13, 9, Images::TitleEuchre_Assist_Cursor, (assistCursor * 3) + currentPlane);
            if (screwDealer)    SpritesU::drawPlusMaskFX(13, 27, Images::TitleEuchre_Assist_Cursor, ((1 - assistCursor) * 3) + currentPlane);

            break;

    }            

}
