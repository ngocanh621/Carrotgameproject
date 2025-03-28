#ifndef TOWER_H
#define TOWER_H

#include <SDL.h>
#include "Enemy.h"
#include <cmath>

struct Tower {
    SDL_Rect pos;
    SDL_Texture* texture;
    int attackRange;
    int attackDamage;
    int attackSpeed;
    Uint32 lastAttackTime;

    Tower(int x, int y) {
        pos.x = x;
        pos.y = y;
        pos.w = 64;
        pos.h = 64;
        attackRange = 150;
        attackDamage = 10;
        attackSpeed = 1000;
        lastAttackTime = 0;
    }

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, NULL, &pos);
    }

    bool checkCollision(Enemy& enemy) {
        int distanceX = enemy.pos.x - pos.x;
        int distanceY = enemy.pos.y - pos.y;
        int distance = sqrt(distanceX * distanceX + distanceY * distanceY);

        return distance <= attackRange;
    }

    void attack(SDL_Renderer* renderer, Enemy& enemy, Uint32 currentTime) {
        if (currentTime - lastAttackTime >= attackSpeed) {
            if (checkCollision(enemy)) {
                enemy.takeDamage(attackDamage);
                lastAttackTime = currentTime;
            }
        }
    }
};

#endif
