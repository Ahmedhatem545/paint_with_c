#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 1400
#define HEIGHT 900

struct Color {
  Uint8 r, g, b, a;
};

struct Circle {
  float x, y;
  float radius;
  bool clicked;
  struct Color color;
};

struct Color_rect {
  int width, height;
  int x, y;
  struct Color color;
};

void Fill_Rect(struct Color_rect rect, SDL_Renderer *renderer) {
  SDL_Rect sdl_rect = {rect.x, rect.y, rect.width, rect.height};

  SDL_SetRenderDrawColor(renderer, rect.color.r, rect.color.g, rect.color.b,
                         rect.color.a);

  SDL_RenderFillRect(renderer, &sdl_rect);
}

void FillCircle(SDL_Renderer *renderer, struct Circle circle, bool erasing) {
  int radius_squared = circle.radius * circle.radius;

  for (int x = circle.x - circle.radius; x <= circle.x + circle.radius; x++) {
    for (int y = circle.y - circle.radius; y <= circle.y + circle.radius; y++) {
      int dx = x - circle.x;
      int dy = y - circle.y;

      if (dx * dx + dy * dy <= radius_squared && circle.clicked) {
        if (erasing) {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        } else {
          SDL_SetRenderDrawColor(renderer, circle.color.r, circle.color.g,
                                 circle.color.b, circle.color.a);
        }

        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }
}

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    printf("Window Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  if (!renderer) {
    printf("Renderer Error: %s\n", SDL_GetError());
    return 1;
  }

  struct Color red = {255, 0, 0, 255};
  struct Color green = {0, 255, 0, 255};
  struct Color blue = {0, 0, 255, 255};
  struct Color yellow = {255, 255, 0, 255};
  struct Color magenta = {255, 0, 255, 255};
  struct Color cyan = {0, 255, 255, 255};
  struct Color white = {255, 255, 255, 255};
  struct Color gray = {128, 128, 128, 255};
  struct Color_rect toolbar = {
      .width = WIDTH, .height = 45, .x = 0, .y = 0, .color = gray};

  struct Color_rect palette[] = {
      {.width = 40, .height = 40, .x = 0, .y = 0, .color = red},
      {.width = 40, .height = 40, .x = 40, .y = 0, .color = green},
      {.width = 40, .height = 40, .x = 80, .y = 0, .color = blue},
      {.width = 40, .height = 40, .x = 120, .y = 0, .color = yellow},
      {.width = 40, .height = 40, .x = 160, .y = 0, .color = magenta},
      {.width = 40, .height = 40, .x = 200, .y = 0, .color = cyan},
      {.width = 40, .height = 40, .x = 240, .y = 0, .color = white},
  };

  int palette_count = sizeof(palette) / sizeof(palette[0]);

  struct Circle brush = {0};
  brush.radius = 5;
  brush.color = white;

  bool running = true;
  bool erasing = false;

  SDL_Event event;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_e) {
          erasing = !erasing;
        }
      }
      if (event.type == SDL_KEYDOWN){
         if(event.key.keysym.sym == SDLK_UP){
            brush.radius +=5;
         }
      }
       if (event.type == SDL_KEYDOWN){
         if(event.key.keysym.sym == SDLK_DOWN){
            brush.radius -=5;
         }
      }

           
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;

        for (int i = 0; i < palette_count; i++) {
          if (mx >= palette[i].x && mx <= palette[i].x + palette[i].width &&
              my >= palette[i].y && my <= palette[i].y + palette[i].height) {
            brush.color = palette[i].color;
          }
        }
      }

      if (event.type == SDL_MOUSEMOTION &&
          (event.motion.state & SDL_BUTTON_LMASK) &&
          event.motion.y > toolbar.height) {
        brush.x = event.motion.x;
        brush.y = event.motion.y;
        brush.clicked = true;
      }

      if (event.type == SDL_MOUSEBUTTONUP) {
        brush.clicked = false;
      }
    }

    Fill_Rect(toolbar, renderer);

    for (int i = 0; i < palette_count; i++) {
      Fill_Rect(palette[i], renderer);
    }

    FillCircle(renderer, brush, erasing);

    SDL_RenderPresent(renderer);
    SDL_Delay(2);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
