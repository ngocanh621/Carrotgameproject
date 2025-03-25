// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

struct Enemy {
    SDL_Texture* texture;
    SDL_Rect pos;
    int speed;
    Enemy(int x, int y, int s) {
        pos.x = x;
        pos.y = y;
        pos.w = 50;
        pos.h = 50;
        speed = s;
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
};

#endif
