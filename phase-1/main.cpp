#include "tokenizer.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: tokenizer <source-file>" << std::endl;
        return 1;
    }

    try {
        tokenizer_t tokenizer(argv[1]);
        auto tokens = tokenizer.get_tokens();
        for (auto t : tokens) {
            std::cout << t << " : " << get_cursor_kind_spelling(t) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
