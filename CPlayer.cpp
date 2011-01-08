#include "CPlayer.h"
#include "Berry.h"
#include "StateMon.h"

CPlayer::CPlayer (QCoreApplication &qc, StateMon &st, Berry &ber) : qCore(qc), state(st), berry(ber) {
    connect(&qc, SIGNAL(aboutToQuit()), this, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(makeTurn()));
    timer.start(100);
}
void CPlayer::makeTurn () {
    if (state.roundOn())
        berry.makeTurn();
}
