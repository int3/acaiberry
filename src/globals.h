#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdexcept>

enum {
    MAXWORDLEN = 15,
    BOARDSIZE = 15,
    RACKSIZE = 7,
    CHARNUM = 27,
    ALPHANUM = 26,
    BLANK = 26,
    X = 0, Y = 1,
    TWS, DWS, TLS, DLS, NIL
};

enum {
    HORIZONTAL = 1, VERTICAL = 0,
    BACKWARDS = 0, FORWARDS = 1
};

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

inline uint toindex (char c) {
    if (c > 64 && c < 91) // uppercase
        return c-65;
    if (c > 96 && c < 123)
        return c-97;
    throw std::runtime_error("toindex() input out of range");
}

inline uint toindex2 (char c) {
    if ((c > 64 && c < 91) || c == '?')
        return BLANK;
    if (c > 96 && c < 123)
        return c-97;
    throw std::runtime_error("toindex2() input out of range");
}

class bad_input : public std::exception {
    std::string where;
    std::string info;
public:
    bad_input (std::string wh = "", std::string inf = "") throw() :  where(wh), info(inf) {
    }
    ~bad_input () throw() {
    }
    const char *what () const throw () {
        std::string tmp;
        tmp = "Bad input to " + where + ". " + info;
        return tmp.c_str();
    }
};

#endif // GLOBALS_H
