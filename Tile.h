#ifndef TILE_H
#define TILE_H

#include "globals.h"

class Tile {
    char c;
public:
    explicit Tile (char a) : c(a) {
    }
    char letter () {
        return c;
    }
    int value () {
        if ((c >= 65 && c <= 90) || (c == 97+BLANK)) return 0;
        if (c == 'e' || c == 'a' || c == 'i' || c == 'o' || c == 'n' || c == 'r'
            || c == 's' || c == 'l' || c == 't' || c == 'u')
            return 1;
        else if (c == 'd' || c == 'g') return 2;
        else if (c == 'b' || c == 'c' || c == 'm' || c == 'p') return 3;
        else if (c == 'f' || c == 'h' || c == 'v' || c == 'w' || c == 'y') return 4;
        else if (c == 'k') return 5;
        else if (c == 'j' || c == 'x') return 8;
        else if (c == 'q' || c == 'z') return 10;
        return -1;
    }
};

#endif // TILE_H
