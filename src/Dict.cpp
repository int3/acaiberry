#include "Dict.h"
#include "Board.h"

using std::string;
using std::vector;
using std::runtime_error;

void Dict::load (string fname) {
    FILE *fp;
    uint nsize;
    if ((fp = fopen(fname.c_str(), "rb")) == 0) {
        fclose(fp);
        throw runtime_error("Dict::unserialize: cannot open file.");
    }
    if (fread(&nsize, sizeof(uint), 1, fp) != 1) {
        fclose(fp);
        throw runtime_error("Dict::unserialize: read error.");
    }
    nodes.resize(nsize);
    for (uint i=0; i<nsize; i++) {
        nodes[i].unserialize(fp);
    }
    fclose(fp);
}

bool Dict::contains (const string str) {
    int np = 0;
    for (uint i=0; i<str.size(); i++) {
        if ((np = nodes[np][toupper(str[i])-65]) == 0)
            return false;
    }
    if (nodes[np].isTerm) return true;
    else return false;
}

vector<string> Dict::findWords (TileRepo tiles) {
    string tmp;
    vector<string> res;
    findWordsRec (tiles, tmp, res, 0);
    return res;
}

void Dict::findWordsRec (TileRepo &tiles, string &str, vector<string> &res, int np) {
    if (nodes[np].isTerm) {
        res.push_back(str);
    }
    for (int i=0; i<ALPHANUM; i++) {
        if (nodes[np][i] != 0) {
            if (tiles.hasTile(i)) {
                tiles.takeTile(i);
                str.push_back(i+65);
                findWordsRec(tiles, str, res, nodes[np][i]);
                str.erase(str.size()-1);
                tiles.putTile(Tile(i+97));
            } else if (tiles.hasTile(BLANK)) {
                tiles.takeTile(BLANK);
                str.push_back(i+65);
                findWordsRec(tiles, str, res, nodes[np][i]);
                str.erase(str.size()-1);
                tiles.putTile(Tile('?'));
            }
        }
    }
}

Constraint Dict::findConstraint (const string str) {
    Constraint cons;
    findConstraintRec (str, cons, 0, 0);
    return cons;
}

bool Dict::findConstraintRec (const string &str, Constraint &cons, int np, uint pos) {
    if (pos == str.size()) {
        if (nodes[np].isTerm) return true;
        else return false;
    }
    if (isalpha(str[pos]) && ((np = nodes[np][toindex(str[pos])]) != 0)) {
        return findConstraintRec(str, cons, np, pos+1);
    }
    else if (str[pos] == '?') {
        uint nsize = nodes[np].size();
        for (uint i=0; i<nsize; ++i) {
            const Edge next = nodes[np].at(i);
            if (findConstraintRec(str, cons, next.ptr, pos+1)) {
                cons.addLetter(next.val);
            }
        }
    }
    return false;
}

void Dict::findMoves (TileRepo tiles, Board &bd, const int coord[2], int ori, int minLen, vector<MoveInfo> &res) {
    string str;
    findMovesRec (tiles, bd, coord, ori, minLen, str, res);
}

void Dict::findMovesRec (TileRepo &tiles, Board &bd, const int coord[2], int ori, int minLen, string &str, vector<MoveInfo> &res, int depth, int np, int usedTiles) {
    int p[2] = {coord[0]+(ori^1)*depth, coord[1]+ori*depth};
    if (nodes[np].isTerm && depth >= minLen && (p[ori] == BOARDSIZE || !bd.hasTile(p[0], p[1])) && usedTiles > 0) {
        res.push_back(MoveInfo(coord[0], coord[1], ori, str, bd));
    }
    if (p[ori] == BOARDSIZE) return;
    if (bd.hasTile(p[0], p[1])) {
        char c = bd.getTile(p[0], p[1]).letter();
        if (nodes[np][toindex(c)] != 0) {
            str.push_back(c);
            findMovesRec (tiles, bd, coord, ori, minLen, str, res, depth+1, nodes[np][toindex(c)], usedTiles);
            str.erase(str.size()-1);
        }
        return;
    }
    uint nsize = nodes[np].size();
    for (uint i=0; i<nsize; ++i) {
        const Edge next = nodes[np].at(i);
        if (bd.xCheck(ori, coord[0]+(ori^1)*depth, coord[1]+ori*depth).hasLetter(next.val)) {
            int a;
            if (tiles.hasTile(next.val)) a = next.val+97;
            else if (tiles.hasTile(BLANK)) a = next.val+65;
            else continue;
            tiles.takeTile(toindex2(a));
            str.push_back(a);
            findMovesRec (tiles, bd, coord, ori, minLen, str, res, depth+1, next.ptr, usedTiles+1);
            str.erase(str.size()-1);
            tiles.putTile(Tile(a));
        }
    }
}

void Edge::unserialize (FILE *fp) {
    uint tmp = 0;
    if (fread(&tmp, 3, 1, fp) != 1) throw runtime_error("Edge::serialize: write error.");
    val = tmp >> 17;
    ptr = tmp & 0x1FFFF;
}

void Node::unserialize (FILE *fp) {
    listSize = 0;
    if (fread(&listSize, 1, 1, fp) != 1) throw runtime_error("EdgeList::unserialize: read error.");
    edges.resize(listSize);
    for (uint i=0; i<listSize; ++i) {
        edges[i].unserialize(fp);
    }
    if (fread(&isTerm, sizeof(isTerm), 1, fp) != 1) throw runtime_error("Node::unserialize: read error.");
}
