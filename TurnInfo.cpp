#include "TurnInfo.h"
#include "Board.h"

using std::string;
using std::vector;

MoveInfo::MoveInfo () : _orient(0) { }

/// _isLegal defaults to true, so we can just forgo calling calcLegal() for computer moves and everything will still work fine
MoveInfo::MoveInfo (int a, int b, int c, string s, Board &bd) : _orient(c), _word(s), _score(0), _isLegal(true) {
    coord[0] = a;
    coord[1] = b;
    calcScore(bd);
}

void MoveInfo::calcScore (Board &bd) {
    int wsMod = 1; // word score modifier
    int branchScoreCum = 0, mainScore = 0; // cumulative branch score, main line score
    int numUsed = 0; // for calculating bingo
    for (uint p[2]={coord[0], coord[1]}, i=0; i<_word.size(); ++p[_orient], ++i) {
        if (bd.hasTile(p[0], p[1])) {
            mainScore += bd.getTile(p[0], p[1]).value();
            continue;
        } else {
            ++numUsed;
            _usedTiles.putTile(toindex2(_word[i]));
        }

        mainScore += Tile(_word[i]).value() * bd.lsMult(p[0], p[1]);
        wsMod *= bd.wsMult(p[0], p[1]);

        if (bd.hasBranch(_orient, p[0], p[1])) {
            int branchScore = bd.xScore(_orient, p[0], p[1]) + Tile(_word[i]).value() * bd.lsMult(p[0], p[1]);
            branchScore *= bd.wsMult(p[0], p[1]);
            branchScoreCum += branchScore;
        }
    }
    _score = mainScore * wsMod + branchScoreCum;
    if (numUsed == 7) _score += 50;
}

void MoveInfo::calcLegal (Board &bd, Dict &dict) {
    _isLegal = true;
    for (uint p[2]={coord[0], coord[1]}, i=0; i<_word.size(); ++p[_orient], ++i) {
        if (!bd.xCheck(_orient, p[0], p[1]).hasLetter(toindex(_word[i]))) {
            BiString wrongWord;
            wrongWord.push_back(_word[i]);
            // using hack to +1 to p[0] when move is horizontal, and +1 to p[1] when vertical
            for (int q[2]={p[0]+_orient, p[1]+(_orient^1)}; q[_orient^1]<BOARDSIZE && bd.hasTile(q[0], q[1]); ++q[_orient^1]) {
                wrongWord.push_back(bd.getTile(q[0], q[1]).letter());
            }
            for (int q[2]={p[0]-_orient, p[1]-(_orient^1)}; q[_orient^1]>=0 && bd.hasTile(q[0], q[1]); --q[_orient^1]) {
                wrongWord.push_front(bd.getTile(q[0], q[1]).letter());
            }
            illegals.push_back(wrongWord.toStr());
            _isLegal = false;
        }
    }
    if (!dict.contains(_word)) {
        illegals.push_back(_word);
        _isLegal = false;
    }
}

bool MoveInfo::isLegal () const {
    return _isLegal;
}

vector<string> MoveInfo::getIllegals () {
    return illegals;
}

turnType MoveInfo::type () const {
    return MOVE;
}

string MoveInfo::displayText () const {
    char tmp[50];
    char c = pos(0)+65;
    int a = pos(1)+1;
    if (orient() == HORIZONTAL) {
        sprintf(tmp, "MOVE %c%d %s %d", c, a, word().c_str(), score());
    } else {
        sprintf(tmp, "MOVE %d%c %s %d", a, c, word().c_str(), score());
    }
    return tmp;
}

string PassInfo::displayText () const {
    return "PASS";
}

const Rack& MoveInfo::usedTiles () const {
    return _usedTiles;
}
