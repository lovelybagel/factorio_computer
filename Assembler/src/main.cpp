#include "Assembler.hpp"

#include <iostream>
#include <string>

using namespace nb_factorio;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: factorio_asm filename\n";
        exit(EXIT_FAILURE);
    }

    std::string filename = argv[1];
    Assembler a;
    a.assemble(filename);
    for (auto instruction : a.get_result()) {
        std::cout << instruction << '\n';
    }
} 