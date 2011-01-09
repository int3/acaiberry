#ifndef SCORELABEL_H
#define SCORELABEL_H

#include <QLabel>
#include "PlayerIn.h"

class ScoreLabel : public QLabel {
    Q_OBJECT
    PlayerIn &pin;
public:
    ScoreLabel (PlayerIn &p, Qt::Alignment align) : pin(p) {
        setAlignment(align);
        connect(&pin, SIGNAL(modified()), this, SLOT(update()));
    }
public slots:
    void update() {
        setText("Player " + QString::number(pin.id()+1) + ": " + QString::number(pin.score()) + "pts");
    }
};

#endif // SCORELABEL_H
