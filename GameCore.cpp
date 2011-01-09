#include "GameCore.h"
#include "Berry.h"
#include "PlayerIn.h"

GameCore::GameCore (Dict &d) : dict(d), bd(dict), _state(bd, dict) {
    for (int i=0; i<2; i++) {
        _pin[i] = new PlayerIn(bd, dict, tb, i, _state);
        _state.regPlayerIn(*_pin[i]);
    }
    _state.beginRound();
}
GameCore::~GameCore () {
    for (int i=0; i<2; i++) {
        delete _pin[i];
     }
}
void GameCore::reset () {
    bd.reset();
    tb.reset();
    for (int i=0; i<2; i++) {
        _pin[i]->reset();
    }
    _state.beginRound();
}
PlayerIn& GameCore::pin (int n) {
    return *(_pin[n]);
}
StateMon& GameCore::state () {
    return _state;
}
Board& GameCore::board () {
    return bd;
}
Simulator::Simulator (GameCore &g, Dict &d) : gc(g), dict(d) {
    for (int i=0; i<2; i++)
        cp[i] = new Berry(gc.pin(i), gc.board(), dict);
}
Simulator::~Simulator () {
    for (int i=0; i<2; i++) {
        delete cp[i];
    }
}
void Simulator::benchmark (int rounds) {
    for (int i=0; i<rounds; i++) {
        gc.reset();
        bool roundOn = true;
        for (int i=0; i<30; i++) {
            for (int j=0; j<2; j++) {
                cp[j]->makeTurn();
                if (!(roundOn = gc.state().roundOn())) break;
            }
            if (!roundOn) break;
        }
    }
    printf("Simulated %d games.\n", rounds);
}
