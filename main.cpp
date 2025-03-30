#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphic.h"
#include "Carrot.h"
#include "Enemy.h"
#include "Tower.h"
#include <vector>

void waitUntilClickToSwitch(SDL_Rect buttonRect) {
    SDL_Event e;
    bool clicked = false;
    while (!clicked) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) clicked = true;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (e.button.x >= buttonRect.x && e.button.x <= buttonRect.x + buttonRect.w &&
                    e.button.y >= buttonRect.y && e.button.y <= buttonRect.y + buttonRect.h) {
                    clicked = true;
                }
            }
        }
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

bool canPlaceTower(int x, int y, const std::vector<SDL_Rect>& validPositions) {
    for (const auto& pos : validPositions) {
        if (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h) {
            return true;
        }
    }
    return false;
}

const SDL_Rect* getValidPosition(int x, int y, const std::vector<SDL_Rect>& validPositions) {
    for (const auto& pos : validPositions) {
        if (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h) {
            return &pos;
        }
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    SDL_Texture* background[4];
    background[0] = graphics.loadTexture("background carrot cute.png");
    background[1] = graphics.loadTexture("startbutton.png");
    background[2] = graphics.loadTexture("screen.jpg");
    background[3] = graphics.loadTexture("Pathback.jpg");

    if (!background[0] || !background[1] || !background[2] || !background[3]) {
        std::cerr << "Failed to load background textures!" << std::endl;
        graphics.quitSDL();
        return -1;
    }

    graphics.prepareScene(background[0]);
    graphics.renderTexture(background[1], 210, 240, 130, 130, graphics.renderer);
    graphics.presentScene();

    SDL_Rect buttonRect = {210, 240, 150, 150};
    waitUntilClickToSwitch(buttonRect);

    Carrot carrot(526, 145);
    carrot.texture = graphics.loadTexture("carrot.png");

    std::vector<Enemy> enemies;
    enemies.push_back(Enemy(12, 190, TYPE1, 0));
    enemies.push_back(Enemy(12, 190, TYPE2, 2000));
    enemies.push_back(Enemy(12, 190, TYPE3, 4000));
    for (auto& enemy : enemies) {
        enemy.texture = graphics.loadTexture("enemy2.png");
        // Đường đi mới
        enemy.path.setNext("right", 77);   // 101
        enemy.path.setNext("up", 65);      // 70
        enemy.path.setNext("right", 200);  // 221
        enemy.path.setNext("down", 220);   // 260
        enemy.path.setNext("right", 360);  // 382
        enemy.path.setNext("up", 150);     // 154
        enemy.path.setNext("right", WINDOW_WIDTH);
    }

    if (!carrot.texture || !enemies[0].texture) {
        std::cerr << "Failed to load object textures!" << std::endl;
        graphics.quitSDL();
        return -1;
    }


    std::vector<SDL_Rect> validTowerPositions = {
        {120, 200, 30, 30},
        {50, 150, 50, 50},
        {450, 50, 50, 50},
        {450, 150, 50, 50},
        {450, 300, 50, 50}
    };


    std::vector<bool> positionOccupied(validTowerPositions.size(), false);

    std::vector<Tower> towers;
    bool quit = false;
    SDL_Event event;

    SDL_Texture* towerPlaceholderTexture = graphics.loadTexture("plus.png");

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

                const SDL_Rect* clickedPosition = getValidPosition(event.button.x, event.button.y, validTowerPositions);

                if (clickedPosition) {

                    size_t index = 0;
                    for (size_t i = 0; i < validTowerPositions.size(); ++i) {
                        if (&validTowerPositions[i] == clickedPosition) {
                            index = i;
                            break;
                        }
                    }


                    if (!positionOccupied[index]) {
                        Tower newTower(clickedPosition->x, clickedPosition->y);
                        newTower.texture = graphics.loadTexture("tower.png");
                        towers.push_back(newTower);
                        positionOccupied[index] = true;
                    } else {
                        std::cout << "This position already has a tower!" << std::endl;
                    }
                } else {
                    std::cout << "Cannot place tower here!" << std::endl;
                }
            }
        }

        for (auto& enemy : enemies) {
            if (currentTime >= enemy.spawnTime) {
                enemy.move();
                for (auto& tower : towers) {
                    tower.attack(graphics.renderer, enemy, currentTime);
                }
                if (checkCollision(enemy.getCollider(), carrot.pos)) {
                    std::cout << "Game Over! Carrot was destroyed!" << std::endl;
                    quit = true;
                }
            }
        }

        bool allDead = true;
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                allDead = false;
                break;
            }
        }
        if (allDead) {
            std::cout << "You Win! All enemies defeated!" << std::endl;
            quit = true;
        }

        graphics.prepareScene(background[2]);
        graphics.renderTexture(background[3], 0, 0, 600, 400, graphics.renderer);
        carrot.render(graphics.renderer);
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && currentTime >= enemy.spawnTime) {
                enemy.render(graphics.renderer);
            }
        }

        for (size_t i = 0; i < validTowerPositions.size(); ++i) {
            if (!positionOccupied[i]) {
                graphics.renderTexture(towerPlaceholderTexture, validTowerPositions[i].x, validTowerPositions[i].y, 30, 30, graphics.renderer); // Kích thước 25x25
            }
        }

        for (auto& tower : towers) {
            tower.render(graphics.renderer);
        }

        graphics.presentScene();
    }

    SDL_DestroyTexture(towerPlaceholderTexture);
    graphics.quitSDL();
    SDL_Quit();
    return 0;
}
