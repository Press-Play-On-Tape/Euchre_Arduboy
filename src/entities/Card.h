#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "../utils/Utils.h"
#include "Card.h"

struct Card {

    public:

        Suit suit;
        Rank rank;
        bool selected = false;
        Suit origSuit;
        Rank origRank;

        Suit getSuit()                      { return this->suit; }
        Suit getOrigSuit()                  { return this->origSuit; }
        Rank getRank()                      { return this->rank; }
        Rank getOrigRank()                  { return this->origRank; }
        bool isSelected()                   { return this->selected; }

        void setSuit(Suit val)              { this->suit = val; }
        void setOrigSuit(Suit val)          { this->origSuit = val; }
        void setRank(Rank val)              { this->rank = val; }
        void setOrigRank(Rank val)          { this->origRank = val; }
        void setSelected(bool val)          { this->selected = val; }

        void reset() {
        
            this->setSuit(Suit::None);
            this->setRank(Rank::None);
            this->setSelected(false);
            
        }

        void init(uint8_t cardIndex) {

            this->suit = static_cast<Suit>(cardIndex / 13);
            this->rank = static_cast<Rank>(cardIndex % 13);

        }

        void init(Suit suit, uint8_t cardNumber) {

            this->suit = suit;
            this->rank = static_cast<Rank>(cardNumber);

        }

        void init(Suit suit, Rank rank) {

            this->suit = suit;
            this->rank = rank;

            this->origSuit = this->suit;
            this->origRank = this->rank;

        }

        void setTrumps(Suit trumps) {
        
            this->origSuit = this->suit;
            this->origRank = this->rank;

            if (this->isLeftBower(trumps)) {
                this->suit = trumps;
                this->rank = Rank::Left_Bower;
            }

            if (this->isRightBower(trumps)) {
                this->suit = trumps;
                this->rank = Rank::Right_Bower;
            }

        }


        uint8_t getCardIndex() {

            return (3 * ((static_cast<uint8_t>(this->getOrigSuit()) * 11) + (static_cast<uint8_t>(this->getOrigRank()) - 4)));
            
        }


        uint8_t getSortValue(Suit trumps, BidType bidType, uint8_t seq[4]) {

            #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                DEBUG_PRINT(F("Card "));
                DEBUG_PRINT_CARD(this.getRanks(), this.getSuit);
                DEBUG_PRINT_SPACE();
            #endif

            switch (bidType) {
            
                case BidType::None:
                case BidType::Pass:

                    // if (suit == Suit::None) {

                    //     #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                    //         DEBUG_PRINTLN(Constants::Sort_None);
                    //     #endif

                    //     return Constants::Sort_None;
                        
                    // }                    

                    if (rank == Rank::None) {

                        #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                            DEBUG_PRINTLN(Constants::Sort_None);
                        #endif

                        return Constants::Sort_None;
                        
                    }      

                    return (seq[static_cast<uint8_t>(this->suit)] * 20) + (uint8_t)getRank();

                default:

                    // if (suit == Suit::None) {

                    //     #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                    //         DEBUG_PRINTLN(Constants::Sort_None);
                    //     #endif

                    //     return Constants::Sort_None;
                        
                    // }                    
                    if (rank == Rank::None) {

                        #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                            DEBUG_PRINTLN(Constants::Sort_None);
                        #endif

                        return Constants::Sort_None;
                        
                    }                    
                    else if (isLeftBower(trumps)) {

                        #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                            DEBUG_PRINTLN((seq[static_cast<uint8_t>(getTrump_AltSuit(this->suit))] * 20) + (uint8_t)getRank(trumps));
                        #endif

                        return (seq[static_cast<uint8_t>(getTrump_AltSuit(this->suit))] * 20) + (uint8_t)Rank::Left_Bower;

                    }
                    else {
                      
                        #if defined(DEBUG) && defined(DEBUG_SORT) && defined(DEBUG_SORT_DETAIL)
                            DEBUG_PRINTLN((seq[static_cast<uint8_t>(this->suit)] * 20) + (uint8_t)getRank(trumps));
                        #endif  

                        return (seq[static_cast<uint8_t>(this->suit)] * 20) + (uint8_t)getRank();

                    }

                    return (seq[static_cast<uint8_t>(this->getSuit())] * 20) + (uint8_t)this->getRank();         

            }

        }


        bool isTrump(Suit trumps) {
        
            if (this->getSuit() == trumps) {
                return true;
            }

            return false;

        }

        bool isRightBower(Suit trumpSuit) const {

            if (this->getRank() != Rank::Jack) return false;

            return (trumpSuit == this->getSuit());

        }

        bool isLeftBower(Suit trumpSuit) const {

            if (this->getRank() != Rank::Jack) return false;

            Suit cardSuit = this->getSuit();

            if (trumpSuit == Suit::Spades && cardSuit == Suit::Clubs)     return true;
            if (trumpSuit == Suit::Clubs && cardSuit == Suit::Spades)     return true;
            if (trumpSuit == Suit::Diamonds && cardSuit == Suit::Hearts)  return true;
            if (trumpSuit == Suit::Hearts && cardSuit == Suit::Diamonds)  return true;

            return false;

        }

};

