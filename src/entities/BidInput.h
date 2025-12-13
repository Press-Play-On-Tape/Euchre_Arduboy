#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"

struct BidInput {

    private:
    
        BidMode mode = BidMode::Pass;
        BidMode prevMode = BidMode::Pass;
        Suit suit = Suit::Spades;

    public:

        const BidMode getMode()                     { return this->mode; }
        const BidMode getPrevMode()                 { return this->prevMode; }
        const Suit getSuit()                        { return this->suit; }

        void setMode(BidMode val)                   { this->prevMode = this->mode; this->mode = val; }
        void setSuit(Suit val)                      { this->suit = val; }

        void reset() {

            mode = BidMode::Pass;
            suit = Suit::Spades;

        }

        void incMode() {
            this->mode++;
        }

        void decMode() {
            this->mode--;
        }

        void decSuit() {
        
            if (this->suit > Suit::Spades) this->suit--;
            
        }

        void incSuit() {
        
            if (this->suit < Suit::Hearts) this->suit++;
            
        }

};

