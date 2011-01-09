#ifndef CPLAYER_H
#define CPLAYER_H

#include <QThread>
#include <QTimer>
#include <QCoreApplication>
class StateMon;
class Berry;
/**
  Queries the game state repeatedly to see if it is the computer's
  turn to move. Basically simulating what a human player would do too
**/
class CPlayer : public QThread {
    Q_OBJECT
    QCoreApplication &qCore;
    StateMon &state;
    Berry &berry;
    QTimer timer;
public:
    CPlayer (QCoreApplication &qc, StateMon &st, Berry &ber);
public slots:
    void makeTurn ();
};

#endif // CPLAYER_H
