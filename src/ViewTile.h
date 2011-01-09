#ifndef VIEWTILE_H
#define VIEWTILE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include "globals.h"
class ViewRack;

class ViewTile : public QLabel {
    Q_OBJECT
    bool _isMyDisabled, _isUsed;
    Q_PROPERTY(bool myDisabled READ isMyDisabled WRITE setMyDisabled)
    QPoint _initPos, _initClickPos;
public:
    ViewTile (QWidget *parent=0);
    void del ();
    void mousePressEvent (QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent*);
    QPoint center ();
    void setMyDisabled (bool isMyDisabled);
    bool isMyDisabled () const;
    void setUsed (bool);
    bool isUsed () const;
    char letter () const;
signals:
    void released (ViewTile*); /// emits position of _center_ of tile
};

/**
  @brief Forms the board squares / rack squares.
**/
class ViewCell : public QLabel {
    Q_OBJECT
    ViewTile *_vTile;
public:
    ViewCell (QWidget *parent=0);
    void addTile (ViewTile *vt);
    bool hasTile () const;
    void cutTile ();
    void rmvTile ();
    ViewTile* tile ();
    void moveEvent (QMoveEvent*);
    void resizeEvent (QResizeEvent*);
    bool eventFilter(QObject*, QEvent* event);
    bool isInArea (QPoint p) const; /// top-left-inclusive right-bottom-exclusive
    void swapTiles (ViewCell*);
signals:
    void clicked (ViewCell*);
    void modified ();
};

class ViewBoardCell : public ViewCell {
    Q_OBJECT
    ViewBoardCell *adjCell[2][2]; // forward/backward, vertical/horizontal
    int inMode;
    int isActive;
    ViewRack &viewRack;
    QPixmap arrowMap;
public:
    ViewBoardCell (ViewRack &vr, QWidget *parent=0);
    /**
      When activated, an arrow will appear on the cell,
      and the next keypress will cause a tile to be placed
      into this cell.
    **/
    void activate (int dir, int);
    void mousePressEvent (QMouseEvent*);
    void focusOutEvent (QFocusEvent*);
    void keyPressEvent (QKeyEvent *event);
    void link (int dir, int ori, ViewBoardCell *vbc);
signals:
    void submit ();
};

/**
  @brief Coordinates the drag / drop of ViewTiles.
  
  When a ViewTile is released by mouseclick, ViewCellRepo decides to which ViewCell it should go to. All
  ViewTiles and ViewCells must register with this class.
 **/
class ViewCellRepo : public QObject {
    Q_OBJECT
    QWidget *parent;
    std::vector<ViewTile*> tileList;
    std::vector<ViewCell*> cellList;
    ViewCell *activeCell;
public:
    ViewCellRepo (QWidget *par);
    ~ViewCellRepo ();
    ViewTile* newTile (char c);
    ViewCell* addCell ();
    ViewBoardCell* addBoardCell (ViewRack&);
public slots:
    void activateCell (ViewCell *vc);
    void tileGuide (ViewTile *vt);
signals:
    void modified (); /// emitted by tileGuide only
};

#endif // VIEWTILE_H
