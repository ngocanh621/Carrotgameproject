#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

enum EnemyType {
    TYPE1,
    TYPE2,
    TYPE3
};

struct Enemy {
    SDL_Texture* texture;
    SDL_Rect pos;
    int speed;
    int health;
    EnemyType type;

    Enemy(int x, int y, EnemyType t) {
        pos.x = x;
        pos.y = y;
        pos.w = 50;
        pos.h = 50;
        type = t;


        switch (type) {
            case TYPE1:
                speed = 2;
                health = 100;
            case TYPE2:
                speed = 4;
                health = 150;
                break;
            case TYPE3:
                speed = 6;
                health = 200;
                break;
        }
        texture = nullptr;
    }

    void move() {
        pos.y += speed;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, NULL, &pos);
        }
    }

    SDL_Rect getCollider() {
        return pos;
    }

    bool isAlive() {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
};

#endif
