#ifndef VIEWBOARD_H
#define VIEWBOARD_H

#include "Board.h"
#include "ViewTile.h"

class ViewBoard : public QWidget {
    Q_OBJECT
    Board &bd;
    ViewCellRepo &repo;
    ViewRack &viewRack;
    ViewBoardCell *grid[BOARDSIZE][BOARDSIZE];
    MoveInfo *movePtr;
public:
    ViewBoard (Board &b, ViewCellRepo &r, ViewRack &vr, QWidget *parent = 0);
    void resizeCells (const QSize cellSize);
    void cacheMove (MoveInfo mv);
    void uncacheMove ();
public slots:
    /**
      Evaluates whether the tiles currently placed on the board
      constitute a valid move. If so, it calculates and displays that score.
    **/
    void computeMove ();
    void submit ();
    void reload ();
signals:
    void errorMove ();
    void noMove ();
    void modified (MoveInfo);
    void reloaded ();
    void submitMove (MoveInfo);
};

#endif // VIEWBOARD_H
