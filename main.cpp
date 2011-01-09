#include <stdio.h>
#include <iostream>
#include <exception>

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
using std::cout;
using std::exception;

class OptionError : public exception { };

/**
 * Set up the window.
 * @param playerId The ID of the player using the GUI for move input.
 */
void setupWindow (QWidget& win, GameCore& gc, int playerId) {
    win.resize(1024, 768);

    QVBoxLayout *leftLayout = new QVBoxLayout;

    QHBoxLayout *scoreLayout = new QHBoxLayout;
    ScoreLabel *scoreLabel1 = new ScoreLabel(gc.pin(0), Qt::AlignLeft);
    ScoreLabel *scoreLabel2 = new ScoreLabel(gc.pin(1), Qt::AlignRight);
    scoreLayout->addWidget(scoreLabel1);
    scoreLayout->addWidget(scoreLabel2);
    leftLayout->addLayout(scoreLayout);

    PlayArea *playArea = new PlayArea(gc.board(), gc.pin(playerId));
    leftLayout->addWidget(playArea);

    QHBoxLayout *turnBtnsLayout = new QHBoxLayout;
    MoveButton *moveButton = new MoveButton;
    turnBtnsLayout->addWidget(moveButton);
    QObject::connect(moveButton, SIGNAL(clicked()), &playArea->viewBoard(), SLOT(submit()));
    QObject::connect(&playArea->viewBoard(), SIGNAL(submitMove(MoveInfo)), &gc.pin(playerId), SLOT(doMove(MoveInfo)));

    PassButton *passButton = new PassButton;
    turnBtnsLayout->addWidget(passButton);
    QObject::connect(passButton, SIGNAL(clicked()), &gc.pin(playerId), SLOT(doPass()));

    leftLayout->addLayout(turnBtnsLayout);

    MoveLabel *moveLabel = new MoveLabel;
    leftLayout->addWidget(moveLabel);
    QObject::connect(&playArea->viewBoard(), SIGNAL(modified(MoveInfo)), moveLabel, SLOT(loadMove(MoveInfo)));
    QObject::connect(&playArea->viewBoard(), SIGNAL(noMove()), moveLabel, SLOT(clear()));
    QObject::connect(&playArea->viewBoard(), SIGNAL(errorMove()), moveLabel, SLOT(setInvalid()));

    QTextEdit *textDisplay = new TextDisplay;
    QObject::connect(&gc.state(), SIGNAL(moveAdded(int,MoveInfo)), textDisplay, SLOT(showMove(int,MoveInfo)));
    QObject::connect(&gc.state(), SIGNAL(passAdded(int)), textDisplay, SLOT(showPass(int)));
    QObject::connect(&gc.state(), SIGNAL(roundOver()), textDisplay, SLOT(showRoundOver()));
    // namespace prefix is necessary for the signal/slot to match
    QObject::connect(&gc.pin(playerId), SIGNAL(illegalMove(std::vector<std::string>)), textDisplay,
            SLOT(showIllegals(std::vector<std::string>)));
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(textDisplay);

    QHBoxLayout *fullLayout = new QHBoxLayout;
    fullLayout->addLayout(leftLayout);
    fullLayout->addLayout(rightLayout);
    win.setLayout(fullLayout);
    win.show();
}

int main (int argc, char* argv[]) {
    QApplication app(argc, argv);

    qRegisterMetaType<MoveInfo>("MoveInfo");
    qRegisterMetaType<PassInfo>("PassInfo");

    Dict dict;
    dict.load("dawg.bin");
    GameCore gc(dict);

    string usage(
            "AcaiBerry -- A Scrabble Program.\n"
            "Usage:\n"
            "[no arguments]       Run the game.\n"
            "--help               Display this message.\n"
            "--benchmark [num]    Simulate [num] games between 2 AI players. No GUI display.\n"
            "--me-first           Run the game with you making the first move.\n"
            "--demo               Run a game between 2 AI players and see the result.\n"
            );
    QStringList args = app.arguments();
    bool meFirst = false;
    bool demo = false;
    try {
        for (int i=0; i<args.length(); i++) {
            if (args[i] == "--help") {
                cout << usage;
                return 0;
            } else if (args[i] == "--benchmark") {
                i++;
                if (i >= args.length())
                    throw OptionError();
                bool conversionOk;
                int numGames = args[i].toInt(&conversionOk);
                if (!conversionOk)
                    throw OptionError();
                Simulator sim (gc, dict);
                sim.benchmark(numGames);
                return 0;
            } else if (args[i] == "--demo") {
                demo = true;
                break;
            } else if (args[i] == "--me-first") {
                meFirst = true;
                break;
            }
        }
    } catch(OptionError& e) {
        cout << usage;
        return 0;
    }

    QWidget win;
    setupWindow(win, gc, meFirst ? 0 : 1);
    gc.reset();

    if (demo) {
        Simulator sim (gc, dict);
        sim.benchmark(1);
        return app.exec();
    }

    Berry berry (gc.pin(meFirst ? 1 : 0), gc.board(), dict);
    if (!meFirst)
        berry.makeTurn();

    return app.exec();
}
