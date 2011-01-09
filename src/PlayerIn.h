#ifndef PLAYERIN_H
#define PLAYERIN_H

#include "Board.h"
#include "TileRepo.h"
#include <QObject>

class StateMon;
/**
  @brief Player Interface. Used by both human and computer players.

  Keeps track of score, tiles on the rack.
**/
class PlayerIn : public QObject {
    Q_OBJECT
    Board &bd;
    TileBag &tb;
    int _score;
    Rack _rack;
    int _id;
    StateMon &state;
    Dict &dict;
    struct old {
        int score;
        Rack rack;
    } previous;
public:
    PlayerIn (Board &b, Dict &d, TileBag &t, int iden, StateMon &st);
    void reset ();
    void archive ();
    void revert ();
    int id () const;
    const Rack& rack () const;
    void adjustScore (int amt);
    int score () const;
    void activateTurn() const;
public slots:
    void doMove (MoveInfo mv);
    void doPass ();
signals:
    void modified();
    void illegalMove (std::vector<std::string>);
    void turnActivated() const;
};

#endif // PLAYERIN_H
