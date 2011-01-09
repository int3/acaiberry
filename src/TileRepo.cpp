#include "TileRepo.h"
#include <cstdlib>
#include <cmath>

using namespace std;

TileRepo::TileRepo () : _numTiles(0) {
    for (int i=0; i<CHARNUM; i++)
        tiles[i] = 0;
}
TileRepo::TileRepo (std::string str) : _numTiles(0) {
    for (int i=0; i<CHARNUM; i++)
        tiles[i] = 0;
    for (uint i=0; i<str.size(); i++) {
        if (isalpha(str[i]) || str[i] == '?') {
            tiles[toindex2(str[i])]++;
            _numTiles++;
        }
        else
            throw bad_input("TileRepo::(string)");
    }
}
bool TileRepo::hasTile (int i) const { ///< @param i The tile letter in INDEX format
    return tiles[i] > 0;
}
Tile TileRepo::takeTile (int i) { ///< @param i The tile letter in INDEX format
    --tiles[i];
    --_numTiles;
    if (i < ALPHANUM) return Tile(i+97);
    else if (i == BLANK) return Tile('?');
    else throw bad_input("TileRepo::takeTile()");
}
void TileRepo::takeTile (int i, int amt) { /// discards @param amt number of type @param i tiles
    tiles[i] -= amt;
    _numTiles -= amt;
}
void TileRepo::putTile (Tile t) { ///< @param t Uppercase and '?' letters will be interpreted as blanks
    ++tiles[toindex2(t.letter())];
    ++_numTiles;
}
void TileRepo::putTile (int i) {
    ++tiles[i];
    ++_numTiles;
}
void TileRepo::putTile (Tile t, int num) { ///< @param t Uppercase and '?' letters will be interpreted as blanks
    tiles[toindex2(t.letter())] += num;
    _numTiles += num;
}
int TileRepo::numTiles () const {
    return _numTiles;
}
int TileRepo::numOf (int i) const {
    return tiles[i];
}
void TileRepo::empty () {
    for (int i=0; i<CHARNUM; i++)
        tiles[i] = 0;
    _numTiles = 0;
}

int Rack::totalValue () const {
    int res = 0;
    for (int i=0; i<CHARNUM; i++)
        if (hasTile(i)) {
            res += Tile(i).value();
        }
    return res;
}

void Rack::subtract (const Rack &r) {
    for (uint i=0; i<CHARNUM; i++) {
        takeTile(i, r.numOf(i));
    }
}

void Rack::replenish (TileBag &tb) {
    for (int i=numTiles(); i<RACKSIZE; i++) {
        if (tb.numTiles() == 0) break;
        putTile(tb.drawTile());
    }
}

string txtDisplay (const TileRepo& tileRepo) {
    string str;
    for (int i=0; i<CHARNUM; i++) {
        for (int j=0; j<tileRepo.numOf(i); j++) {
            if (i < ALPHANUM) str.push_back(i+97);
            else if (i == BLANK) str.push_back('?');
            else throw bad_input("TileRepo::display()");
        }
    }
    return str;
}

TileBag::TileBag () {
    srand(time(NULL));
    reset();
}

void TileBag::reset () {
    empty();
    putTile(Tile('e'), 12); putTile(Tile('a'), 9);
    putTile(Tile('i'), 9); putTile(Tile('o'), 8);
    putTile(Tile('n'), 6); putTile(Tile('r'), 6);
    putTile(Tile('t'), 6); putTile(Tile('l'), 4);
    putTile(Tile('s'), 4); putTile(Tile('u'), 4);
    putTile(Tile('d'), 4); putTile(Tile('g'), 3);
    putTile(Tile('b'), 2); putTile(Tile('c'), 2);
    putTile(Tile('m'), 2); putTile(Tile('p'), 2);
    putTile(Tile('f'), 2); putTile(Tile('h'), 2);
    putTile(Tile('v'), 2); putTile(Tile('w'), 2);
    putTile(Tile('y'), 2); putTile(Tile('k'), 1);
    putTile(Tile('j'), 1); putTile(Tile('x'), 1);
    putTile(Tile('q'), 1); putTile(Tile('z'), 1);
    putTile(Tile('?'), 2);
}

Tile TileBag::drawTile () {
    int random = (int)round((float)(rand())/RAND_MAX*numTiles());
    int a;
    for (a=0; a<CHARNUM; a++) {
        if (random > numOf(a))
            random -= numOf(a);
        else break;
    }
    return takeTile(a);
}
