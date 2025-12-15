
#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "../utils/Utils.h"
#include "Card.h"
// #include "SuitPile.h"
#include "GameRound.h"


void clearSelection() {

    for (uint8_t i = 0; i < this->cardCount; i++) {

        Card &card = this->cards[i];
        card.setSelected(false);

    }
    
}


uint8_t getCountOfSuit(Suit suit) {

    uint8_t count = 0;

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        if (card.getSuit() == suit) {
        
            count++;
            
        }

    }

    return count;

}


Suit canShortSuit() {

    for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {

        uint8_t count = this->getCountOfSuit(suit);

        if (count == 1) {

            return suit;

        }

    }

    return Suit::None;

}

void playCard(uint8_t idx, bool isHumanPlayer) {

    Card &card = this->cards[idx];
    copyCard(card, this->cardJustPlayed);

    if (!isHumanPlayer) {

        this->gameRound->markCardPlayed(card.getOrigSuit(), card.getOrigRank());
        copyCard(card, this->gameRound->getHand(this->gameRound->getCurrentPlayer_Idx()));


        card.reset();
        this->sort();

    }

}

bool hasTrumps(Suit trumps) {

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        if (card.getSuit() == trumps || card.isLeftBower(trumps)) {
        
            return true;
            
        }

    }

    return false;

}


bool hasSuit(Suit suit) {

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        if (card.getSuit() == suit) return true;

    }

    return false;

}


// Are we hold the nominated card?

uint8_t getCard(Rank rank) {

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        if ((rank == Rank::TopCard && card.getRank() == Rank::TopCard) || card.getRank() == rank) {
        
            return i;

        }

    }

    return Constants::No_Card;

}


// Are we hold the nominated card?

uint8_t getCard(Suit suit, Rank rank) {

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        if ((rank == Rank::TopCard && card.getRank() == Rank::TopCard) || (card.getSuit() == suit && card.getRank() == rank)) {
        
            return i;

        }

    }

    return Constants::No_Card;

}


// Return the next highest trump in the player's hand from specified rank ..

uint8_t getNextHighestTrump(Suit trumps, Rank specifiedRank) {

    Rank foundRank = Rank::None;
    uint8_t foundIdx = Constants::No_Card;
    Card specifiedCard;

    specifiedCard.setRank(specifiedRank);
    specifiedCard.setSuit(trumps);

    for (uint8_t i = 0; i < this->cardCount; i++) {

        Card &card = this->cards[i];

        if (card.getSuit() == trumps) {

            #if defined(DEBUG) && defined(DEBUG_GETNEXTHIGHESTTRUMP)
                DEBUG_PRINT(F("Found card "));
                DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                DEBUG_PRINTLN();
            #endif

            if (card.getRank() > specifiedCard.getRank()) {

                if (foundRank == Rank::None) {
                
                    foundRank = card.getRank();
                    foundIdx = i;

                }
                else if (card.getRank() < foundRank) {

                    foundRank = card.getRank();
                    foundIdx = i;

                }

            }                

        }

    }

    return foundIdx;

}




// Non trump cards ------------------------------------------------------------------------------------


// Get the lowest card in the any suit ..

uint8_t getLowest_AllSuit() {

    return getLowest_AllSuit(Suit::None);

}


// Get the lowest card in the any non trump suit ..

uint8_t getLowest_AllSuit(Suit excludeSuit) {

    uint8_t returnIdx = Constants::No_Card;
    Rank rank = Rank::TopCard;

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

    return returnIdx;

}

// Get the lowest non-trump card in suit ..

uint8_t getLowest_InSuit(Suit suit) {

    return this->getLowest_InSuit(suit, Rank::TopCard);

}


// Get the lowest non-trump card in suit which is lower than the nominated rank..

