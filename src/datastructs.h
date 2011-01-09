#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <string>
#include <deque>
#include "globals.h"

/**
  @brief Board coordinates
**/
class Coord {
    int p[2];
public:
    Coord (int a, int b) {
        p[0] = a;
        p[1] = b;
    }
    Coord (int q[2]) {
        p[0] = q[0];
        p[1] = q[1];
    }
    Coord () {
        p[0] = 0;
        p[1] = 0;
    }
    int operator[] (int n) const {
        return p[n];
    }
    int& operator[] (int n) {
        return p[n];
    }
    Coord tAdd (int ori, int amt) {
        Coord q(*this);
        q[ori] += amt;
        return q;
    }
};

/**
  @brief For move-generating algorithm to store the characters that could legally be placed on a given square
 **/
class Constraint {
    uint cons;
public:
    Constraint () : cons(0) {
    }
    void addLetter (char c) { /// @param c is in INDEX form
        cons |= 1 << c;
    }
    bool hasLetter (char c) { /// @param c is in INDEX form
        return cons & 1 << c;
    }
    void setAll () {
        cons = 0x7FFFFFF;
    }
    void setNone () {
        cons = 0;
    }
};

/**
  @brief Essentially a deque that masquarades as a string.
**/
class BiString {
    std::deque<char> bstr;
public:
    void push_back (char c) {
        bstr.push_back(c);
    }
    void push_front (char c) {
        bstr.push_front(c);
    }
    void clear () {
        bstr.clear();
    }
    uint size () {
        return bstr.size();
    }
    char& operator[] (uint n) {
        return bstr[n];
    }
    std::string toStr () {
        std::string res;
        for (uint i=0; i<bstr.size(); i++)
            res.push_back(bstr[i]);
        return res;
    }
};

#endif // DATASTRUCTS_H
