#pragma once

#include <Arduboy2.h>
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "Card.h"

struct Deck {

    private:

        Card deck[24];
        uint8_t deckSize = 0;
        uint8_t deckPointer = 0;

    public:

        void createDeck() {

            deckSize = 0;

            for (Suit suit = Suit::Spades; suit <= Suit::Hearts; suit++) {

                for (Rank rank = Rank::Nine; rank <= Rank::Ace; rank++) {
                    deck[deckSize].setSuit(suit);
                    deck[deckSize].setRank(rank);
                    deck[deckSize].setOrigSuit(suit);
                    deck[deckSize].setOrigRank(rank);
                    deck[deckSize].setSelected(false);
                    deckSize++;
                }

            }

            deckPointer = 0;

        }

        void shuffleDeck() {

            for (uint8_t i = 0; i < deckSize; i++) {
                uint8_t r = random(i, deckSize);
                Card temp = deck[i];
                deck[i] = deck[r];
                deck[r] = temp;
            }

        }

        Card *getCard() {

            deckPointer++;
            return &this->deck[deckPointer - 1];

        }

};
