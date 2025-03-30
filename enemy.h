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
    int maxHealth;
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
                health = maxHealth = 300;
                break;
            case TYPE2:
                speed = 1.5;
                health = maxHealth = 450;
                break;
            case TYPE3:
                speed = 1.75;
                health = maxHealth = 600;
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
        if (texture != nullptr && isAlive()) {
            SDL_RenderCopy(renderer, texture, NULL, &pos);

            // Vẽ thanh máu
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
            int red = 255; // Giữ đỏ cố định ở 255
            int green = static_cast<int>(105 + (255 - 105) * (1 - healthRatio));
            int blue = static_cast<int>(180 + (255 - 180) * (1 - healthRatio));
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderFillRect(renderer, &healthBar);
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
