#include "Board.h"

using std::string;

Board::Board (Dict &dict) : _dict(dict) {
    calcXValues();
}

void Board::reset () {
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++) {
            grid[i][j].reset();
        }
    calcXValues();
}

void Board::archive () {
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++) {
            previous.grid[i][j].rmvTile();
            if (grid[i][j].hasTile()) previous.grid[i][j].setTile(grid[i][j].getTile());
        }
}

void Board::revert () {
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++) {
            grid[i][j].rmvTile();
            if (previous.grid[i][j].hasTile()) grid[i][j].setTile(previous.grid[i][j].getTile());
        }
    calcXValues();
    emit modified ();
}

void Board::placeTiles (const MoveInfo& mv) {
    //archive ();
    if (mv.orient() == HORIZONTAL) {
        for (uint i=0; i<mv.word().size(); i++) {
            grid[mv.pos(X)][mv.pos(Y)+i].setTile(Tile(mv.word()[i])); //constant row, varying column
        }
    } else {
        for (uint i=0; i<mv.word().size(); i++) {
            grid[mv.pos(X)+i][mv.pos(Y)].setTile(Tile(mv.word()[i]));
        }
    }
    calcXValues();
    emit modified();
}

void Board::calcXValues () {
    for (int i=0; i<BOARDSIZE; i++) {
        for (int j=0; j<BOARDSIZE; j++) {
            for (int ori=0; ori < 2; ori++) {
                _xScore[ori][i][j] = 0;
                _hasBranch[ori][i][j] = false;
                if (grid[i][j].hasTile()) {
                    _xCheck[ori][i][j].addLetter(toindex(grid[i][j].getTile().letter()));
                    continue;
                }
                BiString query;
                query.push_back('?');
                for (int p[2]={i+ori, j+(ori^1)}; p[ori^1]<BOARDSIZE && grid[p[0]][p[1]].hasTile(); ++p[ori^1]) {
                    _xScore[ori][i][j] += grid[p[0]][p[1]].getTile().value();
                    _hasBranch[ori][i][j] = true;
                    query.push_back(grid[p[0]][p[1]].getTile().letter());
                }
                for (int p[2]={i-ori, j-(ori^1)}; p[ori^1]>= 0 && grid[p[0]][p[1]].hasTile(); --p[ori^1]) {
                    _xScore[ori][i][j] += grid[p[0]][p[1]].getTile().value();
                    _hasBranch[ori][i][j] = true;
                    query.push_front(grid[p[0]][p[1]].getTile().letter());
                }
                if (query.size() > 1) {
                    _xCheck[ori][i][j] = _dict.findConstraint(query.toStr());
                } else _xCheck[ori][i][j].setAll();
            }
        }
    }
}

int Board::wsMult (int x, int y) const {
    if (_bonus[x][y] == TWS)
        return 3;
    else if (_bonus[x][y] == DWS)
        return 2;
    else
        return 1;
}
int Board::lsMult (int x, int y) const {
    if (_bonus[x][y] == TLS)
        return 3;
    else if (_bonus[x][y] == DLS)
        return 2;
    else
        return 1;
}

string txtDisplay(Board& board) {
    string str;
    for (int i=0; i<BOARDSIZE; i++) {
        for (int j=0; j<BOARDSIZE; j++) {
            if (board.grid[i][j].hasTile()) {
                str.push_back(' ');
                str.push_back(board.grid[i][j].getTile().letter());
            } else {
                str.push_back(' ');
                str.push_back('.');
            }
        }
        str.push_back('\n');
    }
    return str;
}

const char Board::_bonus[BOARDSIZE][BOARDSIZE] = {
    {TWS, NIL, NIL, DLS, NIL, NIL, NIL, TWS, NIL, NIL, NIL, DLS, NIL, NIL, TWS},
    {NIL, DWS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, DWS, NIL},
    {NIL, NIL, DWS, NIL, NIL, NIL, DLS, NIL, DLS, NIL, NIL, NIL, DWS, NIL, NIL},
    {DLS, NIL, NIL, DWS, NIL, NIL, NIL, DLS, NIL, NIL, NIL, DWS, NIL, NIL, DLS},
    {NIL, NIL, NIL, NIL, DWS, NIL, NIL, NIL, NIL, NIL, DWS, NIL, NIL, NIL, NIL},
    {NIL, TLS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, TLS, NIL},
    {NIL, NIL, DLS, NIL, NIL, NIL, DLS, NIL, DLS, NIL, NIL, NIL, DLS, NIL, NIL},
    {TWS, NIL, NIL, DLS, NIL, NIL, NIL, DWS, NIL, NIL, NIL, DLS, NIL, NIL, TWS},
    {NIL, NIL, DLS, NIL, NIL, NIL, DLS, NIL, DLS, NIL, NIL, NIL, DLS, NIL, NIL},
    {NIL, TLS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, TLS, NIL},
    {NIL, NIL, NIL, NIL, DWS, NIL, NIL, NIL, NIL, NIL, DWS, NIL, NIL, NIL, NIL},
    {DLS, NIL, NIL, DWS, NIL, NIL, NIL, DLS, NIL, NIL, NIL, DWS, NIL, NIL, DLS},
    {NIL, NIL, DWS, NIL, NIL, NIL, DLS, NIL, DLS, NIL, NIL, NIL, DWS, NIL, NIL},
    {NIL, DWS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, TLS, NIL, NIL, NIL, DWS, NIL},
    {TWS, NIL, NIL, DLS, NIL, NIL, NIL, TWS, NIL, NIL, NIL, DLS, NIL, NIL, TWS},
};
