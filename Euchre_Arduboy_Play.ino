#include <ArduboyFX.h>  


void continueBidding() {

    uint8_t passes = 0;

    for (uint8_t i = 0; i < 4; i++) {

        switch (gameRound.getBid(i).getBidType()) {

            case BidType::Pass:     
                passes++;
                break;

        }

    }

    if (passes < 4) {

        do {    // Skip to the next non-passed player ..

            playerCurrentlyBidding = (playerCurrentlyBidding + 1) % 4;

        }
        while (gameRound.getBid(playerCurrentlyBidding).getBidType() == BidType::Pass);
        game.setFrameCount(0);

    }
    else {

        switch (gameState) {
        
            case GameState::Bid_Round1:
            case GameState::Bid_Player_Round1:

                playerCurrentlyBidding = (playerCurrentlyBidding + 1) % 4;
                gameState = GameState::Bid_Round_Delay;
                game.setFrameCount(0);
                break;

            default:

                gameState = GameState::Bid_Failed;
                break;

        }

    }
    
}

void incHand() {

    uint8_t player = gameRound.getCurrentPlayer_Idx();

    while (true) {

        player = (player + 1) % 4;

        if (game.players[player].isPlaying()) {

            gameRound.setCurrentPlayer_Idx(player);
            break;

        }
    
    } 

}

void completeBid(uint8_t winningBidIdx, BidType bidType, Suit suit) {

    gameRound.setWinningBid_Idx(winningBidIdx);
    gameRound.getHighestBid().setPlayerIdx(winningBidIdx);
    gameRound.getHighestBid().setBidType(bidType);
    gameRound.getHighestBid().setSuit(suit);
    gameRound.getBid(winningBidIdx).setBidType(bidType);
    gameRound.getBid(winningBidIdx).setSuit(suit);



    for (uint8_t i = 0; i < 4; i++) {

        // Set is playing ..

        if (bidType == BidType::Alone && (winningBidIdx + 2) % 4 == i) {
            game.players[i].setPlaying(false);
        }
        else {
            game.players[i].setPlaying(true);
        }

        for (uint8_t j = 0; j < game.players[i].getCardCount(); j++) {

            game.players[i].getCard(j).setTrumps(suit);

        }

        game.players[i].sort();

    }
    
    
    uint8_t dealer = gameRound.getDealer_Idx();
    uint8_t firstPlayer = dealer;

    while (true) {

        firstPlayer = (firstPlayer + 1) % 4;

        if (game.players[firstPlayer].isPlaying()) break;
    
    } 

    Serial.println(firstPlayer);

    gameRound.setFirstPlayer_Idx(firstPlayer);
    gameRound.setCurrentPlayer_Idx(firstPlayer);

    game.setFrameCount(0);
    kittyHighlight = 64;
    selectedCard = 0;
    
}

void play_Init() { 

    game.resetFrameCount();
    gameState = GameState::Play_Shuffle;
    gameRound.setScore(0, 0);
    gameRound.setScore(1, 0);

    // #ifdef DEBUG_RAND
    uint16_t r = random(8000);
    // DEBUG_PRINT("Rand ");
    // DEBUG_PRINTLN(r);
    randomSeed(r);
    game.setRandomSeed(r);
    // #endif
    randomSeed(14);

}

void populateRotateDetails(uint8_t card) {
    
    dealPhysics.x = 110 * 16;
    dealPhysics.y = 1 * 16;

    dealPhysics.x2 = pgm_read_byte(Constants::DealCardCoords + (card * 2)) * 16;
    dealPhysics.y2 = pgm_read_byte(Constants::DealCardCoords + (card * 2) + 1) * 16;
    dealPhysics.rotInc = 27 * 16;


    dealPhysics.xInc = (dealPhysics.x2 - dealPhysics.x) / dealPhysics.noOfIncs;
    dealPhysics.yInc = (dealPhysics.y2 - dealPhysics.y) / dealPhysics.noOfIncs;
    dealPhysics.x = (110 * 16) + dealPhysics.xInc;
    dealPhysics.y = -dealPhysics.yInc;
    dealPhysics.rotInc = (dealPhysics.rotInc / dealPhysics.noOfIncs);
    dealPhysics.rot = dealPhysics.rotStart;

}


