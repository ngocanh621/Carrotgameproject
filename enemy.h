#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include "map_direction.h"
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
    Uint32 spawnTime;
    MapDirections path;

    Enemy(int x, int y, EnemyType t, Uint32 st = 0) {
        pos.x = x;
        pos.y = y;
        pos.w = 50;
        pos.h = 50;
        type = t;
        spawnTime = st;

        switch (type) {
            case TYPE1:
                speed = 1;
                health = 100;
                break;
            case TYPE2:
                speed = 1.5;
                health = 150;
                break;
            case TYPE3:
                speed = 2;
                health = 200;
                break;
        }
        texture = nullptr;
    }

    void move() {
        if (path.isEnd()) return;

        std::string dir = path.getDir();
        int stop = path.getStop();

        if (dir == "right" && pos.x < stop) {
            pos.x += speed;
            if (pos.x >= stop) path.next();
        } else if (dir == "up" && pos.y > stop) {
            pos.y -= speed;
            if (pos.y <= stop) path.next();
        } else if (dir == "down" && pos.y < stop) {
            pos.y += speed;
            if (pos.y >= stop) path.next();
        }
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, NULL, &pos);
        }
    }

    SDL_Rect getCollider() { return pos; }
    bool isAlive() const { return health > 0; }
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
};

#endif
