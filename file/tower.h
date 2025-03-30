#ifndef TOWER_H
#define TOWER_H

#include <SDL.h>
#include "Enemy.h"
#include <cmath>

enum TowerType {
    BASIC,
    FAST,
    STRONG
};

struct Tower {
    SDL_Rect pos;
    SDL_Texture* texture;
    int attackRange;
    int attackDamage;
    int attackSpeed;
    Uint32 lastAttackTime;
    TowerType type;

    Tower() {
        pos.x = 0;
        pos.y = 0;
        pos.w = 60;
        pos.h = 60;
        attackRange = 150;
        attackDamage = 10;
        attackSpeed = 1000;
        lastAttackTime = 0;
        type = BASIC;
        texture = nullptr;
    }

    Tower(int x, int y, TowerType t = BASIC) {
        pos.x = x;
        pos.y = y;
        pos.w = 60;
        pos.h = 60;
        type = t;

        switch (type) {
            case BASIC:
                attackRange = 150;
                attackDamage = 10;
                attackSpeed = 1000;
                break;
            case FAST:
                attackRange = 100;
                attackDamage = 5;
                attackSpeed = 500;
                break;
            case STRONG:
                attackRange = 200;
                attackDamage = 20;
                attackSpeed = 1500;
                break;
        }
        lastAttackTime = 0;
        texture = nullptr;
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
