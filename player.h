#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <tuple>
#include <cstdlib>

#include "common.h"
#include "board.h"

#define MINIMAX_DEPTH 0

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board board_rep;
    Side my_side;
    Side op_side;

private: 
    std::vector<Move*> *findValidMoves(bool my_side);
    double moveScore(Move*);
    bool isCorner(Move*);
    bool isAroundCorner(Move*);
    std::tuple<Move*, int> minimaxTree(std::vector<Move*>, int);
};

#endif