uint8_t getCardIdx(uint8_t player, uint8_t offset, uint8_t cardsToBeDealt) {

    uint8_t dealer = gameRound.getDealer_Idx();
    return (cardsToBeDealt * ((player - dealer + 3) % 4)) + offset;

}

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void play_Update() { 

    uint8_t justPressed = getJustPressedButtons();

    game.incFrameCount();

    switch (gameState) {

        case GameState::Play_Shuffle:  

            game.deck.createDeck();
            game.deck.shuffleDeck();
            game.setFrameCount(0);
            gameState = GameState::Play_Deal_00;

            gameRound.resetRound();
            gameRound.setRound(0);

            for (uint8_t player = 0; player < 4; player++) {
                game.players[player].reset();
            }

            gameRound.setDealer_Idx(gameRound.getDealer_Idx() + 1); 
            playerCurrentlyBidding = (gameRound.getDealer_Idx() + 1) % 4;
            // gameRound.setDealer_Idx(2); //SJH
            // playerCurrentlyBidding = (2 + 1) % 4;

            if (gameRound.getDealer_Idx() == 0) {
                populateRotateDetails(0);
            }

        #ifdef DEBUG_SKIP_DEAL
            gameState = GameState::Play_Deal_Catchup;
            [[fallthrough]]
        #else 

            [[fallthrough]]

            case GameState::Play_Deal_00 ... GameState::Play_Deal_07:
                {

                    #ifdef DEBUG_SKIP_DEAL
                        gameState = GameState::Play_Deal_Catchup;
                    #endif

                    if (justPressed & A_BUTTON) {
                        gameState = GameState::Play_Deal_Catchup;
                        return;             
                    }

                    uint8_t dealer = gameRound.getDealer_Idx();
                    uint8_t card = static_cast<uint8_t>(gameState) - static_cast<uint8_t>(GameState::Play_Deal_00);
                    uint8_t round = (gameState <= GameState::Play_Deal_07 ? 0 : 30);

                    if (card >= getCardIdx(1, round, 2) && card <= getCardIdx(1, round, 2) + 1) {

                        uint8_t player = 1;

                        if (dealPhysics.x > dealPhysics.x2 && dealPhysics.y < dealPhysics.y2) {

                            dealPhysics.x = dealPhysics.x + dealPhysics.xInc;
                            dealPhysics.y = dealPhysics.y + dealPhysics.yInc;
                            dealPhysics.rot = dealPhysics.rot + dealPhysics.rotInc;

                            if (dealPhysics.x <= dealPhysics.x2 || dealPhysics.y >= dealPhysics.y2) {

                                dealCard(player);

                                game.players[1].sort();
                                gameState++;
                                game.setFrameCount(0);
                               

                                if (card < getCardIdx(1, round, 2) + 1) {
                                    populateRotateDetails(game.players[1].getCardCount());
                                }

                            }

                        }

                    }
                    else {

                        for (uint8_t player = 0; player < 4; player++) {

                            if (player == Constants::HumanPlayer) continue;
                        
                            if (card >= getCardIdx(player, round, 2) && card <= getCardIdx(player, round, 2) + 1) {

                                if (game.getFrameCount() == 6) {

                                    dealCard(player);

                                    gameState++;
                                    game.setFrameCount(0);

                                    if (card + 1 == getCardIdx(1, round, 2)) {
                                        populateRotateDetails(game.players[1].getCardCount());
                                    }

                                }

                            }

                        }

                    }

                }

                break;
      
            case GameState::Play_Deal_08 ... GameState::Play_Deal_19:
                {
                    uint8_t dealer = gameRound.getDealer_Idx();
                    uint8_t card = static_cast<uint8_t>(gameState) - static_cast<uint8_t>(GameState::Play_Deal_00);

                    if (justPressed & A_BUTTON) {
                        gameState = GameState::Play_Deal_Catchup;
                        return;             
                    }

                    if (card >= getCardIdx(1, 8, 3) && card <= getCardIdx(1, 8, 3) + 2) {

                        uint8_t player = 1;

                        if (dealPhysics.x > dealPhysics.x2 && dealPhysics.y < dealPhysics.y2) {

                            dealPhysics.x = dealPhysics.x + dealPhysics.xInc;
                            dealPhysics.y = dealPhysics.y + dealPhysics.yInc;
                            dealPhysics.rot = dealPhysics.rot + dealPhysics.rotInc;

                            if (dealPhysics.x <= dealPhysics.x2 || dealPhysics.y >= dealPhysics.y2) {

                                dealCard(player);
                                game.players[1].sort();
                                gameState++;
                                game.setFrameCount(0);

                                if (card < getCardIdx(1, 8, 3) + 3) {
                                    populateRotateDetails(game.players[1].getCardCount());
                                }

                            }

                        }

                    }
                    else {

                        for (uint8_t player = 0; player < 4; player++) {

                            if (player == Constants::HumanPlayer) continue;

                     
                            if (card >= getCardIdx(player, 8, 3) && card <= getCardIdx(player, 8, 3) + 2) {

                                if (game.getFrameCount() == 6) {

                                    dealCard(player);

                                    gameState++;
                                    game.setFrameCount(0);

                                    if (card + 1 == getCardIdx(1, 8, 3)) {
                                        populateRotateDetails(game.players[1].getCardCount());
                                    }

                                }

                            }

                        }

                    }

                }

                break;

            case GameState::Play_Deal_20:

                game.setFrameCount(0);
                gameState = GameState::Bid_Round1;
                dealKitty();
                break;
            
        #endif

        case GameState::Play_Deal_Catchup:

            for (uint8_t player = 0; player < 4; player++) {

                while (game.players[player].getCardCount() < 5) {
                    dealCard(player);
                }

            }

            if (gameRound.getKitty()->getRank() == Rank::None) {
                dealKitty();
            }

            game.setFrameCount(0);
            gameState = GameState::Bid_Round1;

            for (uint8_t i = 0; i < 4; i++) {
                game.players[i].sort();
            }

            break;

        case GameState::Bid_Round1:
            {

                bool isHuman = playerCurrentlyBidding == Constants::HumanPlayer;
                uint8_t dealer = gameRound.getDealer_Idx();
                uint16_t bid = 0;

                if (!isHuman) {
                    
                    if (game.getFrameCount() == 4) { //SJH 32
                        
                        bid = game.players[playerCurrentlyBidding].bid(gameRound.getKitty()->getSuit(), *gameRound.getKitty(), false);
                                        
                        if (bid >= 180) { //SJH 180

                            #ifdef DEBUG
                            DEBUG_PRINTLN("Go Alone");
                            #endif

                            game.players[dealer].addCard(gameRound.getKitty());
                            game.players[dealer].getCard(5).setSelected(true);

                            completeBid(playerCurrentlyBidding, BidType::Partner, gameRound.getKitty()->getSuit());
                            gameState = GameState::Handle_Kitty;

                        }
                        else if (bid >= 150) {
                        
                            #ifdef DEBUG
                            DEBUG_PRINTLN("Go!");
                            #endif
                        
                            completeBid(playerCurrentlyBidding, BidType::Partner, gameRound.getKitty()->getSuit());
                            gameState = GameState::Handle_Kitty;
                        
                        }
                        else {

                            uint8_t winningBidIdx = playerCurrentlyBidding;

                            gameRound.getBid(winningBidIdx).setBidType(BidType::Pass);
                            continueBidding();

                        }

                    }

                }
                else {

                    // if (playerAssist) {
                        
                    //     Bid &highestBid = gameRound.getHighestBid();
                    //     Bid &previousBid = gameRound.getBid(playerCurrentlyBidding);
                    //     Bid &partnerBid = gameRound.getBid((playerCurrentlyBidding + 2) % 4);

                    //     Bid retBid = game.players[playerCurrentlyBidding].bid(previousBid, partnerBid, highestBid);
                    //     previousBid.setBid(retBid);
                
                    //     switch (retBid.getBidType()) {
                                            
                    //         case BidType::Pass:
                    //             bidInput.setMode(BidMode::Pass);
                    //             break;
                                            
                    //         case BidType::Misere:
                    //             bidInput.setMode(BidMode::Misere);
                    //             break;
                                            
                    //         case BidType::Partner:
                    //             bidInput.setLevel(retBid.getLevel() - 6);
                    //             bidInput.setSuit(static_cast<uint8_t>(retBid.getSuit()));
                    //             bidInput.setMode(BidMode::Bid);
                    //             break;

                    //     }

                    // }

                    gameState++;

                }

            }

            break;

        case GameState::Bid_Round_Delay:
            {

                if (game.getFrameCount() < 64) {
                    break;
                }

                for (uint8_t i = 0; i < 4; i++) {

                    gameRound.getBid(i).setBidType(BidType::None);

                }

                gameRound.clearKitty();
                game.setFrameCount(0);
                gameState++;
                
            }
            break;

        case GameState::Bid_Round2:
            {
                                             
                bool isHuman = playerCurrentlyBidding == Constants::HumanPlayer;
                uint16_t scores[] = { 0, 0, 0, 0 };
                uint8_t dealer = gameRound.getDealer_Idx();
                uint16_t bid = 0;
                Suit bidSuit = Suit::None;

                if (!isHuman) {
                    
                    if (game.getFrameCount() == 4) { //SJH 32

                        for (uint8_t s = 0; s < 4; s++) {

                            Card noCard;
                            noCard.setSuit(static_cast<Suit>(s));
                            noCard.setRank(Rank::None);
                            scores[s] = game.players[playerCurrentlyBidding].bid(static_cast<Suit>(s), noCard, false);

                        }

                        for (uint8_t s = 0; s < 4; s++) {

                            if (scores[s] > bid) {
                                bid = scores[s];
                                bidSuit = static_cast<Suit>(s);
                            }

                        }

                        if (bid >= 180) {

                            #ifdef DEBUG
                            DEBUG_PRINTLN("Go Alone");
                            #endif
                        
                            completeBid(playerCurrentlyBidding, BidType::Alone, bidSuit);
                            gameState = GameState::Play_Round_Start;

                        }
                        else if (bid >= 150) {
                        
                            #ifdef DEBUG
                            DEBUG_PRINTLN("Go!");
                            #endif
                        
                            completeBid(playerCurrentlyBidding, BidType::Partner, bidSuit);
                            gameState = GameState::Play_Round_Start;

                        }
                        else {

                            uint8_t winningBidIdx = playerCurrentlyBidding;

                            gameRound.getBid(winningBidIdx).setBidType(BidType::Pass);
                            continueBidding();

                        }

                    }

                }
                else {

                    // if (playerAssist) {
                        
                    //     Bid &highestBid = gameRound.getHighestBid();
                    //     Bid &previousBid = gameRound.getBid(playerCurrentlyBidding);
                    //     Bid &partnerBid = gameRound.getBid((playerCurrentlyBidding + 2) % 4);

                    //     Bid retBid = game.players[playerCurrentlyBidding].bid(previousBid, partnerBid, highestBid);
                    //     previousBid.setBid(retBid);
                
                    //     switch (retBid.getBidType()) {
                                            
                    //         case BidType::Pass:
                    //             bidInput.setMode(BidMode::Pass);
                    //             break;
                                            
                    //         case BidType::Misere:
                    //             bidInput.setMode(BidMode::Misere);
                    //             break;
                                            
                    //         case BidType::Partner:
                    //             bidInput.setLevel(retBid.getLevel() - 6);
                    //             bidInput.setSuit(static_cast<uint8_t>(retBid.getSuit()));
                    //             bidInput.setMode(BidMode::Bid);
                    //             break;

                    //     }

                    // }

                    gameState++;

                }

            }

            break;

        case GameState::Bid_Player_Round1:
            {
                uint8_t dealer = gameRound.getDealer_Idx();
                    
                if (justPressed & UP_BUTTON) {
                
                    if (bidInput.getMode() == BidMode::Bid || bidInput.getMode() == BidMode::Alone)   bidInput.decMode();

                }

                else if (justPressed & DOWN_BUTTON) {

                    if (bidInput.getMode() == BidMode::Pass || bidInput.getMode() == BidMode::Bid)    bidInput.incMode();

                }

                else if (justPressed & A_BUTTON) {

                    if (bidInput.getMode() == BidMode::Bid || bidInput.getMode() == BidMode::Alone) {

                        game.players[dealer].addCard(gameRound.getKitty());
                        game.players[dealer].getCard(5).setSelected(true);

                        completeBid(1, bidInput.getBidType(), gameRound.getKitty()->getSuit());
                        gameState = GameState::Handle_Kitty;

                    }

                    else if (bidInput.getMode() == BidMode::Pass) {

                        gameRound.getBid(Constants::HumanPlayer).setBidType(BidType::Pass);
                        gameState = GameState::Bid_Round1;
                        continueBidding();

                    }

                }

            }

            break;

        case GameState::Bid_Player_Round2:
            {

                if (game.getFrameCount() < 64) {
                    break;
                }
                        
                uint8_t dealer = gameRound.getDealer_Idx();
                        
                if (justPressed & UP_BUTTON) {
                
                    if (bidInput.getMode() == BidMode::SuitSelect)          bidInput.incSuit();
                    else if (bidInput.getMode() > BidMode::Pass)            bidInput.decMode();

                }

                else if (justPressed & DOWN_BUTTON) {

                    if (bidInput.getMode() == BidMode::SuitSelect)          bidInput.decSuit();
                    else if (bidInput.getMode() < BidMode::Alone)           bidInput.incMode();

                }

                else if (justPressed & LEFT_BUTTON && bidInput.getMode() != BidMode::SuitSelect) {
                
                    bidInput.setMode(BidMode::SuitSelect);

                }

                else if ((justPressed & RIGHT_BUTTON) && bidInput.getMode() == BidMode::SuitSelect) {
                
                    bidInput.setMode(bidInput.getPrevMode());

                }

                else if (justPressed & A_BUTTON) {

                    if (bidInput.getMode() == BidMode::Bid || bidInput.getMode() == BidMode::Alone) {

                        completeBid(1, bidInput.getBidType(), bidInput.getSuit());
                        gameState = GameState::Play_Round_Start;

                    }

                    else if (bidInput.getMode() == BidMode::Pass) {

                        gameRound.getBid(Constants::HumanPlayer).setBidType(BidType::Pass);
                        gameState = GameState::Bid_Round2;
                        continueBidding();

                    }

                }

            }

            break;

        case GameState::Bid_Failed:
            
            if (game.getFrameCount() % 4 > 0) return;

            switch (game.players[0].getCardCount() > 0) {

                case 1 ... 5:

                    for (uint8_t i = 0; i < 4; i++) {
                        game.players[i].getCard(game.players[i].getCardCount() - 1).reset();
                        game.players[i].setCardCount(game.players[i].getCardCount() - 1);
                    }

                    break;

                case 0:
                    dealPhysics.x = 0;
                    dealPhysics.x2 = 0;
                    dealPhysics.y = 0;
                    dealPhysics.y2 = 0;
                    gameState = GameState::Play_Shuffle;
                    break;
            
            }
            
            break;

        case GameState::Handle_Kitty:
            {
                uint8_t dealer = gameRound.getDealer_Idx();

                if (dealer == Constants::HumanPlayer) {

                    if (justPressed > 0) {
                    
                        if (kittyHighlight > 0) {
                            kittyHighlight = 0;
                            game.players[Constants::HumanPlayer].clearSelection();
                        }

                    }

                    if (justPressed == LEFT_BUTTON && selectedCard > 0) {

                        selectedCard--;

                    }

                    if (justPressed == RIGHT_BUTTON && selectedCard < game.players[Constants::HumanPlayer].getCardCount() - 1) {

                        selectedCard++;

                    }

                    if (justPressed == UP_BUTTON) {

                        game.players[Constants::HumanPlayer].clearSelection();
                        game.players[Constants::HumanPlayer].getCard(selectedCard).setSelected(true);
                    
                    }

                    if (justPressed == DOWN_BUTTON && game.players[Constants::HumanPlayer].getCard(selectedCard).isSelected()) {

                        game.players[Constants::HumanPlayer].getCard(selectedCard).setSelected(false);
                    
                    }

                    if (justPressed == A_BUTTON && game.players[Constants::HumanPlayer].getSelectedCount() == 1) {

                        gameRound.setRound(0);
                        gameRound.clearKitty();

                        uint8_t kittyIndex = 0;

                        for (uint8_t i = 0; i < game.players[Constants::HumanPlayer].getCardCount(); i++) {

                            if (game.players[Constants::HumanPlayer].getCard(i).isSelected()) {

                                game.players[Constants::HumanPlayer].getCard(i).reset();
                                game.players[Constants::HumanPlayer].sort();
                                game.players[Constants::HumanPlayer].setCardCount(5);
                                kittyIndex++;

                            }

                        }

                        if (!playerAssist) highlightSuitInHand();
                        gameState++;

                    }


                    if (kittyHighlight > 0) {
                        
                        kittyHighlight--;
                        
                        if (kittyHighlight == 0) {
                            game.players[Constants::HumanPlayer].clearSelection();
                        }

                    }

                }
                else {

                    if (game.getFrameCount() % 64 == 0) {

                        gameRound.setRound(0);
                        gameRound.clearKitty();
                        game.players[dealer].handleKitty();
                        if (!playerAssist) highlightSuitInHand();
                        gameState++;

                    }

                }

            }

            break;

        case GameState::Play_Round_Start:
            {

                if (game.getFrameCount() % 64 != 0) {
                    break;
                }
            
                gameRound.incRound();

                Bid bid = gameRound.getWinningBid();

                #ifdef DEBUG_BASIC
                    DEBUG_PRINTLN(F("\n-- Start of hand -----------------"));     
                    DEBUG_PRINT(F("Bid: "));
                    DEBUG_PRINT_BID(bid.getBidType(), bid.getSuit());
                    DEBUG_PRINTLN();

                    DEBUG_PRINT(F("Bid Winner: "));
                    DEBUG_PRINTLN(gameRound.getWinningBid_Idx());
                    DEBUG_PRINT(F("Starting Hand: "));
                    DEBUG_PRINTLN(gameRound.getCurrentPlayer_Idx());
                    DEBUG_PRINT(F("Round: "));
                    DEBUG_PRINTLN(gameRound.getRound());
                    DEBUG_PRINT_HANDS();
                #endif
    

                // Select card in players hand ..

                game.setFrameCount(0);
                gameState++;

            }

            break;

        case GameState::Play_Hand:
            {
                bool isHuman = gameRound.getCurrentPlayer_Idx() == Constants::HumanPlayer;

                if (game.getFrameCount() == 32 || isHuman) {

                    if (gameRound.getFirstPlayer_Idx() == gameRound.getCurrentPlayer_Idx()) {
                        game.players[gameRound.getCurrentPlayer_Idx()].playSuit_Lead(isHuman);
                    }
                    else {
                        game.players[gameRound.getCurrentPlayer_Idx()].playSuit_Follow(isHuman);
                    }

                    #ifdef DEBUG_BASIC
                        DEBUG_PRINT(F("99. Player "));
                        DEBUG_PRINT(gameRound.getCurrentPlayer_Idx());
                        DEBUG_PRINT(F(" played "));
                    #endif

                    Card &card = game.players[gameRound.getCurrentPlayer_Idx()].cardJustPlayed;

                    #ifdef DEBUG_BASIC

                        DEBUG_PRINT_CARD(card.getSuit(), card.getRank());
                        DEBUG_PRINTLN();

                    #endif

                    if (!isHuman) {

                        play_CardSelected();

                    }
                    else {


                        if (playerAssist) {
                            
                            // Highlight the previously 'played' card ..

                            game.players[Constants::HumanPlayer].clearSelection();

                            for (uint8_t i = 0; i < game.players[Constants::HumanPlayer].getCardCount(); i++) {
                            
                                if (game.players[Constants::HumanPlayer].getCard(i).getSuit() == game.players[Constants::HumanPlayer].getCardJustPlayed().getSuit() &&
                                    game.players[Constants::HumanPlayer].getCard(i).getRank() == game.players[Constants::HumanPlayer].getCardJustPlayed().getRank()) {

                                    game.players[Constants::HumanPlayer].getCard(i).setSelected(true);
                                    selectedCard = i;
                                    break;

                                }

                            }

                        }

                        gameState = GameState::Play_PlayerInput;

                    }

                }

            }

            break;

        case GameState::Play_PlayerInput:
            {

                if (justPressed == LEFT_BUTTON && selectedCard > 0) {
                    selectedCard--;
                    game.players[Constants::HumanPlayer].clearSelection();
                    game.players[Constants::HumanPlayer].getCard(selectedCard).setSelected(true);

                }

                if (justPressed == RIGHT_BUTTON && selectedCard < game.players[gameRound.getCurrentPlayer_Idx()].getCardCount() - 1) {
                    selectedCard++;
                    game.players[Constants::HumanPlayer].clearSelection();
                    game.players[Constants::HumanPlayer].getCard(selectedCard).setSelected(true);

                }

                if (justPressed == A_BUTTON && game.players[gameRound.getCurrentPlayer_Idx()].getCard(selectedCard).isSelected()) {

                    Suit trumps = gameRound.winningBid_Suit();
                    Card cardLed = gameRound.getCardLed();
                    Bid bid = gameRound.getWinningBid();

                    Card &cardPlayed = game.players[gameRound.getCurrentPlayer_Idx()].getCard(selectedCard);

                    switch (bid.getBidType()) {
                    
                        case BidType::Partner:

                            if (cardLed.getSuit() != cardPlayed.getSuit() && game.players[gameRound.getCurrentPlayer_Idx()].hasSuit(cardLed.getSuit())) {

                                return;

                            }

                            break;
                    
                    }

                    game.players[gameRound.getCurrentPlayer_Idx()].playCard(selectedCard, false);
                    play_CardSelected();

                }

            }

            break;

        case GameState::Play_EndOfHand:

            if (gameRound.getRound() == 5) {

                uint8_t biddingTeam = gameRound.getWinningBid_Team();
                uint8_t winningTeam = gameRound.getTeam_TrickCount(0) > 0 ? 0 : 1;
                int16_t winningScore_BidTeam = gameRound.getWinningScore_BidTeam(gameRound.getTeam_TrickCount(biddingTeam));
                uint8_t winningScore_Alt = gameRound.getWinningScore_AltTeam(gameRound.getTeam_TrickCount(biddingTeam));

                #ifdef DEBUG_BASIC
                    DEBUG_PRINT(F("Winning Bid team "));
                    DEBUG_PRINTLN(biddingTeam);
                    DEBUG_PRINT(F("Winning team "));
                    DEBUG_PRINTLN(winningTeam);
                    DEBUG_PRINT(F("Winning score (Bid) "));
                    DEBUG_PRINTLN(winningScore_BidTeam);
                    DEBUG_PRINT(F("Winning score (Alt) "));
                    DEBUG_PRINTLN(winningScore_Alt);
                #endif

                gameRound.incScore(biddingTeam, winningScore_BidTeam, false);
                gameRound.incScore(biddingTeam == 1 ? 0 : 1, winningScore_Alt, true);

                gameState = GameState::Play_EndOfRound;

            }
            else {

                if (justPressed & A_BUTTON) {
                
                    gameRound.resetHand();
                    gameState = GameState::Play_Round_Start;

                }

            }

            break;          

        case GameState::Play_EndOfRound:

            if ((gameRound.getScore(0) <= -500 || gameRound.getScore(0) >= 500) ||
                (gameRound.getScore(1) <= -500 || gameRound.getScore(1) >= 500)) {  

                gameState = GameState::Play_EndOfGame;
                endOfGame_Y = 0;

            }
            else {

                if (justPressed & A_BUTTON) {

                    gameState = GameState::Play_Shuffle;

                }

            }

            break;            

        case GameState::Play_EndOfGame:

            if (game.getFrameCount() > 120 && game.getFrameCount() < 120 + 64) {

                endOfGame_Y++;

            }

            if (justPressed & A_BUTTON && endOfGame_Y > 60) {

                gameState = GameState::Title_Init;

            }

            break;       
    }

}


