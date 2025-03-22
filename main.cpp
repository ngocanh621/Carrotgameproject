#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphic.h"
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
int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    SDL_Texture* background[2];
    background[0] = graphics.loadTexture("background carrot.png");
    background[1] = graphics.loadTexture("startbutton.png");

    graphics.prepareScene(background[0]);
    graphics.presentScene();

    graphics.renderTexture(background[1], 290, 330, 150, 150, graphics.renderer);
    graphics.presentScene();
    waitUntilClicktoClose();




    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);



    graphics.quitSDL();
    SDL_Quit();
    return 0;
}
}
