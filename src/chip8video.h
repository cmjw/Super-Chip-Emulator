#ifndef CHIP8_VIDEO_H
#define CHIP8_VIDEO_H

#include <SDL2/SDL.h>

const int DISPLAY_WIDTH = 64;
const int DISPLAY_HEIGHT = 32;
const int PIXEL_SCALE = 10; 

const int KEY_ON = 1;
const int KEY_OFF = 0;

class Chip8_Video{
public:
    Chip8_Video(int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Chip8_Video();

    void Update(const void* buffer, int pitch);
    void Render();
    bool HandleInput(uint8_t* keypad);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool running;
};

#endif