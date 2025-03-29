#ifndef ROAD_H
#define ROAD_H

#include <SDL.h>
#include <vector>
#include "Enemy.h"

struct Road {
    std::vector<SDL_Point> points;

    void move(Enemy& enemy) {
        if (!points.empty()) {
            SDL_Point target = points[0];

            if (enemy.pos.x < target.x) {
                enemy.pos.x += enemy.speed;
            } else if (enemy.pos.x > target.x) {
                enemy.pos.x -= enemy.speed;
            }

            if (enemy.pos.y < target.y) {
                enemy.pos.y += enemy.speed;
            } else if (enemy.pos.y > target.y) {
                enemy.pos.y -= enemy.speed;
            }
        }
    }

    void addPoint(int x, int y) {
        points.push_back({x, y});
    }
};

#endif
