#ifndef TOWER_H
#define TOWER_H

#include <SDL.h>
#include "Enemy.h"
#include <cmath>

//dinh nghia nam
enum TowerType {
    BASIC,
    FAST,
    STRONG
};

struct Tower {
    SDL_Rect pos;
    SDL_Texture* texture;
    int attackRange; // vung tan cong
    int attackDamage; //sat thuong
    int attackSpeed;  // thoi gian giua 2 lan tan cong (mili giay)
    Uint32 lastAttackTime; // lan cuoi tan cong
    TowerType type;

    // khoi tao mac dinh cua thap
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

    // khoi tao co so
    Tower(int x, int y, TowerType t = BASIC) {
        pos.x = x;
        pos.y = y;
        pos.w = 60;
        pos.h = 60;
        type = t;

        // tung loai thap
        switch (type) {
            case BASIC:
                attackRange = 150;
                attackDamage = 15;
                attackSpeed = 1000;
                break;
            case FAST:
                attackRange = 100;
                attackDamage = 10;
                attackSpeed = 500;
                break;
            case STRONG:
                attackRange = 250;
                attackDamage = 30;
                attackSpeed = 1000;
                break;
        }
        lastAttackTime = 0;
        texture = nullptr;
    }

    // ve thap
    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, NULL, &pos);
    }

    // kiem tra enemy co trong vung tan cong k
    bool checkCollision(Enemy& enemy) {
        int distanceX = enemy.pos.x - pos.x;
        int distanceY = enemy.pos.y - pos.y;
        int distance = sqrt(distanceX * distanceX + distanceY * distanceY);
        return distance <= attackRange;
    }

    // ham tan cong
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
