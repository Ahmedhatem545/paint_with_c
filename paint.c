#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 1400
#define HEIGHT 900

struct Circle {
  float x, y;
  float radius;
  float dx;
  float dy;
  bool clicked;
};

void FillCircle(SDL_Renderer *renderer, struct Circle circle ) {
  double radius_squared = circle.radius * circle.radius;
  for (double x = circle.x - circle.radius; x <= circle.x + circle.radius;
       x++) {
    for (double y = circle.y - circle.radius; y <= circle.y + circle.radius;
         y++) {
      double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
      if (distance_squared < radius_squared && circle.clicked) {

        SDL_Rect pixel = {x, y, 1, 1};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  bool running = true;
  SDL_Event event;

  struct Circle circle;
  circle.radius = 100;
  SDL_Window *window =
      SDL_CreateWindow("PAINT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("window error %s\n", SDL_GetError);
    return 1;
  }
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    printf("renderer error %s\n", SDL_GetError);
    return 1;
  }

  while (running) {
    while (SDL_PollEvent(&event)) {

      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        circle.x = event.motion.x;
        circle.y = event.motion.y;
        circle.clicked=true;

      }
      if(!SDL_MOUSEBUTTONDOWN){
         circle.clicked=false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    FillCircle(renderer, circle);
    SDL_RenderPresent(renderer);

    SDL_Delay(10);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
