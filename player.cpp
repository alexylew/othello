#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // Seed our random number generator
    srand (static_cast <unsigned> (time(0)));

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    my_side = side;
    op_side = Side((side + 1) % 2);

    std::cerr << "myside: " << my_side << std::endl;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) 
{
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    
    if (opponentsMove != NULL)
    {
        std::cerr << "updating opponents move: " << opponentsMove->x << ", " << opponentsMove->y << std::endl;
        board_rep.doMove(opponentsMove, op_side);
    }

    std::vector<std::tuple<Move*, double>> *moves = findValidMoves();

    if (moves->size() > 0)
    {
        std::tuple<Move*, double> best = (*moves)[0];
        
        for (unsigned int i = 1; i < moves->size(); i++)
        {
            if (std::get<1>((*moves)[i]) > std::get<1>(best))
                best = (*moves)[i];
        }
        
        board_rep.doMove(std::get<0>(best), my_side);
        return std::get<0>(best);
    }
    else    
        return NULL;
}

std::vector<std::tuple<Move*, double>> *Player::findValidMoves()
{
    std::vector<std::tuple<Move*, double>> *moves = new std::vector<std::tuple<Move*, double>>();
    
    for (int x = 0; x < 8; x++) 
    {
        for (int y = 0; y < 8; y++)
        {
            Move *curr = new Move(x, y);
            if (board_rep.checkMove(curr, my_side))
                moves->push_back(std::make_tuple(curr, moveScore(curr)));
        }
    }

    return moves;
}

double Player::moveScore(Move *move)
{
    if (isCorner(move))
        return 2.0;
    return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

bool Player::isCorner(Move *move)
{
    if (move->x == 0 and move->y == 0)
        return true;
    else if (move->x == 7 and move->y == 0)
        return true;
    else if (move->x == 0 and move->y == 7)
        return true;
    else if (move->x == 7 and move->y == 7)
        return true;
    return false;
}

Move *Player::minimaxTree(std::vector<Move*> moves, int depth)
{
    Move *best = NULL;
    if (depth > 0)
    {
        return best;
    }

    else if (depth == 0)
    {
        int min_seen = MIN_DIFF;
        for (auto it = begin (moves); it != end (moves); it++) 
        {
            Board *whatIf = board_rep.copy();
            whatIf->doMove(*it, op_side);
            if ((whatIf->count(my_side) - whatIf->count(op_side) > min_seen))
                best = *it;
        }
    }
    else
        std::cerr << "you screwed up the minimax pretty badly" << std::endl;

    return best;
}
