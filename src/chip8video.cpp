#include "chip8video.h"

Chip8_Video::Chip8_Video(int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
    // initialize SDL video
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip-8 Emulator", 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

Chip8_Video::~Chip8_Video() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * Update the video display.
 */
void Chip8_Video::Update(const void* buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

/**
 * Handle keypad input.
 */
bool Chip8_Video::HandleInput(uint8_t* keypad) {
    bool quit = false;
    SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				quit = true;
			} break;

			case SDL_KEYDOWN: {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: {
						quit = true;
					} break;

					case SDLK_x: {
						keypad[0] = KEY_ON;
					} break;

					case SDLK_1: {
						keypad[1] = KEY_ON;
					} break;

					case SDLK_2: {
						keypad[2] = KEY_ON;
					} break;

					case SDLK_3: {
						keypad[3] = KEY_ON;
					} break;

					case SDLK_q: {
						keypad[4] = KEY_ON;
					} break;

					case SDLK_w: {
						keypad[5] = KEY_ON;
					} break;

					case SDLK_e: {
						keypad[6] = KEY_ON;
					} break;

					case SDLK_a: {
						keypad[7] = KEY_ON;
					} break;

					case SDLK_s: {
						keypad[8] = KEY_ON;
					} break;

					case SDLK_d: {
						keypad[9] = KEY_ON;
					} break;

					case SDLK_z: {
						keypad[0xA] = KEY_ON;
					} break;

					case SDLK_c: {
						keypad[0xB] = KEY_ON;
					} break;

					case SDLK_4: {
						keypad[0xC] = KEY_ON;
					} break;

					case SDLK_r: {
						keypad[0xD] = KEY_ON;
					} break;

					case SDLK_f: {
						keypad[0xE] = KEY_ON;
					} break;

					case SDLK_v: {
					    keypad[0xF] = KEY_ON;
					} break;
				}
			} break;

			case SDL_KEYUP: {
				switch (event.key.keysym.sym) {
					case SDLK_x: {
					    keypad[0] = KEY_OFF;
					} break;

					case SDLK_1: {
						keypad[1] = KEY_OFF;
					} break;

					case SDLK_2: {
						keypad[2] = KEY_OFF;
					} break;

					case SDLK_3: {
						keypad[3] = KEY_OFF;
					} break;

					case SDLK_q: {
						keypad[4] = KEY_OFF;
					} break;

					case SDLK_w: {
						keypad[5] = KEY_OFF;
					} break;

					case SDLK_e: {
						keypad[6] = KEY_OFF;
					} break;

					case SDLK_a: {
						keypad[7] = KEY_OFF;
					} break;

					case SDLK_s: {
						keypad[8] = KEY_OFF;
					} break;

					case SDLK_d: {
						keypad[9] = KEY_OFF;
					} break;

					case SDLK_z: {
						keypad[0xA] = KEY_OFF;
					} break;

					case SDLK_c: {
						keypad[0xB] = KEY_OFF;
					} break;

					case SDLK_4: {
						keypad[0xC] = KEY_OFF;
					} break;

					case SDLK_r: {
						keypad[0xD] = KEY_OFF;
					} break;

					case SDLK_f: {
						keypad[0xE] = KEY_OFF;
					} break;

					case SDLK_v: {
					    keypad[0xF] = KEY_OFF;
					} break;
				}
			} break;
		}
	}
    return quit;
}