#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphic.h"
#include "Carrot.h"
#include "Enemy.h"
#include "Road.h"
#include "Tower.h"
#include <vector>

void waitUntilClicktoClose() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
}

void waitUntilClickToSwitch(SDL_Rect buttonRect) {
    SDL_Event e;
    bool clicked = false;

    while (!clicked) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                clicked = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    if (e.button.x >= buttonRect.x && e.button.x <= buttonRect.x + buttonRect.w &&
                        e.button.y >= buttonRect.y && e.button.y <= buttonRect.y + buttonRect.h) {
                        clicked = true;
                    }
                }
            }
        }
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    SDL_Texture* background[4];
    background[0] = graphics.loadTexture("background carrot cute.png");
    background[1] = graphics.loadTexture("startbutton.png");
    background[2] = graphics.loadTexture("screen.jpg");
    background[3] = graphics.loadTexture("path.jpg");

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

    Carrot carrot(550, 20);
    carrot.texture = graphics.loadTexture("carrot.png");

    Enemy enemy1(50, 20, TYPE1);
    enemy1.texture = graphics.loadTexture("enemy.png");

    Enemy enemy2(50, 20, TYPE2);
    enemy2.texture = graphics.loadTexture("enemy.png");

    Enemy enemy3(50, 20, TYPE3);
    enemy3.texture = graphics.loadTexture("enemy.png");

    if (!carrot.texture || !enemy1.texture || !enemy2.texture || !enemy3.texture) {
        std::cerr << "Failed to load object textures!" << std::endl;
        graphics.quitSDL();
        return -1;
    }

    Road path;
    path.addPoint(50, 20);
    path.addPoint(50, 300);
    path.addPoint(550, 20);

    std::vector<Tower> towers;
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                Tower newTower(event.button.x, event.button.y);
                newTower.texture = graphics.loadTexture("tower.png");
                towers.push_back(newTower);
            }
        }

        path.move(enemy1);
        path.move(enemy2);
        path.move(enemy3);

        Uint32 currentTime = SDL_GetTicks();
        for (auto& tower : towers) {
            tower.attack(graphics.renderer, enemy1, currentTime);
            tower.attack(graphics.renderer, enemy2, currentTime);
            tower.attack(graphics.renderer, enemy3, currentTime);
        }

        if (checkCollision(enemy1.getCollider(), carrot.pos) ||
            checkCollision(enemy2.getCollider(), carrot.pos) ||
            checkCollision(enemy3.getCollider(), carrot.pos)) {
            std::cout << "Game Over! Carrot was destroyed!" << std::endl;
            quit = true;
        }

        if (!enemy1.isAlive() && !enemy2.isAlive() && !enemy3.isAlive()) {
            std::cout << "You Win! All enemies defeated!" << std::endl;
            quit = true;
        }

        graphics.prepareScene(background[2]);
        graphics.renderTexture(background[3], 0, 0, 600, 400, graphics.renderer);
        carrot.render(graphics.renderer);
        if (enemy1.isAlive()) enemy1.render(graphics.renderer);
        if (enemy2.isAlive()) enemy2.render(graphics.renderer);
        if (enemy3.isAlive()) enemy3.render(graphics.renderer);
        for (auto& tower : towers) {
            tower.render(graphics.renderer);
        }

        graphics.presentScene();
    }

    graphics.quitSDL();
    SDL_Quit();
    return 0;
}
