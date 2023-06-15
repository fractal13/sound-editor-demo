// https://gist.github.com/fschr/92958222e35a823e738bb181fe045274
// mixed with
// https://gist.github.com/cicanci/b54715298ab55dff2fbcd0ca3829d13b
// and
// https://wiki.libsdl.org/SDL2/SDL_Event
// and
// https://github.com/aminosbh/sdl2-mixer-sample
// SDL2 Hello, World!
// This should display a white screen until user quits
// compile with: clang++ main.cpp -o hello_sdl2 -lSDL2
// run with: ./hello_sdl2
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char* args[]) {
  (void) argc;
  (void) args;
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
    fprintf(stderr, "SDL2_mixer could not be initialized!\n"
            "SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  Mix_Music *music = Mix_LoadMUS("audio.wav");
  if(Mix_PlayMusic(music, 1) == -1) {
    fprintf(stderr, "Music file could not be played!\n"
            "SDL_Error: %s\n", SDL_GetError());
  }

  window = SDL_CreateWindow(
			    "hello_sdl2",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,
			    SDL_WINDOW_SHOWN
			    );
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }
  screenSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);
  bool exit = false;
  SDL_Event eventData;
  while(!exit) {
    while(SDL_PollEvent(&eventData)) {
      switch(eventData.type) {
      case SDL_QUIT:
        exit = true;
        break;
      case SDL_KEYDOWN:
        switch(eventData.key.keysym.sym) {
        case SDLK_q:
          exit = true;
          break;
        case SDLK_SPACE:
          if(Mix_PausedMusic()) {
            std::cout << "Resume." << std::endl;
            Mix_ResumeMusic();
          } else {
            std::cout << "Pause." << std::endl;
            Mix_PauseMusic();
          }
          break;
        case SDLK_LEFT:
          // reload and play
          Mix_FreeMusic(music);
          music = Mix_LoadMUS("audio.wav");
          if(Mix_PlayMusic(music, 1) == -1) {
            fprintf(stderr, "Music file could not be played!\n"
                    "SDL_Error: %s\n", SDL_GetError());
          }
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    }
  }
  Mix_FreeMusic(music);
  //SDL_Delay(2000);
  SDL_DestroyWindow(window);
  Mix_CloseAudio();
  SDL_Quit();
  return 0;
}
