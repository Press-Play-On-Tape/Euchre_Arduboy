#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Card.h"

struct Bid {

    private:
    
        Suit suit = Suit::None;       
        uint8_t playerIdx = 0;
        BidType type = BidType::None;
        uint16_t bidScore = 0;                          // The internal evaluation score for the hand

    public:

        Suit getSuit()                                  { return this->suit; }
        BidType getBidType()                            { return this->type; }
        uint8_t getPlayerIdx()                          { return this->playerIdx; }
        uint16_t getBidScore()                          { return this->bidScore; }

        void setSuit(Suit val)                          { this->suit = val; }
        void setBidType(BidType val)                    { this->type = val; }
        void setBidScore(uint16_t val)                  { this->bidScore = val; }
        void setPlayerIdx(uint16_t val)                 { this->playerIdx = val; }

        void decSuit() {
        
            if (this->suit > Suit::Spades) this->suit--;
            
        }

        void incSuit() {
        
            if (this->suit < Suit::Hearts) this->suit++;
            
        }

        void reset(uint8_t playerIdx) {
        
            this->playerIdx = playerIdx;
            this->type = BidType::None;
            this->suit = Suit::None;
            this->bidScore = 0;
            
        }
 
};