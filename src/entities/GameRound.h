#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Card.h"
#include "Bid.h"

struct GameRound {

    private:

        Bid highestBid; // Used to track bidding only.
        Bid bid[4];
        Card hand[4];
        Card kitty;

        uint8_t tricksWon[Constants::Player_Count];
        int16_t score[Constants::Team_Count];
        uint16_t playedCards[Constants::Suit_Count];
        TriState hasSuitInHand[Constants::Player_Count][Constants::Suit_Count];  

        uint8_t round = 0;
        uint8_t firstPlayer = 0;
        uint8_t currentPlayer = 0;
        uint8_t dealerIdx = 0;
        uint8_t winningBidIdx = Constants::WinningBid_None;   // Index to bid array ..

    public:

        Card *getHand(uint8_t playerIdx)                            { return &this->hand[playerIdx]; }
        Card *getKitty()                                            { return &this->kitty; }
        Bid &getBid(uint8_t playerIdx)                              { return this->bid[playerIdx]; }
        Bid &getHighestBid()                                        { return this->highestBid; }
        int16_t getScore(uint8_t teamIdx)                           { return this->score[teamIdx]; }
        uint8_t getRound()                                          { return this->round; }
        uint8_t getFirstPlayer_Idx()                                { return this->firstPlayer; }
        uint8_t getCurrentPlayer_Idx()                              { return this->currentPlayer; }
        uint8_t getWinningBid_Idx()                                 { return this->winningBidIdx; }
        uint8_t getDealer_Idx()                                     { return this->dealerIdx; }

        void setKitty(Card &kitty)                                  { this->kitty.setSuit(kitty.getSuit()); this->kitty.setRank(kitty.getRank()); }
        void setScore(uint8_t teamIdx, int16_t val)                 { this->score[teamIdx] = val; }
        void setRound(uint8_t val)                                  { this->round = val; }
        void setFirstPlayer_Idx(uint8_t val)                        { this->firstPlayer = val; }
        void setCurrentPlayer_Idx(uint8_t val)                      { this->currentPlayer = val; }
        void setWinningBid_Idx(uint8_t val)                         { this->winningBidIdx = val; }
        void setDealer_Idx(uint8_t val)                             { this->dealerIdx = val % 4; }

        uint8_t getTableCardCount() {

            uint8_t count = 0;

            if (this->hand[0].getSuit() != Suit::None) count++;
            if (this->hand[1].getSuit() != Suit::None) count++;
            if (this->hand[2].getSuit() != Suit::None) count++;
            if (this->hand[3].getSuit() != Suit::None) count++;
            
            return count;

        }

        void setHighestBid(Bid &bid) {
        
            this->highestBid.setBidScore(bid.getBidScore());
            this->highestBid.setBidType(bid.getBidType());
            // this->highestBid.setLevel(bid.getLevel());
            this->highestBid.setSuit(bid.getSuit());
            
        }
 
        TriState getHasSuitInHand(uint8_t playerIdx, Suit suitIdx) {

            return this->hasSuitInHand[playerIdx][static_cast<uint8_t>(suitIdx)];

        }

        void setHasSuitInHand(uint8_t playerIdx, Suit suitIdx, TriState val) {

            this->hasSuitInHand[playerIdx][static_cast<uint8_t>(suitIdx)] = val;
            
        }

        void incScore(uint8_t teamIdx, int16_t val, bool capAt500) { 

            if (capAt500 && this->score[teamIdx] + val >= 500) {

                this->score[teamIdx] = 490;
            
            }
            else {

                this->score[teamIdx] = this->score[teamIdx] + val; 
            
            }

        }

        void incRound() { 

            this->round++;
            
        }

        Bid &getWinningBid() {

            return this->bid[this->winningBidIdx];

        }

        uint8_t getTeam_TrickCount(uint8_t teamIdx) {
        
            switch (teamIdx) {
            
                case 0:
                case 2:
                    return this->tricksWon[0] + this->tricksWon[2];

                default:
                    return this->tricksWon[1] + this->tricksWon[3];
                
            }

        }


