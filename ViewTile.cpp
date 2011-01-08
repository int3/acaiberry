#include "ViewTile.h"
#include "ViewRack.h"

using namespace std;

ViewTile::ViewTile (QWidget *parent) : QLabel (parent), _isMyDisabled(true), _isUsed(false) {
    setAlignment(Qt::AlignCenter);
    del();
}
void ViewTile::del () {
    hide();
    setMyDisabled(true);
    setUsed(false);
    move(-100, -100);
}
void ViewTile::mousePressEvent (QMouseEvent *event) {
    if (_isMyDisabled) return;
    if (event->button() == Qt::RightButton) {
        if (letter() == '?') setText("A");
        else if (letter() == 'Z') setText("?");
        else if (letter() >= 'A' && letter() <= 'Z')
            setText(QChar(letter() + 1));
    } else if (event->button() == Qt::LeftButton) {
        if (letter() == '?') setText("Z");
        else if (letter() == 'A') setText("?");
        else if (letter() >= 'A' && letter() <= 'Z')
            setText(QChar(letter() - 1));
    }
    _initPos = pos();
    _initClickPos = event->globalPos();
    raise();
}
void ViewTile::mouseMoveEvent (QMouseEvent *event) {
    if (_isMyDisabled) return;
    move(_initPos+event->globalPos()-_initClickPos);
}
void ViewTile::mouseReleaseEvent (QMouseEvent*) {
    if (_isMyDisabled) return;
    emit released(this);
}
QPoint ViewTile::center () {
    return pos() + QPoint(width()/2, height()/2);
}
void ViewTile::setMyDisabled (bool b) {
    _isMyDisabled = b;
}
bool ViewTile::isMyDisabled () const {
    return _isMyDisabled;
}
void ViewTile::setUsed (bool b) {
    _isUsed = b;
}
bool ViewTile::isUsed () const {
    return _isUsed;
}
char ViewTile::letter () const {
    return text()[0].toAscii();
}

////////////////////////////////////////
// BEGIN CLASS VIEWCELL
////////////////////////////////////////
ViewCell::ViewCell (QWidget *parent) : QLabel(parent), _vTile(0) {
    resize(40, 40);
}
void ViewCell::addTile (ViewTile *vt) {
    _vTile = vt;
    _vTile->move(pos());
    _vTile->resize(size());
    _vTile->installEventFilter(this);
    emit modified();
}
bool ViewCell::hasTile () const {
    return _vTile != 0 ? true : false;
}
void ViewCell::cutTile () {
    _vTile->removeEventFilter(this);
    _vTile = 0;
    emit modified();
}
void ViewCell::rmvTile () {
    _vTile->removeEventFilter(this);
    _vTile->del();
    _vTile = 0;
}
ViewTile* ViewCell::tile () {
    return _vTile;
}
void ViewCell::moveEvent (QMoveEvent*) {
    if (_vTile != 0) {
        _vTile->move(pos());
    }
}
void ViewCell::resizeEvent (QResizeEvent*) {
    if (_vTile != 0) {
        _vTile->resize(size());
    }
}
bool ViewCell::eventFilter(QObject*, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        emit clicked(this);
    }
    return false;
}
bool ViewCell::isInArea (QPoint p) const {
    if (p.x() >= x() && p.x() <= x()+width() && p.y() >= y() && p.y() <= y()+height())
        return true;
    return false;
}
void ViewCell::swapTiles (ViewCell *vc) {
    ViewTile *tmp = vc->tile();
    vc->cutTile();
    if (hasTile()) {
        vc->addTile(tile());
        cutTile();
    }
    if (tmp) addTile(tmp);
}

////////////////////////////////////////
// BEGIN CLASS VIEWBOARDCELL
////////////////////////////////////////

