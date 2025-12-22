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

        BidType getBidType() {

            switch (this->mode) {

                case BidMode::Bid:
                    return BidType::Partner;

                case BidMode::Alone:
                    return BidType::Alone;
                    break;

                case BidMode::Pass:
                    return BidType::Pass;
                    break;

            }

        }

        void incMode() {
            this->mode++;
        }

        void decMode() {
            this->mode--;
        }

        void decSuit(Suit ignoreSuit) {

            switch (this->suit) {
                
                case Suit::Spades:
                    break;
                
                case Suit::Clubs:
                    if (ignoreSuit != Suit::Spades) this->suit--;
                    break;
                
                case Suit::Diamonds:
                    if (ignoreSuit != Suit::Clubs) this->suit = Suit::Clubs;
                    if (ignoreSuit == Suit::Clubs) this->suit = Suit::Spades;
                    break;
                
                case Suit::Hearts:
                    if (ignoreSuit != Suit::Diamonds) this->suit = Suit::Diamonds;
                    if (ignoreSuit == Suit::Diamonds) this->suit = Suit::Clubs;
                    break;

            }

        }

        void incSuit(Suit ignoreSuit) {
        
            switch (this->suit) {
                
                case Suit::Spades:
                    if (ignoreSuit != Suit::Clubs) this->suit = Suit::Clubs;
                    if (ignoreSuit == Suit::Clubs) this->suit = Suit::Diamonds;
                    break;
                
                case Suit::Clubs:
                    if (ignoreSuit != Suit::Diamonds) this->suit = Suit::Diamonds;
                    if (ignoreSuit == Suit::Diamonds) this->suit = Suit::Hearts;
                    break;
                
                case Suit::Diamonds:
                    if (ignoreSuit != Suit::Hearts) this->suit++;
                    break;
                
                case Suit::Hearts:
                    break;

            }
            
        }

};

