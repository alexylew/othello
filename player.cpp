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
    // Process opponent's move
    if (opponentsMove != NULL)
        board_rep.doMove(opponentsMove, op_side);
    
    // Find list of possible moves
    std::vector<Move*> *moves = board_rep.findValidMoves(my_side);
    
    // Implementation of minimiax tree
    // Find best possible move 
    Move* best;
    if (moves->size() > 0)
    { 
        best = std::get<0>(*(minimaxTree(*moves, MINIMAX_DEPTH)));
        board_rep.doMove(best, my_side);
        return best;
    }

    // If there are no possible moves, pass
    else
        return NULL;
    

    
    // Implementation of corners heuristic
    /*
    // Find all possible moves
    std::vector<std::tuple<Move*, double>> moves_scored;

    // For each move, find the score based on the heuristic
    // Heuristic is: 
        // Always take corners
        // Never take spaces adjacent to corners
        // Otherwise, choose randomly
    for (auto it = begin (*moves); it != end (*moves); it++) 
    {
        moves_scored.push_back(std::make_tuple(*it, moveScore(&board_rep, *it)));
    }
    
    // If we have any moves, make the best one
    if (moves_scored.size() > 0)
    {
        std::tuple<Move*, double> best = moves_scored[0];
        
        for (unsigned int i = 1; i < moves_scored.size(); i++)
        {
            if (std::get<1>(moves_scored[i]) > std::get<1>(best))
                best = moves_scored[i];
        }
        
        board_rep.doMove(std::get<0>(best), my_side);
        return std::get<0>(best);
    }
    
    // Otherwise, pass
    else    
        return NULL;
    */
}

double Player::moveScore(Board *board, Move *move)
{
    double score = 0.0;

    // Corners heuristic as described above
    if (isCorner(move))
        score -= 1000.0;
    else if (isAroundCorner(move))
        score += 12.0;
    
    // Coin parity heuristic
    score -= (board->count(op_side) - board->count(my_side));

    return score;
}

// Determines if move is a corner
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

// Determines if move is adjacent to a corner
bool Player::isAroundCorner(Move *move)
{
    if (move->x == 0 and move->y == 1)
        return true;
    else if (move->x == 7 and move->y == 1)
        return true;
    else if (move->x == 1 and move->y == 7)
        return true;
    else if (move->x == 6 and move->y == 7)
        return true;
    else if (move->x == 1 and move->y == 0)
        return true;
    else if (move->x == 6 and move->y == 0)
        return true;
    else if (move->x == 0 and move->y == 6)
        return true;
    else if (move->x == 7 and move->y == 6)
        return true;
    else if (move->x == 1 and move->y == 1)
        return true;
    else if (move->x == 6 and move->y == 1)
        return true;
    else if (move->x == 1 and move->y == 6)
        return true;
    else if (move->x == 6 and move->y == 6)
        return true;
    return false;
}

std::tuple<Move*, int> *Player::minimaxTree(std::vector<Move*> moves, int depth)
{
    std::cerr << "curr depth: " << depth << std::endl;
    std::tuple<Move*, int> *best = new tuple<Move*, int>();
    std::tuple<Move*, int> temp;
    std::vector<std::tuple<Move*, int>> move_values;
    
    // If this isn't the last depth, we need to recursively call to trace the whole tree
    if (depth > 0)
    {        
        for (auto it = begin (moves); it != end (moves); it++)
        {
            // Do each move passed in
            Board *whatIf = board_rep.copy();
            whatIf->doMove(*it, my_side);
            
            // Find the list of moves that opponent can make
            std::vector<Move*> *op_moves = whatIf->findValidMoves(op_side);
            
            // For each of these moves, make the move 
            // Then pass the possible moves from there back to minimax recursive call
            std::vector<std::tuple<Move*, int>> op_move_values;
            for (auto it2 = begin (*op_moves); it2 != end (*op_moves); it2++)
            {
                Board *thatsWhat = whatIf->copy();
                thatsWhat->doMove(*it2, op_side);
                op_move_values.push_back(*(minimaxTree(*(thatsWhat->findValidMoves(my_side)), depth - 1)));
            }
            
            // Now, what we're interested in is the minimum value from the above set of moves
            // since it represents the result of each of the opponents possible moves
            // So, we push back the minimum move/value pair from each of the opponents moves 
            // onto our overall tracker
            move_values.push_back(
                std::make_tuple(
                    *it, 
                    std::get<1>(*(std::min_element(op_move_values.begin(), op_move_values.end(),
                    [] (std::tuple<Move*, int> const& first, std::tuple<Move*, int> const& second) 
                    {return std::get<1>(first) < std::get<1>(second);})))
                    ));
        }
        
        // Set the best move equal to the move with the maximum minimum score
        temp = *(std::max_element(move_values.begin(), move_values.end(),
            [] (std::tuple<Move*, int> const& first, std::tuple<Move*, int> const& second) 
                {return std::get<1>(first) < std::get<1>(second);} ));
    }

    // If this is the last depth, then no need to recurisvely call
    // just solve each of the subtrees
    else if (depth == 0)
    {
        for (auto it = begin (moves); it != end (moves); it++) 
        {
            // Initialize the min possible value of this move
            int min_seen = MIN_DIFF;

            // Do each move passed in
            Board *whatIf = board_rep.copy();
            whatIf->doMove(*it, my_side);
            
            // Find the list of moves that opponent can make
            std::vector<Move*> *op_moves = whatIf->findValidMoves(op_side);

            // Update the minimum score based on the outcome of each of those moves
            for (auto it2 = begin (*op_moves); it2 != end (*op_moves); it2++)
            {
                Board *thatsWhat = whatIf->copy();
                if ((moveScore(thatsWhat, *it2) < min_seen))
                    min_seen = moveScore(thatsWhat, *it2);
            }    
                 
        move_values.push_back(std::make_tuple(*it, min_seen));
        }
        
        temp = *(std::max_element(move_values.begin(), move_values.end(),
            [] (std::tuple<Move*, int> const& first, std::tuple<Move*, int> const& second) 
                {return std::get<1>(first) < std::get<1>(second);} ));   
    }

    else
        std::cerr << "you screwed up the minimax pretty badly" << std::endl;

    std::get<0>(*best) = std::get<0>(temp);
    std::get<1>(*best) = std::get<1>(temp);

    return best;
}
