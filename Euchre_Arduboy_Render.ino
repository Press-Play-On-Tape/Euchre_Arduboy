#include <ArduboyFX.h>  


void renderPlayerHands(uint8_t currentPlane, bool blinkSelected, bool blinkRaised) {

    if (game.players[Constants::HumanPlayer].isPlaying()) {
        
        uint8_t cardCount = game.players[Constants::HumanPlayer].getCardCount();
        int8_t xStart = 48 - (cardCount * 4);

        for (uint8_t i = 0; i < cardCount; i++) {

            Card &card = game.players[Constants::HumanPlayer].getCard(i);
            Rank rank = card.getRank();

            if (rank != Rank::None) {

                uint8_t y = 47;


                // Raise cards only when the player is throwing cards from the kitty or its his turn ..

                switch (gameState){
                
                    case GameState::Play_Hand:
                    case GameState::Play_PlayerInput:

                        if (gameRound.getCurrentPlayer_Idx() == Constants::HumanPlayer) {
                            
                            y = 47 - (card.isSelected() ? 4 : 0);

                        }

                        break;

                    case GameState::Handle_Kitty:

                        // if (gameRound.getHighestBid().getPlayerIdx() == Constants::HumanPlayer) {
                        if (gameState == GameState::Handle_Kitty) {
                            
                            y = 47 - (card.isSelected() ? 4 : 0);

                        }

                        break;

                }

                if ((blinkRaised && card.isSelected() || (blinkSelected && i == selectedCard)) && game.getFrameCount(48)) {
                
                    SpritesU::drawPlusMaskFX(xStart + (i * 8), y, Images::Cards_Bottom_Grey, game.players[Constants::HumanPlayer].getCard(i).getCardIndex() + currentPlane);

                }
                else {

                    SpritesU::drawPlusMaskFX(xStart + (i * 8), y, Images::Cards_Bottom, game.players[Constants::HumanPlayer].getCard(i).getCardIndex() + currentPlane);

                }

            }

        }

    }


    if (game.players[2].getCardCount() > 0 && game.players[2].isPlaying()) {
        SpritesU::drawOverwriteFX(0, 6, Images::Hand_Left, (game.players[2].getCardCount() * 3) + currentPlane);
    }

    if (game.players[3].getCardCount() > 0 && game.players[3].isPlaying()) {
        SpritesU::drawOverwriteFX(31, -3, Images::Hand_Top, (game.players[3].getCardCount() * 3) + currentPlane);
    }

    if (game.players[0].getCardCount() > 0 && game.players[0].isPlaying()) {
        SpritesU::drawOverwriteFX(100, 6, Images::Hand_Right, (game.players[0].getCardCount() * 3) + currentPlane);
    }

}


void renderKitty(uint8_t currentPlane) {

    if (gameRound.getKitty()->getRank() != Rank::None) {

        SpritesU::drawPlusMaskFX(26, 14, Images::Cards_Bottom, gameRound.getKitty()->getCardIndex() + currentPlane);

    }

}


void renderDealer(uint8_t currentPlane) {

    uint8_t dealer = gameRound.getDealer_Idx();
    
    uint8_t x;
    uint8_t y;

    switch (dealer) {

        case 0:
            x = 93;
            y = 23;
            break;

        case 1:
            x = 52;
            y = 40;
            break;

        case 2:
            x = 7;
            y = 23;
            break;

        case 3:
            x = 52;
            y = 6;
            break;

    }

    SpritesU::drawPlusMaskFX(x, y, Images::Dealer, currentPlane); 

}


void renderDiscard(uint8_t currentPlane) {

    uint8_t dealer = gameRound.getDealer_Idx();

    switch (dealer) {
    
        case 0:
            SpritesU::drawPlusMaskFX(92, 13, Images::Bid_Right_Discard, currentPlane); 
            break;

        case 2:
            SpritesU::drawPlusMaskFX(6, 15, Images::Bid_Left_Discard, currentPlane);
            break;

        case 3:
            SpritesU::drawPlusMaskFX(39, 6, Images::Bid_Top_Discard, currentPlane);
            break;
    
    }

}

