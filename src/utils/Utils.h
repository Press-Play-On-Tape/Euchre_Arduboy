#pragma once

#include "Constants.h"
#include "Enums.h"

void DEBUG_PRINT_SUIT(Suit suit) {

    #ifdef DEBUG

        switch (suit) {

            case Suit::Spades:
                DEBUG_PRINT(F("S"));
                break;

            case Suit::Clubs:
                DEBUG_PRINT(F("C"));
                break;

            case Suit::Diamonds:
                DEBUG_PRINT(F("D"));
                break;

            case Suit::Hearts:
                DEBUG_PRINT(F("H"));
                break;
                
            case Suit::None:
                DEBUG_PRINT(F("_"));
                break;
                
            case Suit::No_Trumps:
                DEBUG_PRINT(F("!"));
                break;
        }

    #endif
    
}


void DEBUG_PRINT_RANK(Rank rank) {

    #ifdef DEBUG

        switch (rank) {

            case Rank::None ... Rank::Ten:
                DEBUG_PRINT(static_cast<uint8_t>(rank));
                break;

            case Rank::Jack:
                DEBUG_PRINT(F("J"));
                break;

            case Rank::Queen:
                DEBUG_PRINT(F("Q"));
                break;
                
            case Rank::King:
                DEBUG_PRINT(F("K"));
                break;
                
            case Rank::Ace:
                DEBUG_PRINT(F("A"));
                break;
                
            case Rank::Left_Bower:
                DEBUG_PRINT(F("L"));
                break;
                
            case Rank::Right_Bower:
                DEBUG_PRINT(F("R"));
                break;
                
        }

    #endif
    
}


void DEBUG_PRINT_CARD(Suit suit, Rank rank) {

    #ifdef DEBUG

        DEBUG_PRINT_RANK(rank);
        DEBUG_PRINT_SUIT(suit);

    #endif

}


void DEBUG_PRINT_SPACE() {

    #ifdef DEBUG

        DEBUG_PRINT(F(" "));

    #endif

}

void DEBUG_PRINT_TRISTATE(TriState tristate) {

    #ifdef DEBUG

        switch (tristate) {
        
            case TriState::True:
                DEBUG_PRINT(F("T"));
                break;
        
            case TriState::False:
                DEBUG_PRINT(F("F"));
                break;
        
            case TriState::Maybe:
                DEBUG_PRINT(F("M"));
                break;
                
        }

    #endif

}


void DEBUG_PRINT_BID(BidType bidType, Suit suit) {

    #ifdef DEBUG
        
        switch (bidType) {
        
            case BidType::Suit:
                DEBUG_PRINT_SUIT(suit);
                break;
        
            case BidType::Pass:
                DEBUG_PRINT(F("Pass"));
                break;
        
            case BidType::None:
                DEBUG_PRINT(F("None"));
                break;

        }

    #endif

}

Suit getTrump_AltSuit(Suit trumps) {

    if (trumps == Suit::Spades)    return Suit::Clubs;
    if (trumps == Suit::Clubs)     return Suit::Spades;
    if (trumps == Suit::Diamonds)  return Suit::Hearts;
    if (trumps == Suit::Hearts)    return Suit::Diamonds;

    return Suit::None;

}


