#pragma once
#include "../../fxdata/fxdata.h"
#include "../../fxdata/images/Images.h"

#define DEBUG_PRINT    Serial.print
#define DEBUG_PRINTLN  Serial.println
#define DEBUG_BREAK    asm volatile("break\n");

#define _DEBUG
#define _DEBUG_BID
#define _DEBUG_SKIP_DEAL
#define _DEBUG_BASIC
#define _DEBUG_RAND


// Bidding

#define _DEBUG_EVALUATE
#define _DEBUG_EVALUATE_SUIT
#define _DEBUG_EVALUATE_NOTRUMPS
#define _DEBUG_EVALUATE_MISERE

#define _DEBUG_PREPARE_SUIT_PILES
#define _DEBUG_CALC_MISERE_SCORE
#define _DEBUG_ELEVPART_EVALUATE

//Kitty

#define _DEBUG_HANDLEKITTY

//Play

#define _DEBUG_PLAYER_WINNING_HAND
#define _DEBUG_PLAYNOTRUMPS_LEAD
#define _DEBUG_PLAYNOTRUMPS_FOLLOW
#define _DEBUG_PLAYSUIT_LEAD;
#define _DEBUG_PLAYSUIT_FOLLOW;
#define _DEBUG_PLAYMISERE_LEAD
#define _DEBUG_PLAYMISERE_FOLLOW
#define _DEBUG_MARKCARDPLAYED
#define _DEBUG_HASCARDBEENPLAYED
#define _DEBUG_GETTOP_INSUIT
#define _DEBUG_SORT
#define _DEBUG_SORT_DETAIL
#define _DEBUG_PLAY_ADDCARD
#define _DEBUG_DISCARDALL_INSUIT
#define _DEBUG_DISCARDCARD
#define _DEBUG_GETBOTTOM_INSUIT
#define _DEBUG_GETNEXTLOWEST_INSUIT
#define _DEBUG_NUMBER_OF_UNPLAYED_CARDS


namespace Constants {

    const uint16_t DealCardCoords[] PROGMEM = {
      // x,  y,  noOfIncs  StartingRot, numberofRots
        44, 54,  // 01
        48, 48,  // 02
        52, 48,  // 03
        56, 48,  // 04
        60, 48,  // 05
        64, 48,  // 06
        68, 48,  // 07
        72, 48,  // 08
        76, 48,  // 09
        80, 48,  // 10
    };

    const int8_t Bounce[] PROGMEM = { -36, -30, -24, -16, -12, -6, 0, 4, 8, 12, 16, 20, 24, 28, 32, 28, 25, 22, 21, 21, 22, 25, 28, 32, 29, 27, 26, 26, 27, 28, 32, 30, 29, 30, 32, 31, 32 };

    constexpr uint8_t NoPlayer = 255;
    constexpr uint8_t NoCard = 255;
    constexpr uint8_t NoBid = 255;
    constexpr uint8_t NoWinner = 255;
    constexpr uint8_t PlayerCount = 4;
    constexpr uint8_t CardsInDeck = 52;
    constexpr uint8_t HumanPlayer = 1;
    constexpr uint8_t HumanTeam = 1;
    constexpr uint8_t BotTeam = 0;

    constexpr uint8_t Delay = 12;
    constexpr uint8_t Title_Start_Action = 60;
    

	static constexpr uint8_t Player_Count = 4;
	static constexpr uint8_t Suit_Count = 4;
	static constexpr uint8_t Team_Count = 2;

	static constexpr uint8_t Deal_Delay = 2;
	static constexpr uint8_t No_Card = 255;
	static constexpr uint8_t No_Player = 255;
	static constexpr uint8_t WinningBid_None = 255;

	static constexpr uint8_t Sort_None = 255;

	//													   						        J   Q   K   A
	static constexpr uint8_t ScoreType_Basic[] = 	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	static constexpr uint8_t ScoreType_Trumps[] = 	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 15, 21, 50 };

};

struct DealPhysics {

    int16_t x = 0;
    int16_t y = 0;

    int16_t xInc = 0;
    int16_t yInc = 0;
    int16_t x2 = 0;
    int16_t y2 = 0;
    int16_t rot = 0;
    int16_t rotInc = 27 * 16;

    static constexpr int16_t rotStart = 24 * 16;
    static constexpr int8_t noOfIncs = 24;

};