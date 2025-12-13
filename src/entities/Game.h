#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Card.h"
#include "Player.h"
#include "GameRound.h"

struct Game {

    private:

        uint16_t randomSeed = 0;
        uint16_t frameCount = 0;
        uint8_t selecteCard = 255;

    public:

        Player players[4];
        Deck deck;
        GameRound *gameRound;

        uint16_t getFrameCount()                        { return this->frameCount; }
        uint16_t getFrameCount(uint8_t val)             { return this->frameCount % val < val / 2; }
        uint8_t getSelectedCard()                       { return this->selecteCard; }

        void setRandomSeed(uint16_t val)                { this->randomSeed = val; }
        void setFrameCount(uint16_t val)                { this->frameCount = val; }
        void setSelectedCard(uint8_t val)               { this->selecteCard = val; }


        void incFrameCount() {

            this->frameCount++;

        }

        void resetFrameCount() {

            this->frameCount = 0;
            
        }


};