#ifndef CARROT_H
#define CARROT_H

#include <SDL.h>
struct Carrot {
    SDL_Rect pos;
    SDL_Texture* texture;

    Carrot(int x, int y) {
        pos.x = x;
        pos.y = y;
        pos.w = 80;
        pos.h = 60;
        texture = nullptr;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, NULL, &pos);
        }
    }
};

#endif
