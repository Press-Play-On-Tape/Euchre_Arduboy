#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Card.h"

struct SuitPile {

    Suit suit;
    Card cards[11];
    uint8_t card_count = 0;
    uint8_t score = 0;
    uint8_t rankScore = 0;

    void reset(Suit suit) {
    
        this->suit = suit;

        for (uint8_t j = 0; j < 11; j++) {
            this->cards[j].reset();
        }

    }

    void sort() {
    
        uint8_t seq[4] = { 1, 1, 1, 1 };


        // Do a bubble sort ..

        for (uint8_t i = 0; i < 11; i++) {

            for (uint8_t j = i + 1; j < 11; j++) {

                if (this->cards[j].getSortValue(Suit::None, BidType::Pass, seq) < this->cards[i].getSortValue(Suit::None, BidType::Pass, seq)) {

                    Card card;
                    card.setSuit(this->cards[i].getSuit());
                    card.setRank(this->cards[i].getRank());

                    this->cards[i] = this->cards[j];
                    this->cards[j].setSuit(card.getSuit());
                    this->cards[j].setRank(card.getRank());

                }
                
            }

        }

    }

    void calcMisereScore() {
    
        Rank lowestRank;

        #if defined(DEBUG) && defined(DEBUG_CALC_MISERE_SCORE)
            DEBUG_PRINT(F("calcMisereScore, suit: "));
            DEBUG_PRINT_SUIT(this->suit);
            DEBUG_PRINT(F(":  "));
        #endif
        
        switch (this->suit) {
        
            case Suit::Spades:
            case Suit::Clubs:
                lowestRank = Rank::Five;
                break;

            case Suit::Diamonds:
            case Suit::Hearts:
                lowestRank = Rank::Four;
                break;

        }


        // No cards?

        if (this->cards[0].getSuit() == Suit::None) {

            #if defined(DEBUG) && defined(DEBUG_CALC_MISERE_SCORE)
                DEBUG_PRINTLN(F("1. No cards = 255"));
            #endif

            this->score = Constants::No_Card;
            return;
        }


        // First pass: Do we have any suits where there no small cards and there are more than 3 bigger cards?  

        if (this->cards[0].getRank() >= Rank::Six && this->card_count >= 4) {

            #if defined(DEBUG) && defined(DEBUG_CALC_MISERE_SCORE)
                DEBUG_PRINTLN(F("2. Too big = 0"));
            #endif

            this->score = 0; // Too big
            return;

        }


        // Are all the cards a bottom up run?
        
        uint8_t num_of_cards = 0;

        for (uint8_t i = 0; i < 11; i++) {

            if (this->cards[i].getSuit() == Suit::None) break;

            if (this->cards[i].getRank() == lowestRank) {

                num_of_cards++;
                lowestRank++;
            
            }
            else {
            
                num_of_cards = 0;
                break;
            }

        }

        if (num_of_cards > 0) {

            #if defined(DEBUG) && defined(DEBUG_CALC_MISERE_SCORE)
                DEBUG_PRINT(F("3. Bottom up run of "));
                DEBUG_PRINT(num_of_cards);
                DEBUG_PRINT(F(" cards =  "));
                DEBUG_PRINTLN(20 + (num_of_cards * 5));
            #endif

            this->score = 20 + (num_of_cards * 5);
            return;
        }


        // Is it a decent bottom up run?
        // Add ranks - 4, 5             =  9 for 2 cards         (4 + 5)
        //             4, 6             = 10 for 2 cards         (4 + 6 + 9)
        //             4, Q             = 16 for 2 cards         (4 + 12)
        //             5, 8             = 13 for 2 cards         (5 + 6)
        //             4, A             = 18 for 2 cards         (4 + 14)
        //                                12 / 2 = 6
        //             4, 5, 7          = 16 for 3 cards         (4 + 5 + 7)
        //             4, 6, 9          = 19 for 3 cards         (4 + 6 + 9)
        //             5, 6, 9          = 20 for 3 cards         (5 + 6 + 9)
        //             5, 6, Q          = 23 for 3 cards         (5 + 6 + 12)
        //             4, K, A          = 31 for 3 cards         (4 + 13 + 14)
        //                                21 / 3 = 7
        //             4, 5, 7, 8       = 24 for 4 cards         (4 + 5 + 7 + 8)
        //             4, 6, 9, 10      = 29 for 4 cards         (4 + 6 + 9 + 10)
        //             5, 6, 9, Q       = 32 for 4 cards         (5 + 6 + 9 + 12)
        //             5, 6, Q, A       = 37 for 4 cards         (5 + 6 + 12 + 14)
        //                                28 / 4 = 7
        //             4, 5, 6, 7, 8    = 30 for 5 cards         (4 + 5 + 6 + 7 + 8)
        //             4, 6, 8, 9, 10   = 37 for 5 cards         (4 + 6 + 8 + 9 + 10)
        //             5, 6, 9, 10, Q   = 43 for 5 cards         (5 + 6 + 9 + 10 + 12)
        //             5, 6, 10, Q, A   = 46 for 5 cards         (5 + 6 + 10 + 12 + 14)
        //                                40 / 5 = 7
        
        num_of_cards = 0;
        
        for (uint8_t i = 0; i < 11; i++) {

            if (this->cards[i].getSuit() == Suit::None) break;

            this->rankScore = this->rankScore + static_cast<uint8_t>(this->cards[i].getRank());
            num_of_cards++;

            if (this->suit == Suit::Clubs || this->suit == Suit::Spades) {
                this->rankScore--;
            }

        }

        switch (num_of_cards) {
        
            case 1:

                switch (this->rankScore) { // Range 4 - 14

                    case 0 ... 5:
                        this->score = 20;
                        break;

                    case 6 ... 7:
                        this->score = 8;
                        break;

                    case 8 ... 9:
                        this->score = 5;
                        break;

                    default:
                        this->score = 0;
                        break;

                }

                break;
        
            case 2:

                switch (this->rankScore) { // Range 9 - 18

                    case 0 ... 10:
                        this->score = 30;
                        break;

                    case 11 ... 12:
                        this->score = 20;
                        break;

                    case 13 ... 14:
                        this->score = 10;
                        break;

                    default:
                        this->score = 0;
                        break;

                }

                break;


            case 3:

                switch (this->rankScore) { // Range 15 - 31

                    case 0 ... 18:
                        this->score = 40;
                        break;

                    case 19 ... 22:
                        this->score = 20;
                        break;

                    case 23 ... 26:
                        this->score = 10;
                        break;

                    default:
                        this->score = 0;
                        break;

                }

                break;

            case 4:

                switch (this->rankScore) { // Range 24 - 38

                    case 0 ... 26:
                        this->score = 50;
                        break;

                    case 27 ... 29:
                        this->score = 30;
                        break;

                    case 30 ... 32:
                        this->score = 10;
                        break;

                    default:
                        this->score = 0;
                        break;

                }

                break;

            case 5:

                switch (this->rankScore) { // Range 30 - 50

                    case 0 ... 34:
                        this->score = 60;
                        break;

                    case 35 ... 38:
                        this->score = 40;
                        break;

                    case 39 ... 42:
                        this->score = 10;
                        break;

                    default:
                        this->score = 0;
                        break;

                }

                break;

            default:

                switch (this->rankScore) { // Range 39 - 

                    case 0 ... 45:
                        this->score = 90;
                        break;

                    case 46 ... 55:
                        this->score = 80;
                        break;

                    case 56 ... 65:
                        this->score = 70;
                        break;

                    case 66 ... 75:
                        this->score = 60;
                        break;

                    default:
                        this->score = 0;
                        break;

                }

                break;

        }

        #if defined(DEBUG) && defined(DEBUG_CALC_MISERE_SCORE)
            DEBUG_PRINT(F("4. RankScore "));
            DEBUG_PRINT(this->rankScore);
            DEBUG_PRINT(F(", length "));
            DEBUG_PRINT(num_of_cards);
            DEBUG_PRINT(F(" = "));
            DEBUG_PRINT(this->score);
            DEBUG_PRINTLN();            
        #endif

    }

};