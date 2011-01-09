#include <stdio.h>

#include "Dict.h"
#include "GameCore.h"
#include "Berry.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MoveLabel.h"
#include "PlayArea.h"
#include "TextDisplay.h"
#include "ScoreLabel.h"

using std::vector;
using std::string;

int main (int argc, char* argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<MoveInfo>("MoveInfo");
    qRegisterMetaType<PassInfo>("PassInfo");

    Dict dict;
    dict.load("dawg.bin");
    GameCore gm(dict);

    QWidget win;
    win.resize(1024, 768);

    QVBoxLayout *leftLayout = new QVBoxLayout;

    QHBoxLayout *scoreLayout = new QHBoxLayout;
    ScoreLabel *scoreLabel1 = new ScoreLabel(gm.pin(0), Qt::AlignLeft);
    ScoreLabel *scoreLabel2 = new ScoreLabel(gm.pin(1), Qt::AlignRight);
    scoreLayout->addWidget(scoreLabel1);
    scoreLayout->addWidget(scoreLabel2);
    leftLayout->addLayout(scoreLayout);

    PlayArea *playArea = new PlayArea(gm.board(), gm.pin(1));
    leftLayout->addWidget(playArea);

    QHBoxLayout *turnBtnsLayout = new QHBoxLayout;
    MoveButton *moveButton = new MoveButton;
    turnBtnsLayout->addWidget(moveButton);
    QObject::connect(moveButton, SIGNAL(clicked()), &playArea->viewBoard(), SLOT(submit()));
    QObject::connect(&playArea->viewBoard(), SIGNAL(submitMove(MoveInfo)), &gm.pin(1), SLOT(doMove(MoveInfo)));

    PassButton *passButton = new PassButton;
    turnBtnsLayout->addWidget(passButton);
    QObject::connect(passButton, SIGNAL(clicked()), &gm.pin(1), SLOT(doPass()));

    leftLayout->addLayout(turnBtnsLayout);

    MoveLabel *moveLabel = new MoveLabel;
    leftLayout->addWidget(moveLabel);
    QObject::connect(&playArea->viewBoard(), SIGNAL(modified(MoveInfo)), moveLabel, SLOT(loadMove(MoveInfo)));
    QObject::connect(&playArea->viewBoard(), SIGNAL(noMove()), moveLabel, SLOT(clear()));
    QObject::connect(&playArea->viewBoard(), SIGNAL(errorMove()), moveLabel, SLOT(setInvalid()));

    QTextEdit *textDisplay = new TextDisplay;
    QObject::connect(&gm.state(), SIGNAL(moveAdded(int,MoveInfo)), textDisplay, SLOT(showMove(int,MoveInfo)));
    QObject::connect(&gm.state(), SIGNAL(passAdded(int)), textDisplay, SLOT(showPass(int)));
    QObject::connect(&gm.state(), SIGNAL(roundOver()), textDisplay, SLOT(showRoundOver()));
    // namespace prefix is necessary for the signal/slot to match
    QObject::connect(&gm.pin(1), SIGNAL(illegalMove(std::vector<std::string>)), textDisplay,
            SLOT(showIllegals(std::vector<std::string>)));
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(textDisplay);

    QHBoxLayout *fullLayout = new QHBoxLayout;
    fullLayout->addLayout(leftLayout);
    fullLayout->addLayout(rightLayout);
    win.setLayout(fullLayout);
    win.show();

    gm.reset();
    Berry berry (gm.pin(0), gm.board(), dict);
    berry.makeTurn();

    return app.exec();
}
