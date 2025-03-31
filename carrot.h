#ifndef CARROT_H
#define CARROT_H

#include <SDL.h>

struct Carrot {
    SDL_Rect pos;
    SDL_Texture* texture;
    int health;
    int maxHealth;

    Carrot(int x, int y) {
        pos.x = x;
        pos.y = y;
        pos.w = 60;
        pos.h = 80;
        health = maxHealth = 100; // Máu ban đầu
        texture = nullptr;
    }

    void render(SDL_Renderer* renderer) {
        if (texture != nullptr && health > 0) {

            SDL_RenderCopy(renderer, texture, NULL, &pos);

            // Thanh máu
            int barWidth = 40;
            int barHeight = 5;
            int barX = pos.x + (pos.w - barWidth) / 2;
            int barY = pos.y - 10;

            float healthRatio = static_cast<float>(health) / maxHealth;
            int filledWidth = static_cast<int>(barWidth * healthRatio);


            SDL_Rect barOutline = {barX, barY, barWidth, barHeight};
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(renderer, &barOutline);

            SDL_Rect healthBar = {barX, barY, filledWidth, barHeight};
            int red = 255;
            int green = static_cast<int>(105 + (255 - 105) * (1 - healthRatio));
            int blue = static_cast<int>(180 + (255 - 180) * (1 - healthRatio));
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderFillRect(renderer, &healthBar);
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() const {
        return health > 0;
    }
};

#endif
