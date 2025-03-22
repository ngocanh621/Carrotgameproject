#ifndef MOUSE_H
#define MOUSE_H

struct mouse {
    int x, y;

    void turnNorth() { y--; }
    void turnSouth() { y++; }
    void turnWest() { x--; }
    void turnEast() { x++; }
    void move() {
    }
};

#endif