        uint8_t getWinningBid_Team() {
        
            switch (this->winningBidIdx) {
            
                case 0:
                case 2:
                    return 0;

                default:
                    return 1;
                
            }

        }


        int16_t getWinningScore_BidTeam(uint8_t tricks) {

            Bid winningBid = this->getWinningBid();

            switch (winningBid.getBidType()) {
            
                case BidType::Partner:

                    switch (tricks) {
                    
                        case 3 ... 4:
                            return 1;

                        case 5:
                            return 2;

                        default:
                            return 0;

                    }

                    break;

                case BidType::Alone:

                    switch (tricks) {
                    
                        case 3 ... 4:
                            return 1;

                        case 5:
                            return 4;

                        default:
                            return 0;

                    }

                    break;

            }

        }


        uint8_t getWinningScore_AltTeam(uint8_t tricks) {
        
            Bid winningBid = this->getWinningBid();

            switch (tricks) {
            
                case 0 ... 2:
                    return 2;

                case 5:
                    return 4;

                default:
                    return 0;

            }

        }


        void incTricksWon(uint8_t playerIdx) {

            this->tricksWon[playerIdx]++;

        }


        void clearKitty() {

            this->kitty.setRank(Rank::None);
            // this->kitty.setSuit(Suit::None);
            
        }


        void addKitty(Card *card) {

            this->kitty.setRank(card->getRank());
            this->kitty.setSuit(card->getSuit());
            this->kitty.setOrigRank(card->getRank());
            this->kitty.setOrigSuit(card->getSuit());
            
        }


        void resetRound() {
        
            this->winningBidIdx = 255;
            this->firstPlayer = 0;
            this->currentPlayer = 0;
            // this->jokerSuit = Suit::None;
            // this->playedJoker = false;
            // this->handCount = 10;
            this->highestBid.reset(Constants::No_Player);

            for (uint8_t i = 0; i < 4; i++) {

                this->bid[i].reset(i);
                this->hand[i].reset();
                this->playedCards[i] = 0;
                this->tricksWon[i] = 0;

                for (uint8_t playerIdx = 0; playerIdx < 4; playerIdx++) {
                    this->hasSuitInHand[playerIdx][i] = TriState::Maybe; 
                }

            }

            this->kitty.reset();

        }


        void resetHand(uint8_t winningPlayer) {
        
            this->firstPlayer = winningPlayer;
            this->currentPlayer = winningPlayer;

        }

        void resetHand() {

            for (uint8_t i = 0; i < 4; i++) {
                this->hand[i].reset();
            }

        }


        bool hasPlayedCard(uint8_t playerIdx) {

            Card *card = this->getHand(playerIdx);

            return card->getRank() != Rank::None;

        }
        

        Card &getCardLed() {
        
            return this->hand[this->firstPlayer];

        }


        bool hasCardBeenPlayed(Suit trumps, Suit suit, Rank rank) {

            uint16_t mask = 1 << static_cast<uint8_t>(rank);

            if (rank == Rank::Right_Bower) {

                if (suit == trumps) {
                
                    rank = Rank::Jack;
                    mask = 1 << static_cast<uint8_t>(rank); //SJH need to copy to 500

                    if ((this->playedCards[static_cast<uint8_t>(suit)] & mask) > 0) {

                        return true;        

                    } 

                }
                else {

                    return false;

                }
                
            }
            else if (rank == Rank::Left_Bower) {

                if (suit == trumps) {
                    
                    rank = Rank::Jack;

                    switch (trumps) {
                    
                        case Suit::Spades:      suit = Suit::Clubs; break;
                        case Suit::Clubs:       suit = Suit::Spades; break;
                        case Suit::Diamonds:    suit = Suit::Hearts; break;
                        case Suit::Hearts:      suit = Suit::Diamonds; break;
                    }

                    mask = 1 << static_cast<uint8_t>(rank); //SJH need to copy to 500

                    if ((this->playedCards[static_cast<uint8_t>(suit)] & mask) > 0) {

                        return true;        

                    } 

                }
                else {
                
                    return false;

                }
                
            }
            else {

                #if defined(DEBUG) && defined(DEBUG_HASCARDBEENPLAYED)
                    DEBUG_PRINT(F("Has card been played? "));
                    DEBUG_PRINT_RANK(rank);
                    DEBUG_PRINT_SUIT(suit);
                    DEBUG_PRINT_SPACE();
                    DEBUG_PRINTLN(this->playedCards[static_cast<uint8_t>(suit)] & mask);
                #endif

                if ((this->playedCards[static_cast<uint8_t>(suit)] & mask) > 0) {

                    return true;        

                } 

            }

            return false;

        }


