#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/utils/SpritesU.hpp"



uint8_t getJustPressedButtons() {

    a.pollButtons();

    return a.justPressedButtons();

}


uint8_t getPressedButtons() {

    return a.pressedButtons();

}


void play_CardSelected() {

    incHand();
    gameState = GameState::Play_Hand;


    // Is it the end of the round?

    if (gameRound.getFirstPlayer_Idx() == gameRound.getCurrentPlayer_Idx()) {

        #ifdef DEBUG_BASIC
            DEBUG_PRINTLN(F("-----"));
            DEBUG_PRINT(F("Trick winner "));
            DEBUG_PRINTLN(gameRound.getWinningHand());
        #endif


        // Increase tricks won count and scores ..
        
        gameRound.incTricksWon(gameRound.getWinningHand());

    
        // Did anyone not follow suit?

        Suit cardLedSuit = gameRound.getCardLed().getSuit();
        Suit trumpsSuit = gameRound.winningBid_Suit();
        bool cardLedIsTrump = gameRound.getCardLed().isTrump(trumpsSuit);
        BidType bidType = gameRound.winningBid_Type();

        for (uint8_t playerIdx = 0; playerIdx < 4; playerIdx++) {

            if (cardLedIsTrump) {

                if (!gameRound.getHand(playerIdx)->isTrump(trumpsSuit)) {
                    gameRound.setHasSuitInHand(playerIdx, cardLedSuit, TriState::False);
                }
            
            }
            else {

                if (gameRound.getHand(playerIdx)->getSuit() != cardLedSuit) {
                    gameRound.setHasSuitInHand(playerIdx, cardLedSuit, TriState::False);
                }

            }

        }

        // If playing misere, did the calling hand win?

        #ifdef DEBUG_BASIC
            DEBUG_PRINT(F("Highest hand "));
            DEBUG_PRINT(gameRound.getWinningHand());
            DEBUG_PRINT(F(", lowest hand "));
            DEBUG_PRINT(gameRound.getWinningHand());
            DEBUG_PRINT(F(", WinningBidIdx "));
            DEBUG_PRINTLN(gameRound.getWinningBid_Idx());
            DEBUG_PRINTLN(F("-------"));
        #endif

        gameRound.resetHand(gameRound.getWinningHand());        
        gameState = GameState::Play_EndOfHand;

    }

    game.setFrameCount(0);
    if (!playerAssist) highlightSuitInHand();

}

void highlightSuitInHand() {

    selectedCard = 0;
    game.players[gameRound.getCurrentPlayer_Idx()].clearSelection();
    game.players[gameRound.getCurrentPlayer_Idx()].getCard(selectedCard).setSelected(true);

}