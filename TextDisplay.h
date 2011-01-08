#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <QTextEdit>
#include "TurnInfo.h"

class TextDisplay : public QTextEdit {
    Q_OBJECT
    QTextCursor cursor;
    void addText (QString str) {
        cursor.insertText(str + "\n");
    }
public:
    TextDisplay (QWidget *parent=0) : QTextEdit (parent), cursor(document()) {
        setReadOnly(true);
    }
public slots:
    void showMove (int playId, MoveInfo mv) {
        addText("Player " + QString::number(playId+1) + ": " + QString(mv.displayText().c_str()));
    }
    void showPass (int playId) {
        addText("Player " + QString::number(playId+1) + ": PASS");
    }
    void showRoundOver () {
        addText("Round Over.");
    }
    void showIllegals (std::vector<std::string> illegals) {
        QString str = "Your play has the following illegal words: ";
        for (uint i=0; i<illegals.size(); i++) {
            str += QString(illegals[i].c_str()) + " ";
        }
        addText(str);
    }
};

#endif // TEXTDISPLAY_H
