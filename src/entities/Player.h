#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "../utils/Utils.h"
#include "Card.h"
// #include "SuitPile.h"
#include "GameRound.h"

struct Player {

    public:

        GameRound *gameRound;
        Card cardJustPlayed;

    private:
    
        uint8_t playerNumber;
        uint8_t cardCount;
        uint8_t cardCount_Backup;
        Card cards[6];
        Card backup[6];
        bool playing = true;

    public:

        uint8_t getPlayerNumber()                       { return this->playerNumber; }
        uint8_t getCardCount()                          { return this->cardCount; }
        bool isPlaying()                                { return this->playing; }
        Card &getCard(uint8_t idx)                      { return this->cards[idx]; }
        Card &getCardJustPlayed()                       { return this->cardJustPlayed; }

        void setPlayerNumber(uint8_t val)               { this->playerNumber = val; }
        void setCardCount(uint8_t val)                  { this->cardCount = val; }
        void setPlaying(bool val)                       { this->playing = val; }
        void setCard(uint8_t idx, Card &card)           { this->cards[idx] = card; }

        void sortHand(int n, bool scoringHand, Suit suit, bool trumpsOnLeft = true) {

            uint8_t i, j;

            for (uint8_t i = 0; i < n - 1; i++) {
            
                for (j = 0; j < n-i-1; j++) {

                    if (scoringHand) {

                        if (trumpsOnLeft) {
                            if (this->cards[j].getRank() < this->cards[j+1].getRank()) {
                                this->swap(j, j + 1);
                            }
                        }
                        else {
                            if (this->cards[j].getRank() > this->cards[j+1].getRank()) {
                                this->swap(j, j + 1);
                            }
                        }

                    }
                    else {

                        if (this->cards[j].getCardIndex() > this->cards[j+1].getCardIndex()) {
                            this->swap(j, j + 1);
                        }

                    }

                }

            }

        }

        void backupHand() {

            for (uint8_t i = 0; i < 6; i++) {

                this->backup[i].init(this->cards[i].getSuit(), this->cards[i].getRank());

            }

            this->cardCount_Backup = this->cardCount;

        }

        void restoreHand() {

            for (uint8_t i = 0; i < 6; i++) {

                this->cards[i].init(this->backup[i].getSuit(), this->backup[i].getRank());

            }

            this->cardCount = this->cardCount_Backup;

        }

        void swap(uint8_t x, uint8_t y) {

            Rank tmpRank = this->cards[x].getRank();
            Suit tmpSuit = this->cards[x].getSuit();
            this->cards[x].init(this->cards[y].getSuit(), this->cards[y].getRank());
            this->cards[y].init(tmpSuit, tmpRank);

        }

        void reset() {
        
            for (uint8_t i = 0; i < 5; i++) {
            
                this->cards[i].reset();

            }

            this->cardCount = 0;
            this->playing = true;

        }

        uint8_t getSelectedCount() {
        
            uint8_t count = 0;

            for (uint8_t i = 0; i < this->cardCount; i++) {

                if (this->cards[i].isSelected()) count++;
            }

            return count;

        }



