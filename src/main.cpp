#include "chip8.h"
#include "chip8video.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // print signs of life
    printf("||||||||||||||||\n");
    printf(" Chip8 Emulator\n");
    printf("||||||||||||||||\n\n");

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        return -1;
    }

    // cmd args
    int videoScale = std::stoi(argv[1]);
    int cycleDelay = std::stoi(argv[2]);
    char const* ROMfilename = argv[3];

    Chip8_Video chip8video(VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

    Chip8 chip8;
    chip8.LoadROM(ROMfilename);

    chip8.MemoryDump();

    //return 0;

    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    auto lastCycleTime = std::chrono::high_resolution_clock::now();

    bool quit = false;

    while (!quit) {
        quit = chip8video.HandleInput(chip8.keypad);

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycleDelay) {
            lastCycleTime = currentTime;
            chip8.Cycle();
            chip8video.Update(chip8.video, videoPitch);
        }
    }

    chip8.MemoryDump();

    printf("Quit\n");
    return 0;
}