#ifndef SCHIP_H
#define SCHIP_H

#include <fstream>
#include <cstdint>
#include <random>
#include <chrono>
#include <cstring>
#include <iostream>

const unsigned int MEMORY_SIZE = 4096;
const unsigned int RESERVED_MEMORY_SIZE = 512;

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

const unsigned int INSTRUCTION_WIDTH = 2;
const unsigned int FONT_SIZE = 5; // fonts are 5 bytes

const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

const unsigned int FONTSET_SIZE = 80;

class SChip {
public:
    SChip();
    ~SChip();

    void Cycle();
    void LoadROM(const char* filename);

    void MemoryDump();

    uint8_t keypad[16]; // 16 character keypad

    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]; // 64x32 video output
    /* monochrome video. each 32-bit pixel is on or off.*/

    /* Original SChip Instructions */
    void OP_NULL(); // NULL OP
    void OP_00E0(); // CLS
    void OP_00EE(); // RET
    void OP_1nnn(); // JP addr
    void OP_2nnn(); // CALL addr
    void OP_3xkk(); // SE Vx, byte
    void OP_4xkk(); // SNE Vx, byte
    void OP_5xy0(); // SE Vx, Vy
    void OP_6xkk(); // LD Vx, byte
    void OP_7xkk(); // ADD Vx, byte
    void OP_8xy0(); // LD Vx, Vy
    void OP_8xy1(); // OR Vx, Vy
    void OP_8xy2(); // AND Vx, Vy
    void OP_8xy3(); // XOR Vx, Vy
    void OP_8xy4(); // ADD Vx, Vy
    void OP_8xy5(); // SUB Vx, Vy
    void OP_8xy6(); // SHR Vx
    void OP_8xy7(); // SUBN Vx, Vy
    void OP_8xyE(); // SHL Vx {, Vy}
    void OP_9xy0(); // SNE Vx, Vy
    void OP_Annn(); // LD I, addr
    void OP_Bnnn(); // JP V0, addr
    void OP_Cxkk(); // RND Vx, byte
    void OP_Dxyn(); // DRW Vx, Vy, nibble
    void OP_Ex9E(); // SKP Vx
    void OP_ExA1(); // SKNP Vx
    void OP_Fx07(); // LD Vx, DT
    void OP_Fx0A(); // LD Vx, K
    void OP_Fx15(); // LD DT, Vx
    void OP_Fx18(); // LD ST, Vx
    void OP_Fx1E(); // ADD I, Vx
    void OP_Fx29(); // LD F, Vx
    void OP_Fx33(); // LD B, Vx
    void OP_Fx55(); // LD [I], Vx
    void OP_Fx65(); // LD Vx, [I]

    /* Super-Chip Instructions */
    void OP_00CN(); // Scroll down N lines
    void OP_00FB(); // Scroll right 4 pixels
    void OP_00FC(); // Scroll left 4 pixels 
    void OP_00FD(); // Exit interpreter
    void OP_00FE(); // Disable extended video mode
    void OP_00FF(); // Enable extended video mode
    void OP_Dxyn2(); // Modified DRW for SuperChip
    void OP_Fx30(); // 
    void OP_Fx75(); //
    void OP_Fx85(); //

private:
    uint8_t memory[MEMORY_SIZE]; // 4096 bytes of memory
    /* - 0x200 to 0xFFF is program space
       - 0x000 to 0x1FF is reserved
           - 0x050 to 0x0A0 stores 16 built-in chars */

    uint8_t V[16]; // 16 8-bit registers
    /* V0 to VF, 0x00 to 0xFF
        VF is used as a flag */

    uint16_t I; // special 16-bit index register
    /* stores an address. needed because 8-bit registers can't store
        largest location. 
        usually only uses lower 12 bits. (2^12 = 4096)*/
    
    uint16_t pc; // 16-bit program counter

    uint8_t sp; // 8-bit stack pointer

    uint16_t stack[16]; // 16 level stack (16 bit)

    uint16_t opcode; // current opcode

    uint8_t delayTimer;
    uint8_t soundTimer;

    std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

    // opcode tables
    typedef void (SChip::*SChipFunc)();
    SChipFunc  table[0xF  + 1];
    SChipFunc table0[0xE  + 1];
    SChipFunc table8[0xE  + 1];
    SChipFunc tableE[0xE  + 1];
    SChipFunc tableF[0x65 + 1];

    void LoadOpcodeTables();
    void Table0();
    void Table8();
    void TableE();
    void TableF();
};

#endif