uint8_t getLowest_InSuit(Suit suit, Rank rank) {

    uint8_t idx = Constants::No_Card;

    for (int j = 0; j < this->cardCount; j++) {

        Card &card = cards[j];

        if (card.getSuit() == suit && card.getRank() < rank) {
            idx = j;
            rank = this->cards[j].getRank();
        }

    }

    return idx;

}


// Get the highest card in suit ..

uint8_t getHighest_InSuit(Suit suit) {

    return this->getHighest_InSuit(suit, Rank::None);

}


// Get the highest card in suit which is higher than the nominated rank..

uint8_t getHighest_InSuit(Suit suit, Rank rank) {

    uint8_t idx = Constants::No_Card;

    for (int j = 0; j < this->cardCount; j++) {

        if (cards[j].getSuit() == suit && this->cards[j].getRank() > rank) {
            idx = j;
            rank = this->cards[j].getRank();
        }

    }

    return idx;

}


// Get the highest card in the any suit ..

uint8_t getHighest_AllSuit() {

    return getHighest_AllSuit(Suit::None);

}


// Get the highest card in the any suit ..

uint8_t getHighest_AllSuit(Suit excludeTrumps) {

    uint8_t returnIdx = Constants::No_Card;
    Rank rank = Rank::None;

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

    return returnIdx;

}


// Get the next highest non-trump card in suit which is higher than the nominated rank..

uint8_t getNextHighest_InSuit(Suit suit, Rank specifiedRank) {

    Rank foundRank = Rank::TopCard;
    uint8_t foundIdx = Constants::No_Card;

    #if defined(DEBUG) && defined(DEBUG_GETNEXTHIGHESTCARD_NOTRUMP_INSUIT)
        DEBUG_PRINT(F("getNextHighestCard_InSuit("));
        DEBUG_PRINT_SUIT(suit);
        DEBUG_PRINT(F(","));
        DEBUG_PRINT_RANK(specifiedRank);
        DEBUG_PRINT(F(") - "));
    #endif

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        #if defined(DEBUG) && defined(DEBUG_GETNEXTHIGHESTCARD_NOTRUMP_INSUIT)
            DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
            DEBUG_PRINT_SPACE();
        #endif


        if (card.getSuit() == suit) {

            if (card.getRank() > specifiedRank && card.getRank() < foundRank) {
            
                foundRank = card.getRank();
                foundIdx = i;

            } 

        }

    }


    if (foundIdx != Constants::No_Card) {

        Card &card = this->cards[foundIdx];

        #if defined(DEBUG) && defined(DEBUG_GETNEXTHIGHESTCARD_INSUIT)
            DEBUG_PRINT(F(" = "));
            DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
            DEBUG_PRINTLN();
        #endif

    }
    else {

        #if defined(DEBUG) && defined(DEBUG_GETNEXTHIGHESTCARD_INSUIT)
            DEBUG_PRINTLN(F(" = No_Card."));
        #endif
    
    }

    return foundIdx;

}


// Do we have the top unplayed card in the nominated suit ..

