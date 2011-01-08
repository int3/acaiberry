#ifndef DICT_H
#define DICT_H

#include <vector>
#include "datastructs.h"
#include "TurnInfo.h"
#include "TileRepo.h"

struct Edge {
    uchar val;
    uint ptr;
    Edge (char v = 0, int p = 0) : val(v), ptr(p) {
    }
    void unserialize (FILE *fp);
};

class Node {
    Edge *edges;
    uint listSize;
public:
    bool isTerm;
    Node () : listSize(0), isTerm(false) {
    }
    ~Node () {
        delete[] edges;
    }
    int operator[] (int n) const {
        for (uint i = 0; i < listSize; ++i) {
            if (edges[i].val == n) return edges[i].ptr;
            if (edges[i].val > n) return 0;
        }
        return 0;
    }
    Edge& at (int n) const {
        return edges[n];
    }
    uint size () const {
        return listSize;
    }
    void unserialize (FILE *fp);
};

class Dict {
    Node *nodes;
    void findWordsRec (TileRepo &tiles, std::string &str, std::vector<std::string> &res, int np);
    bool findConstraintRec (const std::string &str, Constraint &cons, int np, uint pos);
    void findMovesRec (TileRepo&, Board&, const int[2], int, int, std::string&, std::vector<MoveInfo>&, int=0, int=0, int=0);
public:
    ~Dict ();
    void load (std::string fname);
    bool contains (const std::string str);
    std::vector<std::string> findWords (TileRepo tiles);
    void findMoves (TileRepo tiles, Board &bd, const int[2], int ori, int minLen, std::vector<MoveInfo>&);
    Constraint findConstraint (const std::string str);
};

#endif // DICT_H
