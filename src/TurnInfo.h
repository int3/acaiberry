#ifndef MOVEINFO_H
#define MOVEINFO_H

#include <string>
#include <vector>
#include "globals.h"
#include "TileRepo.h"

enum turnType {
    MOVE, CHANGE, PASS
};

class TurnInfo {
public:
    virtual turnType type () const = 0;
    virtual std::string displayText () const = 0;
};

class Board;
class Dict;

class MoveInfo : public TurnInfo {
private:
    int coord[2];
    int _orient;
    std::string _word;
    int _score;
    void calcScore (Board &bd);
    Rack _usedTiles;
    bool _isLegal;
    std::vector<std::string> illegals;
public:
    MoveInfo ();
    MoveInfo (int a, int b, int c, std::string s, Board &bd);
    void calcLegal (Board &bd, Dict &dict);
    const std::string& word() const;
    const Rack& usedTiles () const;
    bool isLegal () const;
    std::vector<std::string> getIllegals();
    int pos(int a) const;
    int orient () const;
    int score () const;
    turnType type () const;
    std::string displayText () const;
};

inline const std::string& MoveInfo::word() const {
    return _word;
}
inline int MoveInfo::pos(int a) const {
    return coord[a];
}
inline int MoveInfo::orient () const {
    return _orient;
}
inline int MoveInfo::score () const {
    return _score;
}

class PassInfo : public TurnInfo {
public:
    turnType type () const;
    std::string displayText () const;
};

inline turnType PassInfo::type () const {
    return PASS;
}

/// TODO: implement @class ChangeInfo

#endif // MOVEINFO_H
