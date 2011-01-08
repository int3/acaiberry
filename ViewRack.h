#ifndef VIEWRACK_H
#define VIEWRACK_H

#include "PlayerIn.h"
#include "ViewTile.h"

class ViewRack : public QWidget {
    Q_OBJECT
    PlayerIn &pin;
    ViewCellRepo &repo;
    ViewCell *row[RACKSIZE];
public:
    ViewRack (PlayerIn &p, ViewCellRepo &r, QWidget *parent = 0);
    void resizeCells (const QSize cellSize);
    void moveEvent (QMoveEvent *);
    ViewTile* takeTile (char c); /// @param c should be in non-index form
    void addTile (ViewTile*);
public slots:
    void reload ();
};

#endif // VIEWRACK_H
