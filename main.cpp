#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphic.h"
#include "Carrot.h"
#include "Enemy.h"
#include "Path.h"
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

    SDL_Texture* background[4];
    background[0] = graphics.loadTexture("background carrot.png");
    background[1] = graphics.loadTexture("startbutton.png");
    background[2] = graphics.loadTexture("screen.jpg");
    background[3] = graphics.loadTexture("level1.jpg");


    graphics.prepareScene(background[0]);
    graphics.presentScene();

    graphics.renderTexture(background[1], 290, 300, 150, 150, graphics.renderer);
    graphics.presentScene();

    waitUntilClicktoClose();

    SDL_Rect buttonRect = {290, 300, 150, 150};
    waitUntilClickToSwitch(buttonRect);

    graphics.renderTexture(background[2], 0, 0, 768, 450, graphics.renderer);
    graphics.presentScene();

    Carrot carrot(290, 300);
    carrot.texture = graphics.loadTexture("carrot.png");

    Enemy enemy1(100, 0, TYPE1);
    enemy1.texture = graphics.loadTexture("enemy.png");

    Enemy enemy2(200, 0, TYPE2);
    enemy2.texture = graphics.loadTexture("enemy.png");

    Enemy enemy3(300, 0, TYPE3);
    enemy3.texture = graphics.loadTexture("enemy.png");

    Path path;
    path.addPoint(100, 300);
    path.addPoint(200, 250);
    path.addPoint(300, 200);
    path.addPoint(400, 150);
    path.addPoint(500, 100);


    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        path.move(enemy1.pos);  // Di chuyển kẻ thù 1
        path.move(enemy2.pos);  // Di chuyển kẻ thù 2
        path.move(enemy3.pos);  // Di chuyển kẻ thù 3


        graphics.prepareScene(background[2]);
        graphics.renderTexture(background[3], 96, 56, 576, 338 , graphics.renderer);
        carrot.render(graphics.renderer);
        enemy1.render(graphics.renderer);


        graphics.presentScene();
    }

    graphics.quitSDL();
    SDL_Quit();
    return 0;
}
