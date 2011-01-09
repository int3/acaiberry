#ifndef GAMECORE_H
#define GAMECORE_H

#include "Dict.h"
#include "Board.h"
#include "TileRepo.h"
#include "StateMon.h"

class PlayerIn;
class Berry;

/**
  Encompasses all non-GUI aspects of the game.
**/
class GameCore {
    Dict &dict;
    Board bd;
    TileBag tb;
    StateMon _state;
    PlayerIn *_pin[2];
public:
    GameCore (Dict &d);
    ~GameCore ();
    void reset ();
    PlayerIn& pin(int n);
    StateMon& state();
    Board& board();
};

/**
  Simulates game with two computer players
  for testing / benchmark purposes.
**/
class Simulator {
    GameCore &gc;
    Berry *cp[2];
    Dict &dict;
public:
    Simulator (GameCore &g, Dict &d);
    ~Simulator ();
    void benchmark (int rounds);
};

#endif // GAMECORE_H
