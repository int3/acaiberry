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
    std::vector<Edge> edges;
    uint listSize;
public:
    bool isTerm;
    Node () : listSize(0), isTerm(false) {
    }
    int operator[] (int n) const {
        for (uint i = 0; i < listSize; ++i) {
            if (edges[i].val == n) return edges[i].ptr;
            if (edges[i].val > n) return 0;
        }
        return 0;
    }
    const Edge at (int n) const {
        return edges[n];
    }
    uint size () const {
        return listSize;
    }
    void unserialize (FILE *fp);
};

class Dict {
    std::vector<Node> nodes;
    void findWordsRec (TileRepo &tiles, std::string &str, std::vector<std::string> &res, int np);
    bool findConstraintRec (const std::string &str, Constraint &cons, int np, uint pos);
    void findMovesRec (TileRepo&, Board&, const int[2], int, int, std::string&, std::vector<MoveInfo>&, int=0, int=0, int=0);
public:
    void load (std::string fname);
    bool contains (const std::string str);
    /// Generates all possible anagrams of a set of tiles.
    std::vector<std::string> findWords (TileRepo tiles);
    /// Generates all possible moves given a set of tiles and a board.
    void findMoves (TileRepo tiles, Board &bd, const int[2], int ori, int minLen, std::vector<MoveInfo>&);
    Constraint findConstraint (const std::string str);
};

#endif // DICT_H
