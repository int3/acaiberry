#ifndef BERRY_H
#define BERRY_H

#include "PlayerIn.h"
#include <vector>
#include <QObject>
/**
  The AI
**/
class Berry : public QObject {
    Q_OBJECT
    PlayerIn &pin;
    Board &bd;
    Dict &dict;
public:
    Berry (PlayerIn &p, Board &b, Dict &d);
    std::vector<MoveInfo> genMoves (); ///< generate all possible moves
public slots:
    void makeTurn (); ///< choose the best move and play it
};

#endif // BERRY_H
