// t_main.cpp
#include "tokenizer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./tokenizer <source_file.cpp>\n";
        return 1;
    }

    try {
        tokenizer_t tokenizer(argv[1]);
        tokenizer.print_tokens(); // <- prints kind, spelling, line, column
    } catch (const std::exception& e) {
        std::cerr << "Tokenizer error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
