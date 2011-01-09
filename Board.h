#ifndef BOARD_H
#define BOARD_H

#include "datastructs.h"
#include "Dict.h"
#include "Tile.h"
#include "TurnInfo.h"
#include <QObject>
#include <cstdio>

class Cell {
    Tile *tile;
public:
    Cell () : tile(0) { }
    Cell (Tile t) : tile(new Tile(t)) { }
    void reset () {
        rmvTile();
    }
    ~Cell () {
        if (tile != 0) delete tile;
    }
    void setTile (Tile t) {
        if (tile != 0) delete tile;
        tile = new Tile(t);
    }
    void rmvTile () {
        if (tile != 0) delete tile;
        tile = 0;
    }
    bool hasTile () const {
        if (tile == 0) return false;
        else return true;
    }
    Tile& getTile () {
        return *tile;
    }
};

class MoveInfo;

class Board : public QObject {
    Q_OBJECT
    Cell grid[BOARDSIZE][BOARDSIZE];
    int _xScore[2][BOARDSIZE][BOARDSIZE]; // xScore[0][][] is used for calculating score of horizontal moves, xScore[1][][] the vertical ones
    bool _hasBranch[2][BOARDSIZE][BOARDSIZE];
    static const char _bonus[BOARDSIZE][BOARDSIZE];
    Constraint _xCheck[2][BOARDSIZE][BOARDSIZE];
    Dict &_dict;
    void calcXValues(); ///< generate cross-checks and cross-scores
    /**
      for storing the tiles on the board so it can be reverted to
      an earlier state if necessary. Skips the unnecessary cross-check information
      though, that can be recalculated.
    **/
    struct old {
        Cell grid[BOARDSIZE][BOARDSIZE];
    } previous;
public:
    Board (Dict &dict);
    void reset ();
    void archive (); ///< store the board state
    void revert (); ///< revert to an earlier state
    void placeTiles (const MoveInfo& mv);
    int xScore (int a, int b, int c) const;
    Constraint xCheck (int a, int b, int c) const;
    bool hasTile (int x, int y) const;
    Tile& getTile (int x, int y);
    bool hasBranch (int a, int x, int y) const;
    int wsMult (int x, int y) const; ///< word score multiplier
    int lsMult (int x, int y) const; ///< letter score multiplier
    int bonus (int x, int y) const;
    bool isAnchor (int x, int y) const;
    friend std::string txtDisplay (Board&); ///< displays a console representation
signals:
    void modified ();
};

inline int Board::xScore (int a, int b, int c) const {
    return _xScore[a][b][c];
}
inline Constraint Board::xCheck (int a, int b, int c) const {
    return _xCheck[a][b][c];
}
inline bool Board::hasTile (int x, int y) const {
    return grid[x][y].hasTile();
}
inline Tile& Board::getTile (int x, int y) {
    return grid[x][y].getTile();
}
inline bool Board::hasBranch (int a, int x, int y) const {
    return _hasBranch[a][x][y];
}
inline bool Board::isAnchor (int x, int y) const {
    if (x == (BOARDSIZE-1)/2 && y == (BOARDSIZE-1)/2) return true;
    return _hasBranch[HORIZONTAL][x][y] || _hasBranch[VERTICAL][x][y];
}
inline int Board::bonus (int x, int y) const {
    return _bonus[x][y];
}
#endif //BOARD_H