        void markCardPlayed(Suit suit, Rank rank) {

            #if defined(DEBUG) && defined(DEBUG_MARKCARDPLAYED)
                DEBUG_PRINT(F("Mark Card "));
                DEBUG_PRINT_RANK(rank);
                DEBUG_PRINT_SUIT(suit);
                DEBUG_PRINTLN();
            #endif
            
            uint16_t mask = 1 << static_cast<uint8_t>(rank);
            this->playedCards[static_cast<uint8_t>(suit)] = this->playedCards[static_cast<uint8_t>(suit)] | mask;

        }


        Suit winningBid_Suit() {

            if (this->winningBidIdx == Constants::WinningBid_None) { 
                return Suit::None;
            }

            return this->bid[this->winningBidIdx].getSuit();
            
        }


        BidType winningBid_Type() {

            return this->bid[this->winningBidIdx].getBidType();
            
        }


        Suit suitPlayerCalled(uint8_t playerIdx) {

            return this->bid[playerIdx].getSuit();

        }


        bool isLastPlayer(uint8_t playerIdx) {

            return ((this->firstPlayer + 3) % 4 == (playerIdx % 4));

        }


        TriState hasTrumps(Suit trumps, uint8_t playerIdx) {

            return canPlayerFollowSuit(winningBid_Suit(), playerIdx);

        }


        TriState canPlayerFollowSuit(Suit suitToFollow, uint8_t playerIdx) {
//SJH should consider what is in the current players hand.  
            Suit trumps = winningBid_Suit();
            bool hasSuit = false;

            if (suitToFollow == trumps) {
                
                if (this->hasSuitInHand[playerIdx][static_cast<uint8_t>(suitToFollow)] == TriState::False) {
                    return TriState::False;
                }
                else if (this->hasSuitInHand[playerIdx][static_cast<uint8_t>(suitToFollow)] == TriState::Maybe) {
                    return TriState::Maybe;
                }
                else if (this->hasCardBeenPlayed(trumps, getTrump_AltSuit(suitToFollow), Rank::Jack)) {
                    return TriState::Maybe;
                }

                return TriState::False;
            
            }
            else {
            
                return this->hasSuitInHand[playerIdx][static_cast<uint8_t>(suitToFollow)];

            }

        }