uint8_t getTop_InSuit(Suit trumps, Suit suitToFollow) {

    #if defined(DEBUG) && defined(DEBUG_GETTOP_INSUIT)
        DEBUG_PRINT(F("getTop "));
        DEBUG_PRINT_SUIT(suitToFollow);
        DEBUG_PRINT(F(": "));
    #endif
    

    // What is the players highest trump?

    uint8_t idx = this->getHighest_InSuit(suitToFollow);

    if (idx == Constants::No_Card)  {

        #if defined(DEBUG) && defined(DEBUG_GETTOP_INSUIT)
            DEBUG_PRINTLN(F("player has none."));
        #endif

        return Constants::No_Card;

    }


    Card &playersHighestCard = this->cards[idx];

    #if defined(DEBUG) && defined(DEBUG_GETTOP_INSUIT)

        DEBUG_PRINT(F("player holds "));
        DEBUG_PRINT_CARD(playersHighestCard.getSuit(), playersHighestCard.getRank());
        DEBUG_PRINT(F(": "));

    #endif


    // What is the highest unplayed card in this suit?

    Rank highestUnplayedCard = playersHighestCard.getRank();

    Rank topRank = (trumps == suitToFollow ? Rank::Right_Bower : Rank::Ace);

    for (Rank rank = topRank; rank >= playersHighestCard.getRank(); rank--) {

        if (trumps == suitToFollow && rank == Rank::Jack) continue;

        if (!this->gameRound->hasCardBeenPlayed(trumps, suitToFollow, rank)) {

            #if defined(DEBUG) && defined(DEBUG_GETTOP_INSUIT)

                DEBUG_PRINT_CARD(suitToFollow, rank);
                DEBUG_PRINT(F(" not played. "));

            #endif

            highestUnplayedCard = rank; 
            break;       

        } 
        else {

            #if defined(DEBUG) && defined(DEBUG_GETTOP_INSUIT)

                DEBUG_PRINT_CARD(suitToFollow, rank);
                DEBUG_PRINT(F(" played. "));

            #endif

        }

    }


    // So are we holding the top card?

    #if defined(DEBUG) && defined(DEBUG_GETTOP_INSUIT)
        DEBUG_PRINT(F("Highest unplayed "));
        DEBUG_PRINT_CARD(suitToFollow, highestUnplayedCard);
        DEBUG_PRINTLN(F("."));
    #endif

    if (highestUnplayedCard == playersHighestCard.getRank()) {

        return idx;

    }

    return Constants::No_Card;

}


// Do we have the top unplayed card in the nominated suit below the nominated rank ..

uint8_t getTop_InSuit(Suit trumps, Suit suitToFollow, Rank topRank) {


    // What is the players highest card in the specified suit?

    uint8_t idx = this->getHighest_InSuit(suitToFollow);
    if (idx == Constants::No_Card)  return Constants::No_Card;

    Card &playersHighestCard = this->cards[idx];


    // What is the highest unplayed card in this suit?

    Rank highestUnplayedCard = Rank::None;

    for (Rank rank = Rank::Nine; rank <= topRank; rank++) {

        if (trumps == suitToFollow && rank == Rank::Jack) continue;

        if (!this->gameRound->hasCardBeenPlayed(trumps, suitToFollow, rank)) {

            highestUnplayedCard = rank;        

        } 

    }

    // So are we holding the top card?

    if (highestUnplayedCard == playersHighestCard.getRank()) {

        return idx;

    }

    return Constants::No_Card;

}


// Get the count of card in hand of a given suit..

uint8_t getNumberOfCards_InSuit(Suit suit) {

    return this->getNumberOfCards_InSuit(suit, Rank::None);

}


// Get the count of card in hand of a given suit higher than LowestRank..

uint8_t getNumberOfCards_InSuit(Suit suit, Rank lowestRank) {

    uint8_t numberOfCards = 0;

    for (int j = 0; j < this->cardCount; j++) {

        Card &card = cards[j];

        if (card.getSuit() == suit && card.getRank() >= lowestRank) {
            numberOfCards++;
        }

    }

    return numberOfCards;

}




// Get the basic score of card in hand of a given suit..

uint8_t getScoreOfCards_InSuit(Suit suit, BidType bidType) {

    return this->getScoreOfCards_InSuit(suit, Rank::None, bidType);

}


// Get the basic score of card in hand of a given suit..

uint8_t getScoreOfCards_InSuit(Suit suit, Rank lowestRank, BidType bidType) {

    uint8_t scoreOfCards = 0;

    for (int j = 0; j < this->cardCount; j++) {

        Card &card = cards[j];

        if (card.getSuit() == suit && card.getRank() >= lowestRank) {

            switch (bidType) {
            
                case BidType::Partner:

                    scoreOfCards = scoreOfCards + Constants::ScoreType_Basic[static_cast<uint8_t>(card.getRank())];
                    break;
                        
            }

        }

    }

    return scoreOfCards;

}


