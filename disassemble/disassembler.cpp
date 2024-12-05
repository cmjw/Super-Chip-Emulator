#include "disassembler.h"

/**
 * Disassemble the instructions contained in the buffer
 */
void Disassembler::disassemble(const std::vector<uint8_t>& buffer) {
    // check alignment
    if (buffer.size() % 2 != 0) {
        std::cerr << "Error: File size is not even (program must be aligned)" << std::endl;
        return;
    }

    // disassemble instructions in buffer
    for (size_t i = 0; i < buffer.size(); i += 2) {
        uint16_t opcode = (buffer[i] << 8) | buffer[i + 1]; // combine two bytes into 1, 16 bit instruction
        std::string instr = decodeOpcode(opcode);

        // change to variable output stream later
        std::cout << std::hex << std::setw(4) << std::setfill('0') << i // address
                << ": " << std::hex << std::setw(4) << std::setfill('0') << opcode 
                << " | " << instr << std::endl;
    }
}

/**
 * Disassemble a single instruction given an opcode
 */
std::string Disassembler::decodeOpcode(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    uint8_t kk = (opcode & 0x00FF);
    uint8_t n = opcode & 0x000F;
    uint16_t nnn = opcode & 0x0FFF;

    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) return "CLS";
            if (opcode == 0x00EE) return "RET";
            return "SYS " + std::to_string(nnn); // THESE ARE PRINTING IN DEC
        case 0x1000: return "JP 0x" + std::to_string(nnn);
        case 0x2000: return "CALL 0x" + std::to_string(nnn);
        case 0x3000: return "SE V" + std::to_string(x) + ", 0x" + std::to_string(kk);
        case 0x4000: return "SNE V" + std::to_string(x) + ", 0x" + std::to_string(kk);
        case 0x5000: return "SE V" + std::to_string(x) + ", V" + std::to_string(y);
        case 0x6000: return "LD V" + std::to_string(x) + ", 0x" + std::to_string(kk);
        case 0x7000: return "ADD V" + std::to_string(x) + ", 0x" + std::to_string(kk);
        case 0x8000:
            switch (n) {
                case 0x0: return "LD V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0x1: return "OR V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0x2: return "AND V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0x3: return "XOR V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0x4: return "ADD V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0x5: return "SUB V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0x6: return "SHR V" + std::to_string(x);
                case 0x7: return "SUBN V" + std::to_string(x) + ", V" + std::to_string(y);
                case 0xE: return "SHL V" + std::to_string(x);
                default: return "UNKNOWN";
            }
        case 0x9000: return "SNE V" + std::to_string(x) + ", V" + std::to_string(y);
        case 0xA000: return "LD I, 0x" + std::to_string(nnn);
        case 0xB000: return "JP V0, 0x" + std::to_string(nnn);
        case 0xC000: return "RND V" + std::to_string(x) + ", 0x" + std::to_string(kk);
        case 0xD000: return "DRW V" + std::to_string(x) + ", V" + std::to_string(y) + ", 0x" + std::to_string(n);
        case 0xE000:
            if (kk == 0x9E) return "SKP V" + std::to_string(x);
            if (kk == 0xA1) return "SKNP V" + std::to_string(x);
            break;
        case 0xF000:
            switch (kk) {
                case 0x07: return "LD V" + std::to_string(x) + ", DT";
                case 0x0A: return "LD V" + std::to_string(x) + ", K";
                case 0x15: return "LD DT, V" + std::to_string(x);
                case 0x18: return "LD ST, V" + std::to_string(x);
                case 0x1E: return "ADD I, V" + std::to_string(x);
                case 0x29: return "LD F, V" + std::to_string(x);
                case 0x33: return "LD B, V" + std::to_string(x);
                case 0x55: return "LD [I], V" + std::to_string(x);
                case 0x65: return "LD V, [I]";
                default: return "UNKNOWN";
            }
    }

    return "UNKNOWN";
}