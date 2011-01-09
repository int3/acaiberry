#ifndef PLAYERIN_H
#define PLAYERIN_H

#include "Board.h"
#include "TileRepo.h"
#include <QObject>

class StateMon;
/**
  Generic interface that both human and computer players use
**/
class PlayerIn : public QObject { // Player Interface
    Q_OBJECT
    Board &bd;
    TileBag &tb;
    int _score;
    Rack _rack;
    int _id;
    bool _isMyTurn;
    StateMon &state;
    Dict &dict;
    struct old {
        int score;
        Rack rack;
        bool isMyTurn;
    } previous;
public:
    PlayerIn (Board &b, Dict &d, TileBag &t, int iden, StateMon &st);
    void reset ();
    void archive ();
    void revert ();
    int id () const;
    void setTurn(bool isTurn);
    void displayRack ();
    const Rack& rack () const;
    void adjustScore (int amt);
    bool isMyTurn () const;
    int score () const;
public slots:
    void doMove (MoveInfo mv);
    void doPass ();
signals:
    void modified();
    void illegalMove (std::vector<std::string>);
};

#endif // PLAYERIN_H