void play(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {

    uint8_t currentPlane = a.currentPlane();
    if (a.needsUpdate()) play_Update();

    switch (gameState) {

        case GameState::Play_Deal_00 ... GameState::Play_Deal_20:

            renderPlayerHands(currentPlane, false, false);
            renderKitty(currentPlane);
            renderDealer(currentPlane);
            renderHUD(currentPlane, true, false);

            if (dealPhysics.x > dealPhysics.x2 && dealPhysics.y < dealPhysics.y2) {
                SpritesU::drawPlusMaskFX((dealPhysics.x / 16)  - 5, (dealPhysics.y / 16) - 4, Images::Rotate, ((static_cast<uint8_t>((dealPhysics.rot / 16)) % 36) * 3) + currentPlane);
            }

            break;

        case GameState::Bid_Round1:
        case GameState::Bid_Player_Round1:

            renderKitty(currentPlane);
            renderPlayerHands(currentPlane, false, false);
            renderHUD(currentPlane, false, false);

            if (playerCurrentlyBidding == Constants::HumanPlayer) {

                SpritesU::drawPlusMaskFX(45, 17, Images::Bid_Round1, (static_cast<uint8_t>(bidInput.getMode()) * 3) + currentPlane - 3);
            
            }
           
            renderBids(currentPlane);

            break;

        case GameState::Bid_Round_Delay:

            // renderKitty(currentPlane);
            renderPlayerHands(currentPlane, false, false);
            renderHUD(currentPlane, false, false);

            if (playerCurrentlyBidding == Constants::HumanPlayer) {

                SpritesU::drawPlusMaskFX(45, 17, Images::Bid_Round1, (static_cast<uint8_t>(bidInput.getMode()) * 3) + currentPlane - 3);
            
            }
           
            renderBids(currentPlane);

            break;

        case GameState::Bid_Round2:
        case GameState::Bid_Player_Round2:

            renderKitty(currentPlane);
            renderPlayerHands(currentPlane, false, false);
            renderHUD(currentPlane, false, false);

            if (playerCurrentlyBidding == Constants::HumanPlayer) {

                SpritesU::drawPlusMaskFX(29, 17, Images::Bid_Round2, (static_cast<uint8_t>(bidInput.getMode()) * 3) + currentPlane);

                if (bidInput.getMode() == BidMode::SuitSelect) {
                    SpritesU::drawOverwriteFX(34, 25, Images::Bid_Suits, (static_cast<uint8_t>(bidInput.getSuit()) * 3) + currentPlane);
                }
                else {
                    SpritesU::drawOverwriteFX(34, 25, Images::Bid_Suits, ((static_cast<uint8_t>(bidInput.getSuit()) + 4) * 3) + currentPlane);
                }
            
            }
           
            renderBids(currentPlane);

            break;

        case GameState::Bid_Failed:

            SpritesU::drawOverwriteFX(34, 19, Images::EveryonePassed, currentPlane);
            renderPlayerHands(currentPlane, false, false);
            renderBids(currentPlane);
            renderHUD(currentPlane, false, false);

            break;

        case GameState::Handle_Kitty:

            if (gameRound.getDealer_Idx() == Constants::HumanPlayer) {
                SpritesU::drawOverwriteFX(21, 15, Images::KittyInstructions, currentPlane);
                renderPlayerHands(currentPlane, true, false);
            }
            else {
                renderDealer(currentPlane);
                renderPlayerHands(currentPlane, false, false);
            }

            renderHUD(currentPlane, false, true);
            renderBids(currentPlane);
            renderDiscard(currentPlane);

            break;

        case GameState::Play_Round_Start:

            renderPlayerHands(currentPlane, false, false);
            renderTableCards(currentPlane, Constants::NoWinner);
            renderHUD(currentPlane, false, true);

            if (gameRound.getRound() == 0) {
                renderBids(currentPlane);
            }
            // renderDiscard(currentPlane);

            break;

        case GameState::Play_Hand ... GameState::Play_PlayerInput:

            renderPlayerHands(currentPlane, false, false);
            renderTableCards(currentPlane, Constants::NoWinner);
            renderHUD(currentPlane, false, true);

            break;


        case GameState::Play_EndOfHand ... GameState::Play_EndOfRound:

            renderPlayerHands(currentPlane, false, false);
            renderTableCards(currentPlane, gameRound.getWinningHand());
            renderHUD(currentPlane, false, true);

            break;

        case GameState::Play_EndOfGame:

            if (endOfGame_Y > 0) {

                renderTableCards(currentPlane, Constants::NoWinner, endOfGame_Y);

                uint8_t idx = 0;
                if ((game.getFrameCount() % 140) < 64) idx = game.getFrameCount() % 140 / 2;

                if (gameRound.getScore(Constants::HumanTeam) >= 500 || gameRound.getScore(Constants::BotTeam) <= 500) {
                    SpritesU::drawOverwriteFX(16, endOfGame_Y - 55, Images::You_Won, (idx * 3) + currentPlane);
                }
                else {
                    SpritesU::drawOverwriteFX(16, endOfGame_Y - 55, Images::You_Lost, (idx * 3) + currentPlane);
                }

            }
            else {
                renderTableCards(currentPlane, gameRound.getWinningHand(), endOfGame_Y);
            }

            renderHUD(currentPlane, false, true);


            break;

    }

}
