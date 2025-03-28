#ifndef PATH_H
#define PATH_H

#include <SDL.h>
#include <vector>

struct Path {
    std::vector<SDL_Point> points;
    int currentPoint = 0;

    void addPoint(int x, int y) {
        points.push_back({x, y});
    }

    void move(SDL_Rect& pos) {
        if (currentPoint < points.size()) {
            SDL_Point target = points[currentPoint];
            int dx = target.x - pos.x;
            int dy = target.y - pos.y;
            if (abs(dx) > 1 || abs(dy) > 1) {
                pos.x += dx / 10;
                pos.y += dy / 10;
            } else {
                currentPoint++;
            }
        }
    }
};

#endif

