#ifndef NB_ASSEMBLER_HPP
#define NB_ASSEMBLER_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace nb_factorio {

typedef std::vector<int32_t> bytecode;

class Assembler
{
public:
    Assembler();
    void assemble(std::string& file_name);
    bytecode get_result();
    void make_bp_json(std::string& file_name);
private:
    struct instruction {
        uint32_t immediate;
        uint32_t address;
        uint8_t  opcode;
    };
    typedef std::vector<instruction> instruction_list;

private:
    void parse_directives(std::ifstream& file);
    void parse_labels(std::ifstream& file);
    void parse_instructions(std::ifstream& file);
    void convert_instructions();

private:
    uint32_t ram_start{513}, variable_address{ram_start};

private:
    enum class directive {entry, var, ram_start, peripheral};
    std::unordered_map<std::string, directive> directives = {
        {"entry", directive::entry},
        {"var", directive::var},
        {"ram_start", directive::ram_start},
        {"peripheral", directive::peripheral},
    };
    std::unordered_map<std::string, uint8_t> keywords = {
        {"NOP", 0},  {"ADD", 1},  {"SUB", 2},  {"MUL", 3},  {"DIV", 4},  {"MOD", 5},
        {"EXP", 6},  {"SHL", 7},  {"SHR", 8},  {"AND", 9},  {"OR",  10}, {"XOR", 11},
        {"GT",  12}, {"LT",  13}, {"GE",  14}, {"LE",  15}, {"EQ",  16}, {"NE",  17},
        {"LD",  18}, {"ST",  19}, {"STA", 20}, {"JMP", 21}, {"JZ",  22}, {"PSH", 23},
        {"POP", 24}, {"HLT", 63}
    };
    std::unordered_map<std::string, uint32_t> labels;
    instruction_list instructions;
    bytecode result;
    std::string program_name;

private:
    void emit_head(std::ofstream& file);
    void emit_middle(std::ofstream& file);
    void emit_end(std::ofstream& file);
};

} // namespace nb_factorio
#endif