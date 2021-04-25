#include "Assembler.hpp"

#include <iostream>

using namespace std;

namespace nb_factorio {

Assembler::Assembler() {};

void Assembler::assemble(string& file_name)
{
    ifstream file;
    file.open(file_name);
    if (!file.is_open()) {
        cerr << Failed to open: << file_name << '\n';
    } else {
        parse_labels(file);
        parse_instructions(file);
        file.close();
        convert_instructions();
    }
}

bytecode Assembler::get_result() {   return result; }

void Assembler::parse_labels(ifstream& file)
{
    string token;
    labels.clear();
    for (int line_number = 1; !file.eof(); line_number++) {    
        file >> token;
        if (!keywords.contains(token)) {
            labels.insert(token, line_number--);
        }
        file.getline();
    }
    file.seekg(0, ios_base::beg);
}

void Assembler::parse_instructions(ifstream& file)
{
    instructions.clear();
    string token;
    while (!file.eof()) {
        file >> token;
        if (keywords.contains(token)) {
            instruction i;
            i.opcode = keywords.at(token);
            auto pos = file.tellg();
            file >> token;
            if (labels.contains(token)) {
                i.immediate = labels.at(token);
            } else {
                file.seekg(pos, ios_base::beg);
                file >> i.immediate;
            }
            pos = file.tellg();
            file >> token;
            if (labels.contains(token)) {
                i.address = labels.at(token);
            } else {
                file.seekg(pos, ios_base::beg);
                file >> i.address;
            }
            instructions.push_back(i);
        }
        file.getline();
    }
}

void Assembler::convert_instructions()
{
    result.clear();
    for (instruction i : instructions) {
        result.push_back(static_cast<int32_t>((i.immediate << 19) | (i.address << 6) | i.opcode));
    }
}

} // namespace nb_factorio