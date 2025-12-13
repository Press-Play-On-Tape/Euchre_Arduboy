
#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Card.h"
#include "SuitPile.h"
#include "GameRound.h"



// Trump cards -----------------------------------------------------------------


// bool playJoker(Suit suit, bool isHuman) {

//     for (uint8_t i = 0; i < this->cardCount; i++) {
    
//         Card &card = this->cards[i];

//     }

//     return false;

// }

bool isCardInTrick(Suit suit, Rank rank) {

    for (uint8_t i = 0; i < 4; i++) {
    
        Card *card = this->gameRound->getHand(i);

        if (card->getSuit() == suit && card->getRank() == rank) {
        
            return true;

        }

    }

    return false;

}


// Play the lowest trump in the player's hand that is below the specified rank ..

bool playLowest_LowerThan_InSuit(Suit trumps, Rank rank, bool isHuman) {

    uint8_t idx = this->getLowest_InSuit(trumps, rank);

    if (idx == Constants::No_Card)  {
    
        return false;

    }
    else {

        this->playCard(idx, isHuman);
        return true;

    }

}


// Play the lowest card in the specified suit ..

bool playLowest_InSuit(Suit suitToFollow, bool isHuman) {

    uint8_t idx = this->getLowest_InSuit(suitToFollow);

    if (idx == Constants::No_Card)  {
    
        return false;

    }
    else {

        this->playCard(idx, isHuman);
        return true;

    }

}


// Play the highest card in the specified suit ..

bool playHighest_InSuit(Suit suitToFollow, bool isHuman) {

    uint8_t idx = this->getHighest_InSuit(suitToFollow);

    if (idx == Constants::No_Card)  {
    
        return false;

    }
    else {

        this->playCard(idx, isHuman);
        return true;

    }

}


// Play the highest card in the any suit greater than the specified rank ..

bool playHighest_LargerThan_AllSuit(Rank rank, bool isHuman) {
    
    return playHighest_LargerThan_AllSuit(Suit::None, rank, isHuman);

}


// Play the highest card in the any non trump suit greater than the specified rank ..

bool playHighest_LargerThan_AllSuit(Suit excludeTrumps, Rank rank, bool isHuman) {

    uint8_t returnIdx = Constants::No_Card;

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {
    
        if (suit != excludeTrumps) {

            uint8_t idx = this->getHighest_InSuit(suit);

            if (idx != Constants::No_Card)  {

                Card &card = this->cards[idx];

                if (card.getRank() > rank) {
                    
                    returnIdx = idx;
                    rank = card.getRank();

                }

            }

        }

    }


    // Play the best card we found ..

    if (returnIdx != Constants::No_Card)  {

        this->playCard(returnIdx, isHuman);
        return true;

    }

    return false;

}


// Play the lowest card in the any non trump suit ..

bool playLowest_AllSuit(bool isHuman) {

    return playLowest_AllSuit(Suit::None, isHuman);

}


// Play the lowest card in the any non trump suit ..

bool playLowest_AllSuit(Suit excludeSuit, bool isHuman) {

    Rank rank = Rank::TopCard;
    uint8_t returnIdx = Constants::No_Card;

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {
    
        if (suit != excludeSuit) {

            uint8_t idx = this->getLowest_InSuit(suit);

            if (idx != Constants::No_Card)  {

                Card &card = this->cards[idx];

                if (card.getRank() < rank) {
                   
                    returnIdx = idx;
                    rank = card.getRank();

                }

            }

        }

    }


    // Play the best card we found ..

    if (returnIdx != Constants::No_Card)  {

        this->playCard(returnIdx, isHuman);
        return true;

    }

    return false;

}


bool playTop_InSuit(Suit trumps, Suit suit, bool isHuman) { 

    uint8_t idx = this->getTop_InSuit(trumps, suit);

    if (idx != Constants::No_Card)  {

        this->playCard(idx, isHuman);
        return true;

    }

    return false;

}


bool playTop_AllSuit(Suit trumps, bool isHuman) { 

    return playTop_AllSuit(trumps, Suit::None, isHuman);

}

bool playTop_AllSuit(Suit trumps, Suit excludeSuit, bool isHuman) { 

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {
    
        if (suit != excludeSuit) {

            uint8_t idx = this->getTop_InSuit(trumps, suit);

            if (idx != Constants::No_Card)  {

                this->playCard(idx, isHuman);
                return true;

            }

        }

    }

    return false;

}


