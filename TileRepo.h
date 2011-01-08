#ifndef TILEREPO_H
#define TILEREPO_H

#include "Tile.h"
#include "datastructs.h"

class TileRepo {
    int tiles[CHARNUM];
    int _numTiles;
public:
    TileRepo ();
    TileRepo (std::string str);
    bool hasTile (int i) const;
    Tile takeTile (int i);
    void takeTile (int i, int amt);
    void putTile (Tile t);
    void putTile (int i);
    void putTile (Tile t, int num);
    int numTiles () const;
    int numOf (int i) const;
    void empty ();
    std::string txtDisplay () const;
};

class TileBag : public TileRepo {
public:
    TileBag ();
    void reset ();
    Tile drawTile ();
};

class Rack : public TileRepo {
public:
    Rack (std::string s) : TileRepo(s) {
    }
    Rack () {
    }
    int totalValue() const;
    void replenish(TileBag&);
    void subtract (const Rack &r);
};

#endif // TILEREPO_H