        uint8_t numberOfUnplayedCards_InSuit(Suit suit, Rank cardsAbove) {

            uint8_t count = 0;
            Suit trumps = winningBid_Suit();
            
            // Reposition to bottom of the deck if no 'cardsAbove' provided ..

            if (cardsAbove == Rank::None) cardsAbove = Rank::Nine;

            #if defined(DEBUG) && defined(DEBUG_NUMBER_OF_UNPLAYED_CARDS)
                DEBUG_PRINT(F("numberOfUnplayedCards_InSuit("));
                DEBUG_PRINT_SUIT(suit);
                DEBUG_PRINT(F(", "));
                DEBUG_PRINT_RANK(cardsAbove);
                DEBUG_PRINT(F(") "));
            #endif

            Rank topRank = (trumps == suit ? Rank::Joker : Rank::Ace);

            for (Rank rank = cardsAbove; rank <= topRank; rank++) {      

                if (trumps == suit && rank == Rank::Jack) continue;

                if (rank != cardsAbove && !this->hasCardBeenPlayed(trumps, suit, rank)) {

                    #if defined(DEBUG) && defined(DEBUG_NUMBER_OF_UNPLAYED_CARDS)
                        DEBUG_PRINT_CARD(suit, rank);
                        DEBUG_PRINT(F(", "));
                    #endif

                    count++;
                }

            }


            //SJH remove count of any cards in players hand or in the kitty (if player is winning bid)

            #if defined(DEBUG) && defined(DEBUG_NUMBER_OF_UNPLAYED_CARDS)
                DEBUG_PRINT(F(" = "));
                DEBUG_PRINTLN(count);
            #endif

            return count;

        }


        bool hasTrumpsBeenLed() {

            BidType bidType = winningBid_Type();
            Suit trumps = winningBid_Suit();

            if (this->firstPlayer == this->currentPlayer) return false;     // No one has played a card ..

            return this->hand[this->firstPlayer].isTrump(trumps);

        }


        bool hasHandBeenTrumped() {

            Suit trumps = winningBid_Suit();

            for (uint8_t i = 0; i < 4; i++) {

                Card &card = this->hand[i];

                if (this->hand[i].isTrump(trumps)) {

                    return true;

                }

            }

            return false;

        }


        bool weWonCall(uint8_t playerIdx, uint8_t partnerIdx) {

            return (this->winningBidIdx == playerIdx || this->winningBidIdx == partnerIdx);

        }


        bool isFirstRound() {
            return this->round == 1;
        }


        uint8_t getUnplayedCountofHigherCards_InSuit(Suit trumps, Card &card) {

            uint8_t count = 0;

            if (card.getSuit() == trumps) {
                
                for (Rank rank = static_cast<Rank>(static_cast<uint8_t>(card.getRank()) + 1); rank < Rank::TopCard; rank++) {

                    if (rank == Rank::Jack) continue;

                    if (!this->hasCardBeenPlayed(trumps, card.getSuit(), rank)) {
                        count++;
                    }

                }

            }
            else {
                
                for (Rank rank = static_cast<Rank>(static_cast<uint8_t>(card.getRank()) + 1); rank <= Rank::Ace; rank++) {

                    if (!this->hasCardBeenPlayed(trumps, card.getSuit(), rank)) {
                        count++;
                    }

                }

            }
                
            return count;

        }


        Card &getLargestCardInPlay(Suit trumps) {

            Suit cardLedSuit = this->hand[this->firstPlayer].getSuit();
            uint8_t returnIdx = Constants::No_Card;
            Rank rank = Rank::None;
            bool hasBeenTrumped = this->hasHandBeenTrumped();

            #if defined(DEBUG) && defined(DEBUG_GETLARGESTCARDINPLAY)
                DEBUG_PRINT(F("getLargestCardInPlay() trumps "));
                DEBUG_PRINT_SUIT(trumps);
                DEBUG_PRINT(F(", suit led "));
                DEBUG_PRINT_SUIT(cardLedSuit);
            #endif

            if (hasBeenTrumped) {

                for (uint8_t i = 0; i < 4; i++) {

                    Card &card = this->hand[i];

                    #if defined(DEBUG) && defined(DEBUG_GETLARGESTCARDINPLAY)
                        DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                        DEBUG_PRINT_SPACE();
                    #endif

                    if (card.getSuit() == trumps && card.getRank() > rank) {

                        rank = card.getRank();
                        returnIdx = i;

                    }

                }

            }
            else {

                for (uint8_t i = 0; i < 4; i++) {

                    Card &card = this->hand[i];

                    #if defined(DEBUG) && defined(DEBUG_GETLARGESTCARDINPLAY)
                        DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                        DEBUG_PRINT_SPACE();
                    #endif

                    if (card.getSuit() == cardLedSuit && card.getRank() > rank) {

                        rank = card.getRank();
                        returnIdx = i;

                    }

                }

            }

            if (returnIdx != Constants::No_Card) {

                #if defined(DEBUG) && defined(DEBUG_GETLARGESTCARDINPLAY)
                    Card &card = this->hand[returnIdx];
                    DEBUG_PRINT(F(" = "));
                    DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                    DEBUG_PRINTLN(F(" "));
                #endif

                return this->hand[returnIdx];

            }
            else {

                #if defined(DEBUG) && defined(DEBUG_GETLARGESTCARDINPLAY)
                    DEBUG_PRINTLN(F(" = (not found)"));
                #endif

                return this->hand[this->currentPlayer];            // Return current players card which should be a Rank::None.

            }

        }