bool playBottom_AllSuit(Suit trumps, bool isHuman) { 

    return playBottom_AllSuit(trumps, Suit::None, isHuman);

}

bool playBottom_AllSuit(Suit trumps, Suit excludeSuit, bool isHuman) { 

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {
    
        if (suit != excludeSuit) {

            uint8_t idx = this->getBottom_InSuit(trumps, suit);

            if (idx != Constants::No_Card)  {

                this->playCard(idx, isHuman);
                return true;

            }

        }

    }

    return false;

}


bool playBottom_InSuit(Suit trumps, Suit suit, bool isHuman) { 

    uint8_t idx = this->getBottom_InSuit(trumps, suit);

    if (idx != Constants::No_Card)  {

        this->playCard(idx, isHuman);
        return true;

    }

    return false;

}

bool playLowest_WithSecondHighest_InSuit(Suit trumps, Suit suitToFollow, bool isHuman) {


    // What is the second highest unplayed card in this suit?

    Rank highestUnplayedCard = Rank::None;
    Rank topRank = Rank::Ace;

    for (Rank rank = Rank::Four; rank <= topRank; rank++) {

        if (trumps == suitToFollow && rank == Rank::Jack) continue;

        if (!this->gameRound->hasCardBeenPlayed(trumps, suitToFollow, rank)) {

            highestUnplayedCard = rank;        

        } 

    }


    // If the lowest card is a Red four or Black five then the highest card is also the lowest ..

    if ((suitToFollow == Suit::Diamonds || suitToFollow == Suit::Hearts) && highestUnplayedCard == Rank::Four) return false;
    if ((suitToFollow == Suit::Clubs    || suitToFollow == Suit::Spades) && highestUnplayedCard == Rank::Five) return false;


    // So are we holding the top card?

    highestUnplayedCard--;
    uint8_t haveSecondLowestCard = this->getTop_InSuit(trumps, suitToFollow, highestUnplayedCard);

    if (haveSecondLowestCard != Constants::No_Card) {

        uint8_t lowCard = this->getLowest_InSuit(suitToFollow, highestUnplayedCard);

        if (lowCard != Constants::No_Card) {

            this->playCard(lowCard, isHuman);
            return true;

        }

    }

    return false;

}



bool playLowest_WithSecondHighest_AllSuit(Suit trumps, Suit excludeSuit, bool isHuman) { 

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {
    
        if (suit != excludeSuit) {

            bool cardPlayed = this->playLowest_WithSecondHighest_InSuit(trumps, suit, isHuman);

            if (cardPlayed)  {

                return true;

            }

        }

    }

    return false;

}


// Play the lowest card in the any non trump suit ..

bool playLowest_ExcludeSuit(Suit excludeTrumps, Suit excludeSuit1, Suit excludeSuit2, bool isHuman) {

    Rank rank = Rank::TopCard;
    uint8_t returnIdx = Constants::No_Card;

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {
    
        if (suit != excludeTrumps && suit != excludeSuit1 && suit != excludeSuit2) {

            uint8_t idx = this->getLowest_InSuit(suit);

            if (idx != Constants::No_Card)  {

                Card &card = this->cards[idx];

                if (card.getRank() < rank) {
                    
                    returnIdx = idx;
                    rank = card.getRank();

                }

            }

        }

    }


    // Play the best card we found ..

    if (returnIdx != Constants::No_Card)  {

        this->playCard(returnIdx, isHuman);
        return true;

    }

    return false;

}

bool playHighest_LargerThan_InSuit(Suit suitToFollow, Rank rank, bool isHuman) {

    uint8_t idx = this->getHighest_InSuit(suitToFollow, rank);

    if (idx != Constants::No_Card)  {

        this->playCard(idx, isHuman);
        return true;

    }

    return false;
}


bool playNextHighest_LargerThan_InSuit(Suit suitToFollow, Rank rank, bool isHuman) {

    uint8_t idx = this->getNextHighest_InSuit(suitToFollow, rank);

    if (idx != Constants::No_Card)  {

        this->playCard(idx, isHuman);
        return true;

    }

    return false;
}


bool playNextLowest_SmallerThan_InSuit(Suit suitToFollow, Rank rank, bool isHuman) {

    uint8_t idx = this->getNextLowest_InSuit(suitToFollow, rank);

    if (idx != Constants::No_Card)  {

        this->playCard(idx, isHuman);
        return true;

    }

    return false;
}