void renderBids(uint8_t currentPlane) {
    
    uint8_t dealer = gameRound.getDealer_Idx();
    uint8_t bidder_X = 0;
    uint8_t bidder_Y;

    { // Player 0

        Bid &bid = gameRound.getBid(0);

        switch (bid.getBidType()) {

            case BidType::Partner:
                {
               
                    SpritesU::drawPlusMaskFX(92, 17, Images::Bid_Right, (1 * 3) + currentPlane); 
                
                    if (dealer == 0) {
                        bidder_X = 93;
                        bidder_Y = 6;  
                    }
                
                }
                break;

            case BidType::Alone:
                {
                
                    SpritesU::drawPlusMaskFX(92, 16, Images::Bid_Right, (0 * 3) + currentPlane); 
                
                    if (dealer == 0) {
                        bidder_X = 93;
                        bidder_Y = 6;  
                    }
                
                }
                break;

            case BidType::Pass:
                
                SpritesU::drawPlusMaskFX(92, 15, Images::Bid_Right, (2 * 3) + currentPlane); 
                
                if (dealer == 0) {
                    bidder_X = 93;
                    bidder_Y = 10;  
                }

                break;


            case BidType::None:
                
                if (dealer == 0) {
                    bidder_X = 93;
                    bidder_Y = 23;                  
                }

                break;

        }


    }

    { // Player 1

        Bid &bid = gameRound.getBid(1);

        switch (bid.getBidType()) {

            case BidType::Partner:
                {
                    SpritesU::drawPlusMaskFX(41, 40, Images::Bid_Bottom, (1 * 3) + currentPlane); 

                    if (dealer == Constants::HumanPlayer) {
                        bidder_Y = 40;                       
                    }

                }
                break;


            case BidType::Alone:
                {
                    SpritesU::drawPlusMaskFX(41, 40, Images::Bid_Bottom, (2 * 3) + currentPlane); 

                    if (dealer == Constants::HumanPlayer) {
                        bidder_Y = 40;                       
                    }

                }
                break;

            case BidType::Pass:

                SpritesU::drawPlusMaskFX(41, 40, Images::Bid_Bottom, (0 * 3) + currentPlane); 

                if (dealer == Constants::HumanPlayer) {
                    bidder_X = 35;
                    bidder_Y = 40;                       
                }

                break;
        
            case BidType::None:

                if (dealer == Constants::HumanPlayer) {
                    bidder_X = 52;
                    bidder_Y = 40;                       
                }

                break;

        }


    }

    { // Player 2

        Bid &bid = gameRound.getBid(2);

        switch (bid.getBidType()) {

            case BidType::Partner:
                {
                    SpritesU::drawPlusMaskFX(6, 16, Images::Bid_Left, (1 * 3) + currentPlane); 

                    if (dealer == 2) {
                        bidder_X = 7;
                        bidder_Y = 7;                       
                    }

                }
                break;

            case BidType::Alone:
                {
                    SpritesU::drawPlusMaskFX(6, 16, Images::Bid_Left, (2 * 3) + currentPlane); 

                    if (dealer == 2) {
                        bidder_X = 7;
                        bidder_Y = 7;                       
                    }

                }
                break; 

            case BidType::Pass:

                SpritesU::drawPlusMaskFX(6, 16, Images::Bid_Left, (0 * 3) + currentPlane); 

                if (dealer == 2) {
                    bidder_X = 7;
                    bidder_Y = 11;                       
                }

                break;
                
            case BidType::None:

                if (dealer == 2) {
                    bidder_X = 7;
                    bidder_Y = 23;                       
                }

                break;

        }

    }

    { // Player 3

        Bid &bid = gameRound.getBid(3);

        switch (bid.getBidType()) {

            case BidType::Partner:
                {
                    SpritesU::drawPlusMaskFX(41, 6, Images::Bid_Top, (1 * 3) + currentPlane); 

                    if (dealer == 3) {
                        bidder_Y = 6;                       
                    }
                    
                }
                break;

            case BidType::Alone:
                {
                    SpritesU::drawPlusMaskFX(41, 6, Images::Bid_Top, (2 * 3) + currentPlane); 

                    if (dealer == 3) {
                        bidder_Y = 6;                       
                    }
                    
                }
                break;

            case BidType::Pass:

                SpritesU::drawPlusMaskFX(41, 6, Images::Bid_Top, (0 * 3) + currentPlane); 

                if (dealer == 3) {
                    bidder_X = 64;
                    bidder_Y = 6;                       
                }

                break;
        
            case BidType::None:

                if (dealer == 3) {
                    bidder_X = 53;
                    bidder_Y = 6;                       
                }

                break;

        }

    }

    if (bidder_X > 0) {

        SpritesU::drawPlusMaskFX(bidder_X, bidder_Y, Images::Dealer, currentPlane); 

    }


}


