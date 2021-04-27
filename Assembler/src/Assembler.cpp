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
        parse_directives(file);
        parse_labels(file);
        parse_instructions(file);
        file.close();
        convert_instructions();
        program_name = file_name;
    }
}

bytecode Assembler::get_result() {   return result; }

void Assembler::make_bp_json(string& file_name)
{
    ofstream file;
    file.open(file_name);
    if (!file.is_open()) {
        cerr << "Failed to open: " << file_name << '\n';
    } else {
        emit_head(file);
        emit_middle(file);
        emit_end(file);
        file.close();
    }
}

void Assembler::parse_directives(ifstream& file)
{
    labels.clear();
    string token;
    uint32_t line_number{1};
    do {
        line_number++;
        file >> token;
        if (directives.contains(token)) {
            switch (directives.at(token)) {
            case directive::entry: {
                break;
            }
            case directive::var: {
                file >> token;
                labels.insert(make_pair(token, variable_address++));
                break;
            }
            case directive::ram_start: {
                file >> ram_start;
                variable_address = ram_start;
                break;
            }
            case directive::peripheral: {
                uint32_t peripheral_address;
                file >> token;
                file >> peripheral_address;
                labels.insert(make_pair(token, peripheral_address));
                break;
            }
            } // switch(directives.at(token))
        } else {
            file.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (token != "entry");
}

void Assembler::parse_labels(ifstream& file)
{
    file.seekg(0, ios_base::beg);
    string token;
    for (uint32_t line_number = 1; !file.eof(); line_number++) {    
        file >> token;
        if (!keywords.contains(token)) {
            labels.insert(make_pair(token, line_number--));
        } else if (directives.contains(token)) {
            line_number--;
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Assembler::parse_instructions(ifstream& file)
{
    file.seekg(0, ios_base::beg);
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

void Assembler::emit_head(ofstream& file)
{
    file << "{\"blueprint\":{\"icons\":[{\"signal\":{\"type\":\"item\",\"name\":\"constant-combinator\"},\"index\":1}],\"entities\":[";
}

// This is messy
// I should use a library for making json stuff
void Assembler::emit_middle(ofstream& file)
{
    int entity_number{0}, x{-128}, y{-102};
    float half{-0.5};
    bool big_gap{false}, first{true};
    
    if (result.size() > 512) {
        cerr << "Program larger than 512 words.\n";
    } else {
        for (int32_t i : result) {
            entity_number++;
            if (entity_number % 64 == 1) {
                x += (big_gap) ? 7 : 5;
                big_gap = !big_gap;
                y = -102;
            }
            if (first) {
                first = false;
            } else {
                file << ",";
            }
            file << "{\"entity_number\":" << entity_number <<
                    ",\"name\":\"constant-combinator\",\"position\":{\"x\":" << static_cast<float>(x) + half <<
                    ",\"y\":" << static_cast<float>(y) + half <<
                    "},\"direction\":2,\"control_behavior\":{\"filters\":[{\"signal\":{\"type\":\"virtual\"," <<
                    "\"name\":\"signal-D\"},\"count\":" << i <<
                    ",\"index\":1}]}}";
            y++;
        }
    }
}

void Assembler::emit_end(ofstream& file)
{
    file << "],\"item\":\"blueprint\",\"label\":\"";
    file << program_name;
    file << "\",\"version\":281479273775104}}";
}

} // namespace nb_factorio