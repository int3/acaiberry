#include "ViewBoard.h"

ViewBoard::ViewBoard (Board &b, ViewCellRepo &r, ViewRack &vr, QWidget *parent) : QWidget(parent), bd(b), repo(r), viewRack(vr), movePtr(0) {
    connect(&repo, SIGNAL(modified()), this, SLOT(computeMove()));
    connect(&b, SIGNAL(modified()), this, SLOT(reload()));
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++) {
            grid[i][j] = repo.addBoardCell(viewRack);
            connect(grid[i][j], SIGNAL(submit()), this, SLOT(submit()));
            connect(grid[i][j], SIGNAL(modified()), this, SLOT(computeMove()));
            if (bd.bonus(i, j) == TLS) grid[i][j]->setObjectName ("TLSCell");
            else if (bd.bonus(i, j) == DLS) grid[i][j]->setObjectName ("DLSCell");
            else if (bd.bonus(i, j) == TWS) grid[i][j]->setObjectName ("TWSCell");
            else if (bd.bonus(i, j) == DWS) grid[i][j]->setObjectName ("DWSCell");
        }
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++) {
            if (i > 0) grid[i][j]->link(BACKWARDS, VERTICAL, grid[i-1][j]);
            if (i < BOARDSIZE-1) grid[i][j]->link(FORWARDS, VERTICAL, grid[i+1][j]);
            if (j > 0) grid[i][j]->link(BACKWARDS, HORIZONTAL, grid[i][j-1]);
            if (j < BOARDSIZE-1) grid[i][j]->link(FORWARDS, HORIZONTAL, grid[i][j+1]);
        }
}
void ViewBoard::resizeCells (const QSize cellSize) {
    resize(cellSize.width() * BOARDSIZE, cellSize.height() * BOARDSIZE);
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++) {
            grid[i][j]->resize(cellSize);
            grid[i][j]->move(x()+j*cellSize.width(), y()+i*cellSize.height());
        }
}
void ViewBoard::computeMove () {
    uncacheMove();
    int activeCount = 0;
    Coord topleft(0,0);
    bool found = false;
    for (Coord p(0,0); p[VERTICAL]<BOARDSIZE; ++p[VERTICAL])
        for (p[HORIZONTAL]=0; p[HORIZONTAL]<BOARDSIZE; ++p[HORIZONTAL])
            if (grid[p[0]][p[1]]->hasTile() && !grid[p[0]][p[1]]->tile()->isMyDisabled()) {
                ++activeCount;
                if (!found) {
                    found = true;
                    topleft = p;
                }
            }
    if (activeCount == 0) {
        emit noMove();
        return;
    }

    BiString word[2];
    Coord start[2];
    int accountedActive = 0;
    int ori;
    for (ori=0; ori<2; ++ori) {
        accountedActive = 0;
        start[ori] = topleft;
        for (Coord q=topleft; q[ori]<BOARDSIZE; ++q[ori]) {
            if (!grid[q[0]][q[1]]->hasTile()) break;
            if (!grid[q[0]][q[1]]->tile()->isMyDisabled()) ++accountedActive;
            word[ori].push_back(grid[q[0]][q[1]]->tile()->text()[0].toAscii());
        }
        for (Coord q=topleft.tAdd(ori, -1); q[ori]>=0; --q[ori]) {
            if (!grid[q[0]][q[1]]->hasTile()) break;
            word[ori].push_front(grid[q[0]][q[1]]->tile()->text()[0].toAscii());
            start[ori] = q;
        }
        if (accountedActive > 1) break; // ori will be in the direction with more active tiles when loop exits, if such a direction exists
    }
    if (accountedActive < activeCount) {
        emit errorMove();
        return;
    }
    if (accountedActive == 1) {
        if (word[HORIZONTAL].size() * word[VERTICAL].size() == 1) { // player is probably in process of putting tiles on board
            emit noMove();
            return;
        }
        ori = word[HORIZONTAL].size() > word[VERTICAL].size() ? HORIZONTAL : VERTICAL;
    }
    cacheMove(MoveInfo(start[ori][0], start[ori][1], ori, word[ori].toStr(), bd));
    emit modified(*movePtr);
}
void ViewBoard::cacheMove (MoveInfo mv) {
    uncacheMove();
    movePtr = new MoveInfo(mv);
}
void ViewBoard::uncacheMove () {
    if (movePtr != 0) delete movePtr;
    movePtr = 0;
}
void ViewBoard::submit () {
    if (movePtr != 0) emit submitMove(*movePtr);
    uncacheMove();
}
void ViewBoard::reload () {
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++)
            if (grid[i][j]->hasTile()) {
                grid[i][j]->rmvTile();
            }
    for (int i=0; i<BOARDSIZE; i++)
        for (int j=0; j<BOARDSIZE; j++)
            if (bd.hasTile(i, j)) {
                ViewTile *tmp = repo.newTile(bd.getTile(i, j).letter());
                grid[i][j]->addTile(tmp);
            }
    emit reloaded();
    emit noMove();
}
