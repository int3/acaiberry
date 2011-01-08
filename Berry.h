#ifndef BERRY_H
#define BERRY_H

#include "PlayerIn.h"
#include <vector>
/**
  The AI
**/
class Berry {
    PlayerIn &pin;
    Board &bd;
    Dict &dict;
public:
    Berry (PlayerIn &p, Board &b, Dict &d);
    std::vector<MoveInfo> genMoves (); ///< generate all possible moves
    void makeTurn (); ///< choose the best move and play it
};

#endif // BERRY_H
