
#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Bid.h"
#include "Card.h"

void handleKitty() {

    uint8_t numberDiscarded = 0;    
    Suit trumps = this->gameRound->winningBid_Suit();
    uint8_t dealer = gameRound->getDealer_Idx();
    // Bid winningBid = this->gameRound->getBid(this->getPlayerNumber());


    // Have either oponent bidded ?  Get the suit ..

    Suit startingSuit = this->gameRound->getBid((this->getPlayerNumber() - 1 + 4) % 4).getSuit();

    // if (startingSuit == Suit::None) {
    //     startingSuit = this->gameRound->getBid((this->getPlayerNumber() + 1) % 4).getSuit();
    // }

    // if (startingSuit == Suit::None) {
    //     startingSuit = Suit::Spades;
    // }    


    #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
        DEBUG_PRINT(F("Handle Kitty - "));
        DEBUG_PRINTLN(dealer);
        DEBUG_PRINTLN(F("Short suit?"));
    #endif

    for (uint8_t s = static_cast<uint8_t>(trumps) + 1; s < static_cast<uint8_t>(trumps) + 4; s++) {             // Can we short suit ourselves?

        Suit suit = static_cast<Suit>(s % 4);
    
        #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
            DEBUG_PRINT(F("- suit "));
            DEBUG_PRINT_SUIT(suit);
        #endif

        uint8_t countOfSuit = this->getNumberOfCards_InSuit(suit);
        uint8_t scoreOfSuit = this->getScoreOfCards_InSuit(suit, BidType::Partner);       

        #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
            DEBUG_PRINT(F(" count "));
            DEBUG_PRINT(countOfSuit);
            DEBUG_PRINT(F(", score "));
            DEBUG_PRINT(scoreOfSuit);
        #endif

        switch (countOfSuit) {

            case 1:
                if (scoreOfSuit <= 12) {
                    
                    uint8_t cardsDiscarded = this->discardAll_InSuit(trumps, suit);
                    numberDiscarded = 1;

                    #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
                        DEBUG_PRINT(F(" = discard "));
                        DEBUG_PRINT(cardsDiscarded);
                        DEBUG_PRINT(F(" card."));
                    #endif

                }
                break;
                
        }
    
        #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
            DEBUG_PRINTLN();
        #endif

        if (numberDiscarded == 1) return;

    }


    // If we have not unloaded a card, discard the lowest cards of the shortest suits ..

    #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
        DEBUG_PRINT(F("Lowest of short suits?"));
    #endif

    if (numberDiscarded < 1) {
        
        for (uint8_t i = 1; i < 10; i++) {
                                
            for (Rank rank = Rank::Nine; rank < Rank::TopCard; rank++) {
                
                for (uint8_t s = static_cast<uint8_t>(startingSuit); s < static_cast<uint8_t>(startingSuit) + 4; s++) {

                    Suit suit = static_cast<Suit>(s % 4);

                    if (suit == trumps)                         continue;
                    if (suit != trumps && rank > Rank::Ace)     continue;

                    if (numberDiscarded == 1) break;

                    uint8_t countOfSuit = this->getNumberOfCards_InSuit(suit);
                    uint8_t idx = this->getLowest_InSuit(suit);

                    if (idx != Constants::No_Card) {

                        Card &lowestCard = this->cards[idx];

                        if (countOfSuit == i && lowestCard.getRank() == rank) {

                            if (static_cast<uint8_t>(lowestCard.getRank()) + countOfSuit == 15) {

                                #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
                                    DEBUG_PRINT(F("\n- "));
                                    DEBUG_PRINT_CARD(lowestCard.getSuit(), lowestCard.getRank());
                                    DEBUG_PRINT(F(" skipped."));
                                #endif
                            
                            }
                            else {

                                #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
                                    DEBUG_PRINT(F("\n- "));
                                    DEBUG_PRINT_CARD(lowestCard.getSuit(), lowestCard.getRank());
                                    DEBUG_PRINT_SPACE();
                                #endif

                                this->discardCard(lowestCard);
                                numberDiscarded = numberDiscarded + 1;

                            }

                        }            

                    }

                }                                         
                                        
                if (numberDiscarded == 1) {

                    #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
                        DEBUG_PRINTLN();
                    #endif

                    return;

                }

            }

        }

    }

    this->sort();

    #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
        DEBUG_PRINTLN();
    #endif

        
    // Double check we have a card ..
        
    #if defined(DEBUG) && defined(DEBUG_HANDLEKITTY)
        DEBUG_PRINT(F("Dicard last "));
        DEBUG_PRINT(3 - numberDiscarded);
        DEBUG_PRINTLN();
    #endif

    uint8_t idx = this->getLowest_AllSuit();

    if (idx != Constants::No_Card) {
    
        Card &card = this->cards[idx];

        this->discardCard(card);
        numberDiscarded = numberDiscarded + 1;

    }
    
    this->sort();


} 
 