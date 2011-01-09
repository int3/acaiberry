#include "Berry.h"

using std::vector;

Berry::Berry (PlayerIn &p, Board &b, Dict &d) : pin(p), bd(b), dict(d) {
    connect(&p, SIGNAL(turnActivated()), this, SLOT(makeTurn()));
}

vector<MoveInfo> Berry::genMoves () {
    vector<MoveInfo> res;
    for (int p[2]={0, 0}; p[VERTICAL]<BOARDSIZE; p[VERTICAL]++) {
        for (p[HORIZONTAL]=0; p[HORIZONTAL]<BOARDSIZE; p[HORIZONTAL]++) {
            if (!bd.isAnchor(p[0], p[1])) continue;
            for (int ori=0; ori<2; ++ori) {
                for (int q[2] = {p[0], p[1]}; q[ori]>=0 && (!bd.isAnchor(q[0], q[1]) || q[ori] == p[ori]) && p[ori]-q[ori] < pin.rack().numTiles(); --q[ori]) {
                    while (q[ori] > 0 && bd.hasTile(q[0]+(ori^1)*-1, q[1]+ori*-1)) --q[ori];
                    dict.findMoves(pin.rack(), bd, q, ori, p[ori]-q[ori]+1, res);
                }
            }
        }
    }
    return res;
}
void Berry::makeTurn () {
    std::vector<MoveInfo> res = genMoves();
    if (res.size() != 0) {
        MoveInfo *best = &res[0];
        for (uint i=1; i<res.size(); i++)
            if (res[i].score() > best->score())
                best = &res[i];
        pin.doMove(*best);
    } else {
        pin.doPass();
    }
}