uint8_t discardAll_InSuit(Suit trumps, Suit suit) {

    return this->discardAll_InSuit(trumps, suit, Rank::None);

}


// Discard all cards in hand with a rank of 'lowestRank' or above ..

uint8_t discardAll_InSuit(Suit trumps, Suit suit, Rank lowestRank) {

    uint8_t cardsDiscarded = 0;

    #if defined(DEBUG) && defined(DEBUG_DISCARDALL_INSUIT)
        DEBUG_PRINT(F("discardAll_InSuit("));
        DEBUG_PRINT_SUIT(suit);
        DEBUG_PRINT(F(","));
        DEBUG_PRINT_RANK(lowestRank);
        DEBUG_PRINTLN(F(")"));
    #endif

    Rank topRank = (trumps == suit ? Rank::TopCard : Rank::Ace);

    for (Rank rank = lowestRank; rank <= topRank; rank++) {

        if (trumps == suit && rank == Rank::Jack) continue;

        for (int j = 0; j < this->cardCount; j++) {

            Card *card = &cards[j];

            if (card->getSuit() == suit && card->getRank() == rank) {
                
                this->cardCount--;
                cardsDiscarded++;
                this->gameRound->addKitty(card);

                #if defined(DEBUG) && defined(DEBUG_DISCARDALL_INSUIT)
                    DEBUG_PRINT(F(" discard "));
                    DEBUG_PRINT_CARD(card->getSuit(), card->getRank());
                    DEBUG_PRINT_SPACE();
                #endif

                card->reset();
                this->sort();

            }

            if (this->cardCount == 10) break;

        }

        if (this->cardCount == 10) break;

    }

    this->sort();

    return cardsDiscarded;

}


void discardCard(Card &discardCard) {

    uint8_t cardsDiscarded = 0;

    for (int j = 0; j < this->cardCount; j++) {

        Card *card = &cards[j];

        if (card->getSuit() == discardCard.getSuit() && card->getRank() == discardCard.getRank()) {
            
            this->cardCount--;
            cardsDiscarded++;
            this->gameRound->addKitty(card);

            #if defined(DEBUG) && defined(DEBUG_DISCARDCARD)
                DEBUG_PRINT(F(" discard "));
                DEBUG_PRINT_CARD(card->getSuit(), card->getRank());
                DEBUG_PRINT_SPACE();
            #endif

            card->reset();
            break;

        }

    }

    this->sort();

    return cardsDiscarded;

}