ViewBoardCell::ViewBoardCell (ViewRack& vr, QWidget *parent) : ViewCell(parent), inMode(VERTICAL), isActive(false), viewRack(vr), arrowMap("img/arrow.gif") {
    setFocusPolicy(Qt::ClickFocus);
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++)
            adjCell[i][j] = 0;
}
void ViewBoardCell::activate (int dir, int ori) {
    if (hasTile()) {
        if (tile()->isMyDisabled()) {
            if (adjCell[dir][ori]) adjCell[dir][ori]->activate(dir, ori);
            return;
        } else {
            viewRack.addTile(tile());
            cutTile();
        }
    }
    inMode = ori;
    isActive = true;
    if (ori == HORIZONTAL) setPixmap(arrowMap.scaled(size()));
    else setPixmap(arrowMap.scaled(size()).transformed(QTransform().rotate(90)));
    setFocus(Qt::OtherFocusReason);
}
void ViewBoardCell::mousePressEvent (QMouseEvent*) {
    if (hasTile()) {
        clearFocus();
        return;
    }
    if (!isActive) {
        activate(FORWARDS, HORIZONTAL);
    } else if (inMode == HORIZONTAL) {
        activate(FORWARDS, VERTICAL);
    } else {
        isActive = false;
        clearFocus();
    }
}
void ViewBoardCell::focusOutEvent (QFocusEvent*) {
    isActive = false;
    setPixmap(QPixmap());
}
void ViewBoardCell::keyPressEvent (QKeyEvent *event) {
    int key = event->key();
    if (key >= Qt::Key_A && key <= Qt::Key_Z) {
        ViewTile *tmp = viewRack.takeTile(key+32);
        if (tmp) {
            addTile(tmp);
            if (adjCell[FORWARDS][inMode]) adjCell[FORWARDS][inMode]->activate(FORWARDS, inMode);
        }
        return;
    } else if (key == Qt::Key_Backspace && adjCell[BACKWARDS][inMode]) {
        adjCell[BACKWARDS][inMode]->activate(BACKWARDS, inMode);
        return;
    } else if (key == Qt::Key_Return) {
        emit submit();
        clearFocus();
        return;
    }
    QLabel::keyPressEvent(event);
    return;
}

void ViewBoardCell::link (int dir, int ori, ViewBoardCell *vbc) {
    adjCell[dir][ori] = vbc;
}
////////////////////////////////////////
// BEGIN CLASS VIEWCELLREPO
////////////////////////////////////////
ViewCellRepo::ViewCellRepo (QWidget *par) : parent(par) {
    for (int i=0; i<150; i++) {
        tileList.push_back(new ViewTile(parent));
        connect(tileList[i], SIGNAL(released(ViewTile*)), this, SLOT(tileGuide(ViewTile*)));
    }
}
ViewCellRepo::~ViewCellRepo () {
    for (uint i=0; i<tileList.size(); i++)
        delete tileList[i];
}
ViewTile* ViewCellRepo::newTile (char c) {
    for (uint i=0; i<tileList.size(); i++) {
        if (!tileList[i]->isUsed()) {
            tileList[i]->setUsed(true);
            tileList[i]->setVisible(true);
            tileList[i]->setText(QString(QChar(c)));
            tileList[i]->raise();
            return tileList[i];
        }
    }
    throw runtime_error("ViewCellRepo::newTile(): not enough tiles to allocate!");
}
ViewCell* ViewCellRepo::addCell () {
    ViewCell *tmp = new ViewCell(parent);
    cellList.push_back(tmp);
    connect(tmp, SIGNAL(clicked(ViewCell*)), this, SLOT(activateCell(ViewCell*)));
    return tmp;
}
ViewBoardCell* ViewCellRepo::addBoardCell (ViewRack &vr) {
    ViewCell *tmp = new ViewBoardCell(vr, parent);
    cellList.push_back(tmp);
    connect(tmp, SIGNAL(clicked(ViewCell*)), this, SLOT(activateCell(ViewCell*)));
    return dynamic_cast<ViewBoardCell*>(tmp);
}
void ViewCellRepo::activateCell (ViewCell *vc) {
    activeCell = vc;
}
void ViewCellRepo::tileGuide (ViewTile *vt) {
    bool homeFound = false;
    for (uint i=0; i<cellList.size(); i++) {
        if (cellList[i]->isInArea(vt->center())) {
            if (cellList[i]->hasTile() && cellList[i]->tile()->isMyDisabled()) break;
            cellList[i]->swapTiles(activeCell);
            homeFound = true;
            break;
        }
    }
    if (!homeFound) {
        activeCell->addTile(vt);
    } else emit modified();
}
