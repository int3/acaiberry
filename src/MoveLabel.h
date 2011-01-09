#ifndef MOVELABEL_H
#define MOVELABEL_H

#include <QLabel>
#include "TurnInfo.h"

class MoveLabel : public QLabel {
    Q_OBJECT
public:
    MoveLabel (QWidget *parent=0) : QLabel(parent) {
    }
public slots:
    void loadMove (MoveInfo mv) {
        setText(QString(mv.displayText().c_str()).remove(0, 5));
    }
    void clear () {
        setText("");
    }
    void setInvalid () {
        setText("invalid");
    }
};

#include <QPushButton>

class MoveButton : public QPushButton {
    Q_OBJECT
public:
    MoveButton (QWidget *parent=0) : QPushButton("Done", parent) {
    }
public slots:
    void mouseReleaseEvent (QMouseEvent* event) {
        emit clicked();
        QPushButton::mouseReleaseEvent(event);
    }
signals:
    void clicked();
};

class PassButton : public QPushButton {
    Q_OBJECT
public:
    PassButton (QWidget *parent=0) : QPushButton("Pass", parent) {
    }
    void mouseReleaseEvent (QMouseEvent *event) {
        emit clicked();
        QPushButton::mouseReleaseEvent(event);
    }
signals:
    void clicked ();
};

#endif // MOVELABEL_H