void sort() {

    BidType bidType = BidType::Pass;
    Suit trumps = this->gameRound->winningBid_Suit();

    if (this->gameRound->getWinningBid_Idx() != Constants::WinningBid_None) {
        bidType = this->gameRound->getWinningBid().getBidType();
    }


    // What sequence do we want?

    uint8_t seq[] = {0, 2, 1, 3 };

    bool spades     = this->hasSuit(Suit::Spades);
    bool clubs      = this->hasSuit(Suit::Clubs);
    bool diamonds   = this->hasSuit(Suit::Diamonds);
    bool hearts     = this->hasSuit(Suit::Hearts);


    if (spades && clubs && diamonds && hearts) {
        if (trumps == Suit::Spades)                             { seq[0] = 3; seq[1] = 1; seq[2]= 0; seq[3] = 2; }
        if (trumps == Suit::Clubs)                              { seq[0] = 1; seq[1] = 3; seq[2]= 0; seq[3] = 2; }
        if (trumps == Suit::Diamonds)                           { seq[0] = 0; seq[1] = 2; seq[2]= 3; seq[3] = 1; }
        if (trumps == Suit::Hearts)                             { seq[0] = 0; seq[1] = 2; seq[2]= 1; seq[3] = 3; }
    }

    if (!spades && clubs && diamonds && hearts) {
        if (trumps == Suit::Spades)                             { seq[0] = 3; seq[1] = 1; seq[2]= 0; seq[3] = 2; } 
        if (trumps == Suit::Clubs)                              { seq[0] = 3; seq[1] = 1; seq[2]= 0; seq[3] = 2; } 
        if (trumps == Suit::Diamonds)                           { seq[0] = 3; seq[1] = 1; seq[2]= 2; seq[3] = 0; }
        if (trumps == Suit::Hearts)                             { seq[0] = 3; seq[1] = 1; seq[2]= 0; seq[3] = 2; }
    }

    if (spades && !clubs && diamonds && hearts) {
        if (trumps == Suit::Spades)                             { seq[0] = 1; seq[1] = 3; seq[2]= 0; seq[3] = 2; }
        if (trumps == Suit::Clubs)                              { seq[0] = 1; seq[1] = 3; seq[2]= 0; seq[3] = 2; }
        if (trumps == Suit::Diamonds)                           { seq[0] = 1; seq[1] = 3; seq[2]= 2; seq[3] = 0; }
        if (trumps == Suit::Hearts)                             { seq[0] = 1; seq[1] = 3; seq[2]= 0; seq[3] = 2; }
    }

    if (spades && clubs && !diamonds && hearts) {
        if (trumps == Suit::Spades)                             { seq[0] = 2; seq[1] = 0; seq[2]= 3; seq[3] = 1; }
        if (trumps == Suit::Clubs)                              { seq[0] = 0; seq[1] = 2; seq[2]= 3; seq[3] = 1; }
        if (trumps == Suit::Diamonds)                           { seq[0] = 0; seq[1] = 2; seq[2]= 3; seq[3] = 1; }
        if (trumps == Suit::Hearts)                             { seq[0] = 0; seq[1] = 2; seq[2]= 3; seq[3] = 1; }
    }

    if (spades && clubs && diamonds && !hearts) {
        if (trumps == Suit::Spades)                             { seq[0] = 2; seq[1] = 0; seq[2]= 1; seq[3] = 3; }
        if (trumps == Suit::Clubs)                              { seq[0] = 0; seq[1] = 2; seq[2]= 1; seq[3] = 3; }
        if (trumps == Suit::Diamonds)                           { seq[0] = 0; seq[1] = 2; seq[2]= 1; seq[3] = 3; }
        if (trumps == Suit::Hearts)                             { seq[0] = 0; seq[1] = 2; seq[2]= 1; seq[3] = 3; }
    }

    if (!spades && !clubs && diamonds && hearts) {
        if (trumps == Suit::Diamonds)                           { seq[0] = 0; seq[1] = 1; seq[2]= 3; seq[3] = 2; }
        if (trumps == Suit::Hearts)                             { seq[0] = 0; seq[1] = 1; seq[2]= 2; seq[3] = 3; }
    }

    if (spades && clubs && !diamonds && !hearts) {
        if (trumps == Suit::Spades)                             { seq[0] = 3; seq[1] = 2; seq[2]= 0; seq[3] = 1; }
        if (trumps == Suit::Clubs)                              { seq[0] = 2; seq[1] = 3; seq[2]= 0; seq[3] = 1; }
    }


    #if defined(DEBUG) && defined(DEBUG_SORT)
        
        if (this->playerNumber == 3) {

            DEBUG_PRINT(F("\n"));
            DEBUG_PRINT(spades);
            DEBUG_PRINT_SPACE();
            DEBUG_PRINT(clubs);
            DEBUG_PRINT_SPACE();
            DEBUG_PRINT(diamonds);
            DEBUG_PRINT_SPACE();
            DEBUG_PRINT(hearts);
            DEBUG_PRINT_SPACE();
            DEBUG_PRINT_SUIT(trumps);
            DEBUG_PRINT_SPACE();
        
            for(uint8_t i = 0; i< 4; i++) {
                DEBUG_PRINT(seq[i]);
            }

            DEBUG_PRINTLN();
        
        }

    #endif


    // Do a bubble sort ..

    for (uint8_t i = 0; i < 6; i++) {

        for (uint8_t j = i + 1; j < 6; j++) {

            if (this->cards[j].getSortValue(trumps, bidType, seq) < cards[i].getSortValue(trumps, bidType, seq)) {

                Card card;
                copyCard(cards[i], card);
                this->cards[i] = this->cards[j];
                copyCard(card, this->cards[j]);

            }
            
        }

    }


    this->cardCount = 0;
    
    for (uint8_t i = 0; i < 6; i++) {

        if (this->cards[i].getRank() != Rank::None) {
            
            this->cardCount++;

        }
        else {
            
            break;

        }

    }

}


