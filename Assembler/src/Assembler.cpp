#include "Assembler.hpp"

#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

namespace nb_factorio {

Assembler::Assembler() {}

void Assembler::assemble(string& file_name)
{
    ifstream file;
    file.open(file_name);
    if (!file.is_open()) {
        cerr << "Failed to open: " << file_name << '\n';
    } else {
        parse_variables(file);
        parse_labels(file);
        parse_instructions(file);
        file.close();
        convert_instructions();
    }
}

bytecode Assembler::get_result() {   return result; }

void Assembler::reset_position(ifstream& file)
{
    file.seekg(0, ios_base::beg);
    string token;
    file >> token;
    while (token != "entry") {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        file >> token;
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Assembler::parse_variables(ifstream& file)
{
    string token;
    file >> token;
    while (token != "entry") {
        if (!keywords.contains(token)) {
            uint32_t address;
            file >> address;
            labels.insert(make_pair(token, address));
        }
        file >> token;
    }
}

void Assembler::parse_labels(ifstream& file)
{
    reset_position(file);
    string token;
    labels.clear();
    for (uint32_t line_number = 1; !file.eof(); line_number++) {    
        file >> token;
        if (!keywords.contains(token)) {
            labels.insert(make_pair(token, line_number--));
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Assembler::parse_instructions(ifstream& file)
{
    reset_position(file);
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
        file.ignore(numeric_limits<streamsize>::max(), '\n');
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