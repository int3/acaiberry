#ifndef STATEMON_H
#define STATEMON_H

class PlayerIn;
class Board;
class Dict;
#include "TurnInfo.h" // full declaration necessary for x-Infos to be emitted in queued connections
#include <vector>
#include <QObject>

/**
  Monitors state of the game, e.g. checks if the game
  should be terminated, whose turn it is
**/

class StateMon : public QObject {
    Q_OBJECT
    PlayerIn *_playIn[2]; /// _playIn[0] always starts first.
    Board &bd;
    Dict &dict;
    int turnCounter;
    void setTurn (int id, bool isTurn);
    std::vector<TurnInfo*> turnList;
    int passCount;
    bool _roundOn;
public:
    StateMon (Board &b, Dict &d);
    void beginRound ();
    void endRound ();
    ~StateMon ();
    void regPlayerIn (PlayerIn &pin); /// register Player interface
    void playerDone (int id, TurnInfo &move);
    void playerRackEmpty (int id);
    void displayMoveList ();
    bool roundOn ();
    void handover ();
signals:
    void illegalMove (std::vector<std::string> illegals);
    void moveAdded (int pid, MoveInfo turn);
    void passAdded (int pid);
    void turnOver ();
    void roundOver ();
};

#endif // STATEMON_H
