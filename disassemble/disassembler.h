#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <string>
#include <vector>

#include <iostream> 
#include <iomanip>

class Disassembler {
public:
    void disassemble(const std::vector<uint8_t>& buffer);

private:
    std::string decodeOpcode(uint16_t opcode);
};

#endif