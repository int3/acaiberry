#include "StateMon.h"
#include "PlayerIn.h"

using namespace std;

StateMon::StateMon (Board &b, Dict &d) : bd(b), dict(d), _roundOn(false) {
}
void StateMon::beginRound () {
    _turnCounter = 0;
    passCount = 0;
    for (uint i=0; i<turnList.size(); ++i) {
        delete turnList[i];
    }
    turnList.clear();
    _roundOn = true;
}
void StateMon::endRound () {
    _roundOn = false;
    emit roundOver();
}
StateMon::~StateMon () {
    for (uint i=0; i<turnList.size(); i++)
        delete turnList[i];
}
void StateMon::regPlayerIn (PlayerIn &pin) {
    _playIn[pin.id()] = &pin;
}
void StateMon::playerDone (int pid, TurnInfo &turn) {
    if (turn.type() != PASS) passCount = 0;
    if (turn.type() == MOVE) {
        MoveInfo *mv = new MoveInfo(dynamic_cast<MoveInfo&>(turn));
        turnList.push_back(mv);
        emit moveAdded(pid, *mv);
    } else if (turn.type() == PASS) {
        PassInfo *ps = new PassInfo(dynamic_cast<PassInfo&>(turn));
        turnList.push_back(ps);
        emit passAdded(pid);
        if (++passCount == 3) {
            _playIn[0]->adjustScore(_playIn[1]->rack().totalValue());
            _playIn[1]->adjustScore(_playIn[0]->rack().totalValue());
            endRound();
            return;
        }
    }
}
int StateMon::turnCounter () {
    return _turnCounter;
}
void StateMon::handover () {
    if (!_roundOn)
        return;
    _turnCounter ^= 1;
    _playIn[_turnCounter]->activateTurn();
}
void StateMon::playerRackEmpty (int id) {
    _playIn[id^1]->adjustScore(_playIn[id]->rack().totalValue());
    endRound();
}
void StateMon::displayMoveList () {
    for (uint i=0; i<turnList.size(); i++)
        printf("%d: %s\n", i, turnList[i]->displayText().c_str());
}
bool StateMon::roundOn () {
    return _roundOn;
}