void renderTableCards(uint8_t currentPlane, uint8_t winningHand) {

    renderTableCards(currentPlane, winningHand, 0);

}


void renderTableCards(uint8_t currentPlane, uint8_t winningHand, uint8_t y) {

    renderTableCards_03(currentPlane, winningHand, y);
    renderTableCards_01(currentPlane, winningHand, y);
    renderTableCards_00(currentPlane, winningHand, y);
    renderTableCards_02(currentPlane, winningHand, y);

}

void renderTableCards_00(uint8_t currentPlane, uint8_t winningHand, uint8_t y) {   

    if (winningHand != 0 || (winningHand == 0 && game.getFrameCount(48))) {

        Card *card = gameRound.getHand(0);

        if (card->getRank() != Rank::None)  {

            uint8_t idx = card->getCardIndex();
            SpritesU::drawPlusMaskFX(60, y + 13, Images::Cards_Right, idx + currentPlane); 

        }

    }
        
}

void renderTableCards_01(uint8_t currentPlane, uint8_t winningHand, uint8_t y) {

    if (winningHand != 1 || (winningHand == 1 && game.getFrameCount(48))) {

        Card *card = gameRound.getHand(1);

        if (card->getRank() != Rank::None)  {

            uint8_t idx = card->getCardIndex();
            SpritesU::drawPlusMaskFX(49, y + 19, Images::Cards_Bottom, idx + currentPlane); 

        }

    }

}

void renderTableCards_02(uint8_t currentPlane, uint8_t winningHand, uint8_t y) {

    if (winningHand != 2 || (winningHand == 2 && game.getFrameCount(48))) {

        Card *card = gameRound.getHand(2);

        if (card->getRank() != Rank::None)  {

            uint8_t idx = card->getCardIndex();
            SpritesU::drawPlusMaskFX(20, y + 21, Images::Cards_Left, idx + currentPlane); 

        }

    }

}

void renderTableCards_03(uint8_t currentPlane, uint8_t winningHand, uint8_t y) {
        
    if (winningHand != 3 || (winningHand == 3 && game.getFrameCount(48))) {

        Card *card = gameRound.getHand(3);

        if (card->getRank() != Rank::None)  {

            uint8_t idx = card->getCardIndex();
            SpritesU::drawPlusMaskFX(38, y + 9, Images::Cards_Bottom, idx + currentPlane); 

        }

    }
        
}