void addCard(Card *card)   { 

    #if defined(DEBUG) && defined(DEBUG_PLAY_ADDCARD)
        DEBUG_PRINT(F("Card "));
        DEBUG_PRINT(cardCount);
        DEBUG_PRINT_SPACE();
        DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
        DEBUG_PRINTLN();
    #endif

    this->cards[this->cardCount].setRank(card->getRank()); 
    this->cards[this->cardCount].setSuit(card->getSuit()); 
    this->cards[this->cardCount].setOrigRank(card->getOrigRank()); 
    this->cards[this->cardCount].setOrigSuit(card->getOrigSuit()); 
    this->cardCount++;

}


uint8_t numberOfUnplayedCards_InSuit(Suit suit, Rank cardsAbove) {

    uint8_t count = this->gameRound->numberOfUnplayedCards_InSuit(suit, cardsAbove);


    // Remove those in the players hand ..

    for (int j = 0; j < this->cardCount; j++) {

        Card &card = this->cards[j];

        if (card.getSuit() == suit && card.getRank() > cardsAbove) {

            count--;

        }

    }


    // Remove any cards in kitty ..

    if (this->getPlayerNumber() == this->gameRound->getWinningBid_Idx()) {

        Card *card = this->gameRound->getKitty();

        if (card->getSuit() == suit && card->getRank() > cardsAbove) {

            count--;

        }
    
    }

    return count;

}




// Do we have the top unplayed card in the nominated suit ..

uint8_t getBottom_InSuit(Suit trumps, Suit suitToFollow) {


    #if defined(DEBUG) && defined(DEBUG_GETBOTTOM_INSUIT)
        DEBUG_PRINT(F("getBottom "));
        DEBUG_PRINT_SUIT(suitToFollow);
        DEBUG_PRINT(F(": "));
    #endif
    

    // What is the players lowest trump?

    uint8_t idx = this->getLowest_InSuit(suitToFollow);

    if (idx == Constants::No_Card)  {

        #if defined(DEBUG) && defined(DEBUG_GETBOTTOM_INSUIT)
            DEBUG_PRINTLN(F("player has none."));
        #endif

        return Constants::No_Card;

    }


    Card &playersLowestCard = this->cards[idx];

    #if defined(DEBUG) && defined(DEBUG_GETBOTTOM_INSUIT)

        DEBUG_PRINT(F("player holds "));
        DEBUG_PRINT_CARD(playersLowestCard.getSuit(), playersLowestCard.getRank());
        DEBUG_PRINT(F(": "));

    #endif


    // What is the lowest unplayed card in this suit?

    Rank lowestUnplayedCard = playersLowestCard.getRank();

    for (Rank rank = Rank::Nine; rank < static_cast<Rank>(static_cast<uint8_t>(playersLowestCard.getRank()) + 1); rank++) {

        if (trumps == suitToFollow && rank == Rank::Jack) continue;

        if (!this->gameRound->hasCardBeenPlayed(trumps, suitToFollow, rank)) {

            #if defined(DEBUG) && defined(DEBUG_GETBOTTOM_INSUIT)

                DEBUG_PRINT_CARD(suitToFollow, rank);
                DEBUG_PRINT(F(" not played. "));

            #endif

            lowestUnplayedCard = rank;     
            break;   

        } 
        else {

            #if defined(DEBUG) && defined(DEBUG_GETBOTTOM_INSUIT)

                DEBUG_PRINT_CARD(suitToFollow, rank);
                DEBUG_PRINT(F(" played. "));

            #endif

        }

    }


    // So are we holding the top card?

    #if defined(DEBUG) && defined(DEBUG_GETBOTTOM_INSUIT)
        DEBUG_PRINT(F("Lowest unplayed "));
        DEBUG_PRINT_CARD(suitToFollow, lowestUnplayedCard);
        DEBUG_PRINTLN(F("."));
    #endif

    if (lowestUnplayedCard == playersLowestCard.getRank()) {

        return idx;

    }

    return Constants::No_Card;

}


