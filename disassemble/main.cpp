#include "disassembler.h"
#include <stdio.h>
#include <fstream>

int main (int argc, char* argv[]) {
    // check args
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    // check input file
    std::ifstream inputFile(argv[1], std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    // read input file into buffer
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    
    // close input file
    inputFile.close();

    // disassemble
    Disassembler disassembler;
    disassembler.disassemble(buffer);

    return 0;
}