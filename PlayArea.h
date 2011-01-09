#ifndef PLAYAREA_H
#define PLAYAREA_H

#include "ViewBoard.h"
#include "ViewRack.h"

class PlayArea : public QWidget {
    Q_OBJECT
    ViewCellRepo repo;
    ViewRack *vrk;
    ViewBoard *vbd;
    QSize cellSize;
public:
    PlayArea (Board &b, PlayerIn &p, QWidget *parent=0);
    void resetStyleSheet ();
    void resizeEvent (QResizeEvent*);
    QSize sizeHint () const;
    ViewRack& viewRack ();
    ViewBoard& viewBoard ();
    void mousePressEvent (QMouseEvent*);
public slots:
    void reload ();
signals:
    void moveModified (MoveInfo);
    void noMove ();
    void errorMove ();
    void submitMove (MoveInfo);
};

#endif // PLAYAREA_H
