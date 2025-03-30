#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "def.h"
#include "graphic.h"
#include "Carrot.h"
#include "Enemy.h"
#include "Tower.h"
#include <vector>

int playerMoney = 500;
int playerScore = 0;
const int TOWER_COSTS[] = {50, 75, 100};

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

void setupEnemyPath(Enemy& enemy) {
    enemy.path.setNext("right", 77);
    enemy.path.setNext("up", 65);
    enemy.path.setNext("right", 200);
    enemy.path.setNext("down", 220);
    enemy.path.setNext("right", 360);
    enemy.path.setNext("up", 150);
    enemy.path.setNext("right", WINDOW_WIDTH);
}

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return 1;
    }
    Mix_Music* backgroundMusic = Mix_LoadMUS("back sound.wav");
      if(backgroundMusic == nullptr){
        SDL_Log("Failed to load background music! SDL_mixer Error: %s", Mix_GetError());
        return 1;
      }
      Mix_VolumeMusic(64);
      if(Mix_PlayMusic(backgroundMusic, -1)== -1){
        SDL_Log("Failed to play background music! SDL_mixer Error: %s", Mix_GetError());
        return 1;
      }
    Mix_Chunk* enemyDeathSound = Mix_LoadWAV("bye sound.wav"); // Đường dẫn tới file âm thanh
    if (enemyDeathSound == nullptr) {
        SDL_Log("Failed to load enemy death sound! SDL_mixer Error: %s", Mix_GetError());
        return 1;
    }
    Mix_VolumeChunk(enemyDeathSound, 64);

    SDL_Texture* background[4];
    background[0] = graphics.loadTexture("background carrot cute.png");
    background[1] = graphics.loadTexture("startbutton.png");
    background[3] = graphics.loadTexture("Pathback.jpg");

    if (!background[0] || !background[1] || !background[3]) {
        std::cerr << "Failed to load background textures!" << std::endl;
        graphics.quitSDL();
        return -1;
    }

    graphics.prepareScene(background[0]);
    graphics.renderTexture(background[1], 210, 240, 130, 130, graphics.renderer);
    graphics.presentScene();

    SDL_Rect buttonRect = {210, 240, 150, 150};
    waitUntilClickToSwitch(buttonRect);

    Carrot carrot(530, 130);
    carrot.texture = graphics.loadTexture("carrot.png");

    SDL_Texture* enemyType1Texture = graphics.loadTexture("baby snail.png");
    SDL_Texture* enemyType2Texture = graphics.loadTexture("baby chicken.png");
    SDL_Texture* enemyType3Texture = graphics.loadTexture("baby rabbit.png");

    std::vector<Enemy> enemies;
    std::cout << "Wave 1 incoming!" << std::endl;
    enemies.push_back(Enemy(12, 190, TYPE1, 0));
    enemies.push_back(Enemy(12, 190, TYPE2, 4000));
    enemies.push_back(Enemy(12, 190, TYPE3, 12000));
    std::cout << "Wave 2 will arrive in 10 seconds!" << std::endl;
    enemies.push_back(Enemy(12, 190, TYPE2, 18000));
    enemies.push_back(Enemy(12, 190, TYPE2, 20000));
    enemies.push_back(Enemy(12, 190, TYPE3, 22000));
    std::cout << "Wave 3 will arrive in 20 seconds!" << std::endl;
    enemies.push_back(Enemy(12, 190, TYPE3, 30000));
    enemies.push_back(Enemy(12, 190, TYPE2, 32000));
    enemies.push_back(Enemy(12, 190, TYPE1, 34000));
    for (auto& enemy : enemies) {
        switch (enemy.type) {
            case TYPE1: enemy.texture = enemyType1Texture; break;
            case TYPE2: enemy.texture = enemyType2Texture; break;
            case TYPE3: enemy.texture = enemyType3Texture; break;
        }
        setupEnemyPath(enemy);
    }

    if (!carrot.texture || !enemyType1Texture || !enemyType2Texture || !enemyType3Texture) {
        std::cerr << "Failed to load object textures!" << std::endl;
        graphics.quitSDL();
        return -1;
    }

    std::vector<SDL_Rect> validTowerPositions = {
        {120, 180, 50, 50},
        {30, 150, 50, 50},
        {245, 100, 50, 50},
        {450, 100, 50, 50},
        {410, 205, 50, 50},
        {280, 180, 50, 50},
        {140, 30, 50, 50}
    };

    std::vector<bool> positionOccupied(validTowerPositions.size(), false);
    std::vector<Tower> towers;
    bool quit = false;
    SDL_Event event;

    SDL_Texture* towerPlaceholderTexture = graphics.loadTexture("plus.png");
    SDL_Texture* basicTowerTexture = graphics.loadTexture("mushroom.png");
    SDL_Texture* fastTowerTexture = graphics.loadTexture("mushroom1.png");
    SDL_Texture* strongTowerTexture = graphics.loadTexture("mushroom2.png");

    auto placeTower = [&](int x, int y, size_t index) {
        std::cout << "Money: " << playerMoney << " - 1:Basic(50), 2:Fast(75), 3:Strong(100)\n";
        bool typeSelected = false;
        TowerType selectedType;

        while (!typeSelected && !quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) { quit = true; return; }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            if (playerMoney >= TOWER_COSTS[BASIC]) {
                                selectedType = BASIC;
                                typeSelected = true;
                                playerMoney -= TOWER_COSTS[BASIC];
                            }
                            break;
                        case SDLK_2:
                            if (playerMoney >= TOWER_COSTS[FAST]) {
                                selectedType = FAST;
                                typeSelected = true;
                                playerMoney -= TOWER_COSTS[FAST];
                            }
                            break;
                        case SDLK_3:
                            if (playerMoney >= TOWER_COSTS[STRONG]) {
                                selectedType = STRONG;
                                typeSelected = true;
                                playerMoney -= TOWER_COSTS[STRONG];
                            }
                            break;
                    }
                }
            }
        }

        if (!quit && typeSelected) {
            Tower newTower(x, y, selectedType);
            switch (selectedType) {
                case BASIC: newTower.texture = basicTowerTexture; break;
                case FAST: newTower.texture = fastTowerTexture; break;
                case STRONG: newTower.texture = strongTowerTexture; break;
            }
            towers.push_back(newTower);
            positionOccupied[index] = true;
        }
    };

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
                        placeTower(clickedPosition->x, clickedPosition->y, index);
                    } else {
                        std::cout << "This position already has a Mushroom!" << std::endl;
                    }
                } else {
                    std::cout << "Cannot place Mushroom here!" << std::endl;
                }
            }
        }

        for (auto& enemy : enemies) {
            if (currentTime >= enemy.spawnTime) {
                enemy.move();
                if (checkCollision(enemy.getCollider(), carrot.pos)) {
                    std::cout << "Game Over! Carrot was destroyed!" << std::endl;
                    quit = true;
                }
            }
        }

        for (auto& tower : towers) {
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && currentTime >= enemy.spawnTime) {
                    tower.attack(graphics.renderer, enemy, currentTime);
                    if (!enemy.isAlive() && enemy.health == 0) {
                        switch (enemy.type) {
                            case TYPE1: playerScore += 10; playerMoney += 5; break;
                            case TYPE2: playerScore += 15; playerMoney += 8; break;
                            case TYPE3: playerScore += 20; playerMoney += 10; break;
                        }
                        enemy.health = -1;
                        Mix_PlayChannel(-1, enemyDeathSound, 0);
                    }
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

        graphics.prepareScene(background[3]);
        carrot.render(graphics.renderer);
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && currentTime >= enemy.spawnTime) {
                enemy.render(graphics.renderer);
            }
        }
        for (size_t i = 0; i < validTowerPositions.size(); ++i) {
            if (!positionOccupied[i]) {
                graphics.renderTexture(towerPlaceholderTexture, validTowerPositions[i].x, validTowerPositions[i].y, 30, 30, graphics.renderer);
            }
        }
        for (auto& tower : towers) {
            tower.render(graphics.renderer);
        }


        char scoreText[50];
        sprintf(scoreText, "Score: %d Money: %d", playerScore, playerMoney);
        graphics.renderText(scoreText, 10, 350);
        graphics.presentScene();
    }

    SDL_DestroyTexture(towerPlaceholderTexture);
    SDL_DestroyTexture(basicTowerTexture);
    SDL_DestroyTexture(fastTowerTexture);
    SDL_DestroyTexture(strongTowerTexture);
    SDL_DestroyTexture(enemyType1Texture);
    SDL_DestroyTexture(enemyType2Texture);
    SDL_DestroyTexture(enemyType3Texture);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(enemyDeathSound);
    Mix_CloseAudio();
    graphics.quitSDL();
    SDL_Quit();
    return 0;
}