        Card &getLargestTrumpInPlay(Suit trumps) {

            Rank rank = Rank::None;
            uint8_t returnIdx = Constants::No_Card;

            for (uint8_t i = 0; i < 4; i++) {

                Card &card = this->hand[i];

                if (card.isTrump(trumps) && card.getRank() > rank) {

                    rank = card.getRank();
                    returnIdx = i;

                }

            }

            if (returnIdx != Constants::No_Card) {

                return this->hand[returnIdx];

            }
            else {
            
                return this->hand[this->currentPlayer];            // Return current players card which should be a Rank::None.
            }

        }


        bool isPlayerWinningHand(uint8_t playerIdx) {

            uint8_t winningHand = this->getWinningHand();
            return (playerIdx == winningHand);

        }


        uint8_t getWinningHand() {

            BidType bidType = this->bid[this->winningBidIdx].getBidType();

            #if defined(DEBUG) && defined(DEBUG_PLAYER_WINNING_HAND)

                DEBUG_PRINT(F("getWinningHand() "));

                for (uint8_t i = 0; i < 4; i++) {
                    Card &card = this->hand[i];
                    DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                    DEBUG_PRINT_SPACE();
                }

                DEBUG_PRINT_SPACE();

            #endif

            uint8_t returnIdx = Constants::No_Card;
            Rank rank = Rank::None;

            Suit trumps = winningBid_Suit();
            Suit cardLedSuit = this->hand[this->firstPlayer].getSuit();

            #if defined(DEBUG) && defined(DEBUG_PLAYER_WINNING_HAND)

                DEBUG_PRINT(F("Card led suit: "));            
                DEBUG_PRINT_SUIT(cardLedSuit);
                DEBUG_PRINTLN();

            #endif

            switch (bidType) {
            
                case BidType::Partner:
                case BidType::Alone:

                    if (this->hasHandBeenTrumped()) {

                        #if defined(DEBUG) && defined(DEBUG_PLAYER_WINNING_HAND)
                            DEBUG_PRINTLN(F("Trick has been trumped."));            
                        #endif

                        for (uint8_t i = 0; i < 4; i++) {

                            Card &card = this->hand[i];

                            if (card.isTrump(trumps) && card.getRank() > rank) {

                                rank = card.getRank();
                                returnIdx = i;

                            }

                        }       

                    }
                    else {

                        #if defined(DEBUG) && defined(DEBUG_PLAYER_WINNING_HAND)
                            DEBUG_PRINTLN(F("Trick has not been trumped."));            
                        #endif

                        for (uint8_t i = 0; i < 4; i++) {

                            Card &card = this->hand[i];

                            if (card.getSuit() == cardLedSuit && card.getRank() > rank) {

                                rank = card.getRank();
                                returnIdx = i;

                            }

                        }       
                    
                    }
                               
                    #if defined(DEBUG) && defined(DEBUG_PLAYER_WINNING_HAND)

                        DEBUG_PRINT(F(", BidType::Partner Win:"));
                        DEBUG_PRINTLN(returnIdx);

                    #endif

                    return returnIdx;
            
            }


        }

};
