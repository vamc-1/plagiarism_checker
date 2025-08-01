
// tokenizer.hpp
#pragma once
#include <clang-c/Index.h>
#include <string>
#include <vector>

class tokenizer_t {
public:
    tokenizer_t(std::string __file_name);
    ~tokenizer_t();

    std::vector<int> get_tokens();      // Just the kinds
    void print_tokens();                // Prints <kind> - <spelling> <line> <column>

private:
    std::string file_name;
    CXTranslationUnit unit;
    CXIndex index;

    bool is_from_main_file(CXCursor __cursor);

    struct tokenizer_data_t {
        std::vector<int> tokens;
        tokenizer_t* tokenizer;
    };
};

std::string get_cursor_kind_spelling(int kind);
