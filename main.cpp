#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

class BrainfuckInterpreter {
public:
    BrainfuckInterpreter(const string &code, size_t memory_size = 30000) 
        : code(code), code_ptr(0), mem_ptr(0) {
        memory.resize(memory_size, 0);
        build_jump_table();
    }

    string interpret() {
        ostringstream output;
        while (code_ptr < code.size()) {
            char command = code[code_ptr];
            size_t old_code_ptr = code_ptr;
            switch (command) {
                case '>': increment_mem_ptr(); break;
                case '<': decrement_mem_ptr(); break;
                case '+': increment_value(); break;
                case '-': decrement_value(); break;
                case '.': output << static_cast<char>(memory[mem_ptr]); break;
                case ',': read_input(); break;
                case '[': handle_loop_start(); break;
                case ']': handle_loop_end(); break;
            }
            if (code_ptr == old_code_ptr) {
                code_ptr++;
            }
        }
        return output.str();
    }

private:
    string code;
    size_t code_ptr;
    size_t mem_ptr;
    vector<unsigned char> memory;
    unordered_map<size_t, size_t> jump_table;

    void build_jump_table() {
        vector<size_t> stack;
        for (size_t i = 0; i < code.size(); i++) {
            if (code[i] == '[') {
                stack.push_back(i);
            } else if (code[i] == ']') {
                if (stack.empty()) {
                    throw runtime_error("Unmatched closing bracket at position " + to_string(i));
                }
                size_t open_pos = stack.back();
                stack.pop_back();
                jump_table[open_pos] = i;
                jump_table[i] = open_pos;
            }
        }
        if (!stack.empty()) {
            throw runtime_error("Unmatched opening bracket at position " + to_string(stack.back()));
        }
    }

    void increment_mem_ptr() {
        mem_ptr = (mem_ptr + 1) % memory.size();
    }

    void decrement_mem_ptr() {
        if (mem_ptr == 0) {
            mem_ptr = memory.size() - 1;
        } else {
            mem_ptr--;
        }
    }

    void increment_value() {
        memory[mem_ptr]++;
    }

    void decrement_value() {
        memory[mem_ptr]--;
    }

    void read_input() {
        int input = cin.get();
        if (input == EOF) {
            throw runtime_error("End of input reached");
        }
        memory[mem_ptr] = static_cast<unsigned char>(input);
    }

    void handle_loop_start() {
        if (memory[mem_ptr] == 0) {
            code_ptr = jump_table[code_ptr];
        }
    }

    void handle_loop_end() {
        if (memory[mem_ptr] != 0) {
            code_ptr = jump_table[code_ptr];
        }
    }
};

int main() {
    string code;
    cout << "Enter Brainfuck code: ";
    getline(cin, code);
    try {
        BrainfuckInterpreter interpreter(code);
        string result = interpreter.interpret();
        if (!result.empty()) {
            cout << "Output (int): " << static_cast<int>(static_cast<unsigned char>(result[0])) << endl;
            cout << "Output (char): '" << result[0] << "'" << endl;
        } else {
            cout << "Output is empty" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

//>++++[<+++++>-]<.
