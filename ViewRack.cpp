#include "ViewRack.h"

using namespace std;

ViewRack::ViewRack (PlayerIn &p, ViewCellRepo &r, QWidget *parent) : QWidget(parent), pin(p), repo(r) {
    for (int i=0; i<RACKSIZE; i++) {
        row[i] = repo.addCell();
    }
    reload();
    connect(&pin, SIGNAL(modified()), this, SLOT(reload()));
}
void ViewRack::resizeCells (const QSize cellSize) {
    resize(cellSize.width() * RACKSIZE, cellSize.height());
    for (int i=0; i<RACKSIZE; i++) {
        row[i]->resize(cellSize);
        row[i]->move(x()+i*cellSize.width(), y());
    }
}
void ViewRack::moveEvent (QMoveEvent *) {
    for (int i=0; i<RACKSIZE; i++)
        row[i]->move(x()+i*row[i]->width(), y());
}
ViewTile* ViewRack::takeTile (char c) {
    for (int i=0; i<RACKSIZE; i++) {
        if (row[i]->hasTile()) {
            if (row[i]->tile()->letter() == c) {
                ViewTile *tmp = row[i]->tile();
                row[i]->cutTile();
                return tmp;
            } else if (row[i]->tile()->letter() == '?') {
                ViewTile *tmp = row[i]->tile();
                row[i]->cutTile();
                tmp->setText(QChar(c-32));
                return tmp;
            }
        }
    }
    return 0; // not found
}
void ViewRack::addTile (ViewTile* tile) {
    for (int i=0; i<RACKSIZE; i++) {
        if (!row[i]->hasTile()) {
            row[i]->addTile(tile);
            break;
        }
    }
}
void ViewRack::reload () {
    string str = pin.rack().txtDisplay();
    for (uint i=0; i<RACKSIZE; i++) {
        if (row[i]->hasTile()) row[i]->rmvTile();
    }
    for (uint i=0; i<str.size(); i++) {
        row[i]->addTile(repo.newTile(str[i]));
        row[i]->tile()->setMyDisabled(false);
    }
}
