#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphic.h"
#include "Carrot.h"
#include "Enemy.h"
#include "Level.h"  // Bao gồm Level.h để quản lý các cấp độ

void waitUntilClicktoClose(){
    bool quit = false;
    SDL_Event e;
    while(!quit){
        SDL_PollEvent(&e);
        if(e.type == SDL_QUIT){
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

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    // Tải các hình nền
    SDL_Texture* background[3];
    background[0] = graphics.loadTexture("background carrot.png");
    background[1] = graphics.loadTexture("startbutton.png");
    background[2] = graphics.loadTexture("screen.jpg");

    graphics.prepareScene(background[0]);
    graphics.presentScene();

    graphics.renderTexture(background[1], 290, 300, 150, 150, graphics.renderer);
    graphics.presentScene();

    waitUntilClicktoClose();

    SDL_Rect buttonRect = {290, 300, 150, 150};
    waitUntilClickToSwitch(buttonRect);

    Level level1("level1.jpg", graphics.renderer);
    level1.generateEnemies();  // Tạo các kẻ thù cho cấp độ 1

    graphics.renderTexture(background[2], 0, 0, 768, 450, graphics.renderer);
    graphics.presentScene();

    Carrot carrot(290, 300);
    carrot.texture = graphics.loadTexture("carrot.png");


    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        level1.moveEnemies();

        graphics.prepareScene(background[2]);
        level1.renderEnemies(graphics.renderer);
        graphics.renderTexture(level1.backgroundTexture, 96, 56, 576, 338 , graphics.renderer);
            carrot.render(graphics.renderer);


        graphics.presentScene();
    }

    graphics.quitSDL();
    SDL_Quit();
    return 0;
}

