#include "chip8.h"

uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
/* Chip8 supports a keypad with characters 0 to F.
    Each is displayed in a 8x5 pixel image, which can 
    be represented by 5 bytes.*/


Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
    // initialize pc
    pc = START_ADDRESS;

    // init sp
    sp = 0;

    // zero out memory
    memset(memory, 0, sizeof(uint8_t) * MEMORY_SIZE);

    // load fonts into memory
    for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    LoadOpcodeTables();

    // initialize random number generator
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

Chip8::~Chip8() {

}

/** 
 * Simulate one cycle
 */
void Chip8::Cycle() {
    printf("PC: %03x\n", pc);

    // fetch instruction
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc += 2;

    printf("Opcode: 0x%04x\n", opcode);

    // decode and execute
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    // decrement delay timer
    if (delayTimer > 0) {
        delayTimer--;
    }

    // decrement sound timer
    if (soundTimer > 0) {
        soundTimer--;
    }
}

/**
 * Load a ROM into memory.
 * The contents are loaded starting at 0x200 in memory.
 */
void Chip8::LoadROM(const char* filename) {
    // open file stream
    // ios::ate places cursor at endfile after opening
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        // get size and allocate buffer
        std::streampos size = file.tellg(); 
        char* buffer = new char[size];

        // move cursor to beginning of file
        file.seekg(0, std::ios::beg);
        
        // fill buffer
        file.read(buffer, size);
        file.close();

        // load ROM contents into memory, starting at 0x200
        for (long i = 0; i < size; i++) {
            memory[START_ADDRESS + i] = buffer[i];
        }

        // free the buffer
        delete[] buffer;
    } else {
        std::cerr << "ERROR: Invalid ROM file. Aborting" << std::endl;
        //this->MemoryDump();
        exit(-1);
    } 
}

/**
 * Load main opcode table data
 */
void Chip8::LoadOpcodeTables() {
    // Set up function pointer table
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	for (size_t i = 0; i <= 0xE; i++) {
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	for (size_t i = 0; i <= 0x65; i++) {
		tableF[i] = &Chip8::OP_NULL;
	}

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;
}

/**
 * Load opcode table 0 data
 */
void Chip8::Table0() {
    ((*this).*(table0[opcode & 0x000Fu]))();
}

/**
 * Load opcode table 8 data
 */
void Chip8::Table8() {
    ((*this).*(table8[opcode & 0x000Fu]))();
}

/**
 * Load opcode table E data
 */
void Chip8::TableE() {
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

/**
 * Load opcode table F data
 */
void Chip8::TableF() {
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

/**
 * Print memory contents.
 */
void Chip8::MemoryDump() {
    unsigned int i = 0;
    int j = 0;

    printf("Memory Dump:\n");
    printf("| ---------------------------------- |\n");
    printf("| addr %03x | ", i);

    // print reserved data
    for (i = 0; i < RESERVED_MEMORY_SIZE; i++) {
        printf("%02x ", memory[i]);
        j++;
        if (j % 8 == 0) {
            printf("|\n| ");
            if (i + 1 < RESERVED_MEMORY_SIZE) {
                printf("addr %03x | ", i);
            }
        }
    }

    printf("---------------------------------- |\n| ");
    printf("addr %03x | ", i);

    // program data
    for (; i < MEMORY_SIZE; i++) {
        printf("%02x ", memory[i]);
        j++;
        if (j % 8 == 0 && i < MEMORY_SIZE - 1) {
            printf("|\n| ");
            if (i + 1 < MEMORY_SIZE) {
                printf("addr %03x | ", i);
            }
        } else if (i >= MEMORY_SIZE - 1) {
            printf("|\n");
        }
    }
    printf("| ---------------------------------- |\n");
}