// Do we have the bottom unplayed card in the nominated suit below the nominated rank ..

uint8_t getBottom_InSuit(Suit trumps, Suit suitToFollow, Rank bottomRank) {


    // What is the players highest trump?

    uint8_t idx = this->getLowest_InSuit(suitToFollow);
    if (idx == Constants::No_Card)  return Constants::No_Card;

    Card &playersLowestCard = this->cards[idx];


    // What is the lowest unplayed card in this suit?

    Rank lowestUnplayedCard = Rank::TopCard;

    for (Rank rank = Rank::Nine; rank <= bottomRank; rank++) {

        if (trumps == suitToFollow && rank == Rank::Jack) continue;

        if (!this->gameRound->hasCardBeenPlayed(trumps, suitToFollow, rank)) {

            lowestUnplayedCard = rank;        

        } 

    }

    // So are we holding the bottom card?

    if (lowestUnplayedCard == playersLowestCard.getRank()) {

        return idx;

    }

    return Constants::No_Card;

}


uint8_t getNextLowest_InSuit(Suit suit, Rank specifiedRank) {

    Rank foundRank = Rank::None;
    uint8_t foundIdx = Constants::No_Card;

    #if defined(DEBUG) && defined(DEBUG_GETNEXTLOWEST_INSUIT)
        DEBUG_PRINT(F("getNextLowest_InSuit("));
        DEBUG_PRINT_SUIT(suit);
        DEBUG_PRINT(F(","));
        DEBUG_PRINT_RANK(specifiedRank);
        DEBUG_PRINT(F(") - "));
    #endif

    for (uint8_t i = 0; i < this->cardCount; i++) {
    
        Card &card = this->cards[i];

        #if defined(DEBUG) && defined(DEBUG_GETNEXTLOWEST_INSUIT)
            DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
            DEBUG_PRINT_SPACE();
        #endif


        if (card.getSuit() == suit) {

            if (card.getRank() > foundRank && card.getRank() < specifiedRank) {
            
                foundRank = card.getRank();
                foundIdx = i;

            }                

        }

    }


    if (foundIdx != Constants::No_Card) {

        Card &card = this->cards[foundIdx];

        #if defined(DEBUG) && defined(DEBUG_GETNEXTLOWEST_INSUIT)
            DEBUG_PRINT(F(" = "));
            DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
            DEBUG_PRINTLN();
        #endif

    }
    else {

        #if defined(DEBUG) && defined(DEBUG_GETNEXTLOWEST_INSUIT)
            DEBUG_PRINTLN(F(" = No_Card."));
        #endif
    
    }

    return foundIdx;

}

void copyCard(Card &fromCard, Card &toCard) {

    toCard.setSuit(fromCard.getSuit());
    toCard.setRank(fromCard.getRank());
    toCard.setOrigSuit(fromCard.getOrigSuit());
    toCard.setOrigRank(fromCard.getOrigRank());
    toCard.setSelected(fromCard.isSelected());

}

void copyCard(Card &fromCard, Card *toCard) {

    toCard->setSuit(fromCard.getSuit());
    toCard->setRank(fromCard.getRank());
    toCard->setOrigSuit(fromCard.getOrigSuit());
    toCard->setOrigRank(fromCard.getOrigRank());
    toCard->setSelected(fromCard.isSelected());

}