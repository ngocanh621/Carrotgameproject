#ifndef LEVEL_H
#define LEVEL_H

#include "Enemy.h"
#include <vector>

struct Level {
    SDL_Texture* backgroundTexture;
    std::vector<Enemy> enemies;

    Level(const char* backgroundFile, SDL_Renderer* renderer) {
        backgroundTexture = IMG_LoadTexture(renderer, backgroundFile);
    }

    void generateEnemies() {
        enemies.push_back(Enemy(100, 0, TYPE1));
        enemies.push_back(Enemy(200, 0, TYPE2));
        enemies.push_back(Enemy(300, 0, TYPE3));
        enemies.push_back(Enemy(400, 0, TYPE1));
    }

    void moveEnemies() {
        for (auto& enemy : enemies) {
            enemy.move();
        }
    }

    void renderEnemies(SDL_Renderer* renderer) {
        for (auto& enemy : enemies) {
            enemy.render(renderer);
        }
    }
};

#endif