void renderHUD(uint8_t currentPlane, bool displayCard, bool displayWinningBid) {

    if (displayCard && !displayWinningBid) {

        renderHUD_Top(currentPlane, 0, false, false);
        renderHUD_Bottom(currentPlane, 6, true, true);

    }
    else if (!displayCard && !displayWinningBid) {

        renderHUD_Top(currentPlane, 3, false, false);
        renderHUD_Bottom(currentPlane, 6, true, true);

    }
    else if (!displayCard && displayWinningBid) {

        switch (gameState) {

            case GameState::Play_EndOfHand:
            case GameState::Play_EndOfRound:
            case GameState::Play_EndOfGame:

                if (gameRound.getWinningHand() == 1 || gameRound.getWinningHand() == 3) {

                    if (game.getFrameCount(48)) {

                        renderHUD_Top(currentPlane, 6, true, true);
    
                    }
                    else {
                    
                        renderHUD_Top(currentPlane, 9, false, true);

                    }
                
                }
                else {

                    if (game.getFrameCount(48)) {

                        renderHUD_Top(currentPlane, 6, true, true);
    
                    }
                    else {

                        renderHUD_Top(currentPlane, 12, true, false);

                    }


                }

                if (gameState != GameState::Play_EndOfHand) {

                    uint8_t biddingTeam = gameRound.getWinningBid_Team();
                    int16_t winningScore_BidTeam = gameRound.getWinningScore_BidTeam(gameRound.getTeam_TrickCount(biddingTeam));
                    
                    if ((biddingTeam == 1 && winningScore_BidTeam > 0) || (biddingTeam == 0 && winningScore_BidTeam == 0)) {

                        if (game.getFrameCount(48)) {

                            renderHUD_Bottom(currentPlane, 6, true, true);
        
                        }
                        else {
                        
                            renderHUD_Bottom(currentPlane, 9, false, true);

                        }
                    
                    }
                    else {

                        if (game.getFrameCount(48)) {

                            renderHUD_Bottom(currentPlane, 6, true, true);
        
                        }
                        else {

                            renderHUD_Bottom(currentPlane, 12, true, false);

                        }


                    }

                }
                else {

                    renderHUD_Bottom(currentPlane, 6, true, true);

                }

                break;

            default:
                renderHUD_Top(currentPlane, 6, true, true);
                renderHUD_Bottom(currentPlane, 6, true, true);
                break;

        }
        
        if (gameRound.getWinningBid_Idx() != Constants::WinningBid_None) {

            SpritesU::drawOverwriteFX(110, 25, Images::Suit, (static_cast<uint8_t>(gameRound.getWinningBid().getSuit()) * 3) + currentPlane);

        }

    }

}

void renderHUD_Top(uint8_t currentPlane, uint8_t backgroundIdx, bool showTeam0, bool showTeam1) {

    backgroundIdx = backgroundIdx + (backgroundIdx >= 6 ? gameRound.getFirstPlayer_Idx() * 9 : 0);
    SpritesU::drawOverwriteFX(105, 0, Images::HUD_Top, backgroundIdx + currentPlane);
    if (showTeam0)    SpritesU::drawOverwriteFX(118,  8, Images::HUD_Trick_Score, (gameRound.getTeam_TrickCount(1) * 3) + currentPlane);
    if (showTeam1)    SpritesU::drawOverwriteFX(118, 15, Images::HUD_Trick_Score, (gameRound.getTeam_TrickCount(0) * 3) + currentPlane);

}


void renderHUD_Bottom(uint8_t currentPlane, uint8_t backgroundIdx, bool showTeam0, bool showTeam1) {

    if (backgroundIdx >= 6) {
        backgroundIdx = backgroundIdx + (gameRound.getWinningBid_Idx() == 255 ? 4 * 9 : gameRound.getWinningBid_Idx() * 9);
    }

    SpritesU::drawOverwriteFX(105, 32, Images::HUD_Bottom, backgroundIdx + currentPlane);

    if (showTeam0) SpritesU::drawOverwriteFX(118, 51, Images::HUD_Trick_Score, (gameRound.getScore(1) * 3) + currentPlane);
    if (showTeam1) SpritesU::drawOverwriteFX(118, 58, Images::HUD_Trick_Score, (gameRound.getScore(0) * 3) + currentPlane);

}