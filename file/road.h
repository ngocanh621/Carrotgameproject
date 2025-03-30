#ifndef ROAD_H
#define ROAD_H

#include <SDL.h>
#include <vector>
#include "Enemy.h"

struct Road {
    std::vector<SDL_Point> points;

    void move(Enemy& enemy) {
        if (points.empty()) return;

        SDL_Point target = points.front();

        int dx = target.x - enemy.pos.x;
        int dy = target.y - enemy.pos.y;

        if (abs(dx) <= enemy.speed && abs(dy) <= enemy.speed) {
            enemy.pos.x = target.x;
            enemy.pos.y = target.y;
            points.erase(points.begin());
        } else {
            if (dx > 0) enemy.pos.x += enemy.speed;
            else if (dx < 0) enemy.pos.x -= enemy.speed;
            if (dy > 0) enemy.pos.y += enemy.speed;
            else if (dy < 0) enemy.pos.y -= enemy.speed;
        }
    }

    void addPoint(int x, int y) {
        points.push_back({x, y});
    }
};

#endif
