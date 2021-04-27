#include "Assembler.hpp"

#include <iostream>
#include <string>

using namespace nb_factorio;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: factorio_asm input_file output_file\n";
        exit(EXIT_FAILURE);
    }
    std::string in_file = argv[1];
    std::string out_file = argv[2];
    Assembler a;
    a.assemble(in_file);
    a.make_bp_json(out_file);
}
