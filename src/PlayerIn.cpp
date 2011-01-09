#include "PlayerIn.h"
#include "StateMon.h"

PlayerIn::PlayerIn (Board &b, Dict &d, TileBag &t, int iden, StateMon &st) : bd(b), tb(t), _id(iden), state(st), dict(d) {
    reset();
}

void PlayerIn::reset () { /// must only be called after TileBag has been reset!
    _score = 0;
    _rack.empty();
    _rack.replenish(tb);
    emit modified();
}

void PlayerIn::archive () {
    previous.score = _score;
    previous.rack = _rack;
}

void PlayerIn::revert () { /// not meant to be called if archive () is not called at least once. uninitialized variables!
    _score = previous.score;
    _rack = previous.rack;
    emit modified();
}

void PlayerIn::doMove (MoveInfo mv) {
    if (!_id == state.turnCounter()) return;
    //archive();
    mv.calcLegal(bd, dict); // add some check here to see if player is computer or human
    if (!mv.isLegal()) {
        emit illegalMove (mv.getIllegals());
        return;
    }
    _score += mv.score();
    bd.placeTiles(mv);
    _rack.subtract(mv.usedTiles());
    _rack.replenish(tb);
    state.playerDone(_id, mv);
    if (_rack.numTiles() == 0) state.playerRackEmpty(_id);
    emit modified();
    state.handover();
}

void PlayerIn::doPass () {
    if (!_id == state.turnCounter()) return;
    PassInfo tmp;
    state.playerDone(_id, tmp);
    state.handover();
}
int PlayerIn::id () const {
    return _id;
}
const Rack& PlayerIn::rack () const {
    return _rack;
}
void PlayerIn::adjustScore (int amt) {
    _score += amt;
}
int PlayerIn::score () const {
    return _score;
}
void PlayerIn::activateTurn() const {
    emit turnActivated();
}
