#include "schip.h"

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
/* SChip supports a keypad with characters 0 to F.
    Each is displayed in a 8x5 pixel image, which can 
    be represented by 5 bytes.*/


SChip::SChip() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
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

SChip::~SChip() {

}

/** 
 * Simulate one cycle
 */
void SChip::Cycle() {
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
void SChip::LoadROM(const char* filename) {
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
void SChip::LoadOpcodeTables() {
    // Set up function pointer table
	table[0x0] = &SChip::Table0;
	table[0x1] = &SChip::OP_1nnn;
	table[0x2] = &SChip::OP_2nnn;
	table[0x3] = &SChip::OP_3xkk;
	table[0x4] = &SChip::OP_4xkk;
	table[0x5] = &SChip::OP_5xy0;
	table[0x6] = &SChip::OP_6xkk;
	table[0x7] = &SChip::OP_7xkk;
	table[0x8] = &SChip::Table8;
	table[0x9] = &SChip::OP_9xy0;
	table[0xA] = &SChip::OP_Annn;
	table[0xB] = &SChip::OP_Bnnn;
	table[0xC] = &SChip::OP_Cxkk;
	table[0xD] = &SChip::OP_Dxyn;
	table[0xE] = &SChip::TableE;
	table[0xF] = &SChip::TableF;

	for (size_t i = 0; i <= 0xE; i++) {
		table0[i] = &SChip::OP_NULL;
		table8[i] = &SChip::OP_NULL;
		tableE[i] = &SChip::OP_NULL;
	}

	table0[0x0] = &SChip::OP_00E0;
	table0[0xE] = &SChip::OP_00EE;

	table8[0x0] = &SChip::OP_8xy0;
	table8[0x1] = &SChip::OP_8xy1;
	table8[0x2] = &SChip::OP_8xy2;
	table8[0x3] = &SChip::OP_8xy3;
	table8[0x4] = &SChip::OP_8xy4;
	table8[0x5] = &SChip::OP_8xy5;
	table8[0x6] = &SChip::OP_8xy6;
	table8[0x7] = &SChip::OP_8xy7;
	table8[0xE] = &SChip::OP_8xyE;

	tableE[0x1] = &SChip::OP_ExA1;
	tableE[0xE] = &SChip::OP_Ex9E;

	for (size_t i = 0; i <= 0x65; i++) {
		tableF[i] = &SChip::OP_NULL;
	}

	tableF[0x07] = &SChip::OP_Fx07;
	tableF[0x0A] = &SChip::OP_Fx0A;
	tableF[0x15] = &SChip::OP_Fx15;
	tableF[0x18] = &SChip::OP_Fx18;
	tableF[0x1E] = &SChip::OP_Fx1E;
	tableF[0x29] = &SChip::OP_Fx29;
	tableF[0x33] = &SChip::OP_Fx33;
	tableF[0x55] = &SChip::OP_Fx55;
	tableF[0x65] = &SChip::OP_Fx65;
}

/**
 * Load opcode table 0 data
 */
void SChip::Table0() {
    ((*this).*(table0[opcode & 0x000Fu]))();
}

/**
 * Load opcode table 8 data
 */
void SChip::Table8() {
    ((*this).*(table8[opcode & 0x000Fu]))();
}

/**
 * Load opcode table E data
 */
void SChip::TableE() {
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

/**
 * Load opcode table F data
 */
void SChip::TableF() {
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

/**
 * Print memory contents.
 */
void SChip::MemoryDump() {
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