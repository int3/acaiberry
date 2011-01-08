#include "PlayArea.h"

PlayArea::PlayArea (Board &b, PlayerIn &p, QWidget *parent) : QWidget(parent), repo(this), vrk(new ViewRack(p, repo, this)), vbd(new ViewBoard(b, repo, *vrk, this))  {
    connect(vbd, SIGNAL(reloaded()), this, SLOT(reload()));
    resetStyleSheet();
}
ViewRack& PlayArea::viewRack () {
    return *vrk;
}
ViewBoard& PlayArea::viewBoard () {
    return *vbd;
}
void PlayArea::resetStyleSheet () {
    QString fontStr = "font-size: ";
    fontStr += QString::number(cellSize.width() > 0 ? cellSize.width()*2/3 : 1);
    fontStr += "px;";
    setStyleSheet(
        "ViewCell { background-color: #AAAAAA; border-width: 1; border-style: solid; border-color: #666666; }"
        "ViewCell#TWSCell { background-color: #FFAAAA; border-width: 1; border-style: solid; border-color: #666666; }"
        "ViewCell#DWSCell { background-color: #FFCCCC; border-width: 1; border-style: solid; border-color: #666666; }"
        "ViewCell#TLSCell { background-color: #AAAAFF; border-width: 1; border-style: solid; border-color: #666666; }"
        "ViewCell#DLSCell { background-color: #CCCCFF; border-width: 1; border-style: solid; border-color: #666666; }"
        "ViewTile { background-color: #CCCCCC; border-width: 1; border-style: solid; border-color: #666666; color: #000000; font-weight: bold;"
        + fontStr +
        "}"
        "ViewTile[myDisabled=\"true\"] { color: #333333; }"
    );
}
void PlayArea::resizeEvent (QResizeEvent*) {
    int tmpWidth = width()/15.0f;
    int tmpHeight = height()/17.0f;
    int cs = tmpWidth < tmpHeight ? tmpWidth : tmpHeight;
    cellSize = QSize(cs, cs);
    vbd->resizeCells(cellSize);
    vrk->resizeCells(cellSize);
    vrk->move(cs*4, cs*16);
    resetStyleSheet();
}
QSize PlayArea::sizeHint () const {
    return QSize (512, 580); // 15:17 ratio
}
void PlayArea::reload () {
    resetStyleSheet();
}
void PlayArea::mousePressEvent (QMouseEvent*) {
    vbd->reload();
    vrk->reload();
    setFocus(); // slacky way to make the arrows disappear
}