        uint16_t bid(Suit suit, Card &dealerCard, uint8_t isDealer) {

            this->backupHand();

            if (isDealer) {
                this->cards[5].init(dealerCard.getSuit(), dealerCard.getRank());
                this->cardCount++;
            }

            this->sortHand(this->cardCount, true, suit);


            #ifdef DEBUG_BID
                // this->print(false);
                if (isDealer) DEBUG_PRINT(" (D)");
                if (!isDealer) DEBUG_PRINT(" (d)");
                DEBUG_PRINT(" >> ");
            #endif

            uint8_t handScore = 0;
            bool hasRightBower = false;
            bool hasLeftBower = false;


            // Set suit ..

            for (uint8_t i = 0; i < this->cardCount; i++) {
                this->cards[i].setTrumps(dealerCard.getSuit());
            }


            // Work out score ..

            for (uint8_t i = 0; i < this->cardCount; i++) {

                bool matchingSuit = 0;
                Card &card = this->cards[i];

                #ifdef DEBUG_BID
                DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                DEBUG_PRINT(" ");
                #endif

                if (card.getSuit() == suit) { matchingSuit = true; }

                switch (card.getRank()) {

                    case Rank::Right_Bower:
                        handScore = handScore + 100;
                        hasRightBower = true;

                        #ifdef DEBUG_BID
                        DEBUG_PRINT("100 Right Bower, ");
                        #endif

                        break;

                    case Rank::Left_Bower:
                        handScore = handScore + (hasRightBower ? 100 : 80);
                        hasLeftBower = true;

                        #ifdef DEBUG_BID
                        if (hasRightBower) {
                            DEBUG_PRINT("100 Left Bower (inc RB), ");
                        }
                        else {
                            DEBUG_PRINT("80 Left Bower only, ");
                        }
                        #endif

                        break;

                    default:

                        switch (card.getRank()) {
                                
                            case Rank::Ace:    

                                if (matchingSuit) {

                                    if (hasRightBower && hasLeftBower) {
                                        handScore = handScore + 100;
                                        #ifdef DEBUG_BID
                                        DEBUG_PRINT("100 Trumps (inc RB, LB), ");
                                        #endif                                        
                                    }
                                    else if (hasRightBower && !hasLeftBower) {
                                        handScore = handScore + 70;
                                        #ifdef DEBUG_BID
                                        DEBUG_PRINT("70 Trumps (inc RB), ");
                                        #endif                                        
                                    }
                                    else if (!hasRightBower && hasLeftBower) {
                                        handScore = handScore + 60;
                                        #ifdef DEBUG_BID
                                        DEBUG_PRINT("60 Trumps (inc LB), ");
                                        #endif                                        
                                    }

                                }
                                else {

                                    handScore = handScore + 40;
                                    #ifdef DEBUG_BID
                                    DEBUG_PRINT("40 Non-Trump, ");
                                    #endif                                        

                                }

                                break;

                            case Rank::King:

                                if (matchingSuit) {

                                    switch (i) {

                                        case 0:
                                            handScore = handScore + 20;
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("20 Trump, ");
                                            #endif
                                            break;

                                        case 1:

                                            switch (this->cards[i - 1].getRank()) {
                                                
                                                case Rank::Right_Bower:
                                                    handScore = handScore + 30;
                                                    #ifdef DEBUG_BID
                                                    DEBUG_PRINT("30 Trump (inc RB), ");
                                                    #endif                                                    
                                                    break;
                                                
                                                case Rank::Left_Bower:
                                                    handScore = handScore + 25;
                                                    #ifdef DEBUG_BID
                                                    DEBUG_PRINT("25 Trump (inc LB), ");
                                                    #endif                                                    
                                                    break;
                                                
                                                case Rank::Ace:
                                                    handScore = handScore + 20;
                                                    #ifdef DEBUG_BID
                                                    DEBUG_PRINT("20 Trump (inc Ace), ");
                                                    #endif                                                    
                                                    break;

                                            }

                                            break;

                                        case 2:

                                            switch (this->cards[i - 1].getRank()) {
                                                
                                                case Rank::Left_Bower:
                                                    handScore = handScore + 40;
                                                    #ifdef DEBUG_BID
                                                    DEBUG_PRINT("40 Trump (inc RB, LB), ");
                                                    #endif                                                    
                                                    break;
                                                
                                                case Rank::Ace:
                                                    handScore = handScore + 30;
                                                    #ifdef DEBUG_BID
                                                    DEBUG_PRINT("30 Trump (inc Ace), ");
                                                    #endif                                                    
                                                    break;

                                            }

                                            break;

                                        case 3:
                                            handScore = handScore + 100; 
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("100 Trumps (inc RB, LB, A), ");
                                            #endif                                                    
                                            break;


                                    }

                                }
                                else {

                                    switch (i) {

                                        case 0:
                                            handScore = handScore + 20;
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("20 Non-Trumps (no-Ace), ");
                                            #endif                                                    
                                            break;

                                        default:

                                            if (this->cards[i - 1].getSuit() == card.getSuit()) {
                                                handScore = handScore + 30;
                                                #ifdef DEBUG_BID
                                                DEBUG_PRINT("30 Non-Trumps (inc Ace), ");
                                                #endif                                                    
                                            }
                                            else {
                                                handScore = handScore + 20;
                                                #ifdef DEBUG_BID
                                                DEBUG_PRINT("20 Non-Trumps (no-Ace), ");
                                                #endif                                                    
                                            }

                                    }

                                }

                                break;

                            case Rank::Queen:

                                if (matchingSuit) {

                                    switch (i) {

                                        case 0:
                                            handScore = handScore + 10; 
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("10 Trumps, ");
                                            #endif   
                                            break;

                                        case 1:
                                            handScore = handScore + 15; 
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("15 Trumps, ");
                                            #endif 
                                            break;

                                        case 2:
                                            handScore = handScore + 30; 
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("30 Trumps, ");
                                            #endif                                             
                                            break;

                                        case 3:
                                            handScore = handScore + 45; 
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("45 Trumps, ");
                                            #endif 
                                            break;

                                        case 4:
                                            handScore = handScore + 100; 
                                            #ifdef DEBUG_BID
                                            DEBUG_PRINT("100 Trumps (inc RB, LB, A), ");
                                            #endif 
                                            break;

                                    }

                                }
                                break;

                            case Rank::None ... Rank::Six:

                                if (matchingSuit) {

                                    handScore = handScore + 5;
                                    #ifdef DEBUG_BID
                                    DEBUG_PRINT("15 Trump, ");
                                    #endif 
                                
                                }

                                break;

                            case Rank::Seven ... Rank::Ten:

                                if (matchingSuit) {

                                    handScore = handScore + 10;
                                    #ifdef DEBUG_BID
                                    DEBUG_PRINT("10 Trump, ");
                                    #endif                                     
                                
                                }

                                break;

                        }

                        break;

                }

            }

            this->restoreHand();

            #ifdef DEBUG_BID
            DEBUG_PRINT(" = ");
            DEBUG_PRINTLN(handScore);
            #endif                                     

            return handScore;

        }

        #include "Player_PlayCard.hpp"
        #include "Player_Utils.hpp"
        #include "Player_HandleKitty.hpp"
        #include "Player_PlaySuit.hpp"

};
