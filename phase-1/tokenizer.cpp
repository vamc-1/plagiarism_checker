#include "tokenizer.hpp"
// You should NOT modify ANYTHING in this file.

tokenizer_t::tokenizer_t(std::string __file_name) {
    this->file_name = __file_name;
    this->index = clang_createIndex(0, 0);
    const char* args[] = {"-std=c++20"};
    this->unit = clang_parseTranslationUnit(
        index,
        this->file_name.c_str(), 
        args, 1,
        nullptr, 0,
        CXTranslationUnit_None);
    if (this->unit == nullptr) {
        std::string inv = "Unable to parse file: " + std::string(file_name);
        throw std::invalid_argument(inv.c_str());
    }
}

tokenizer_t::~tokenizer_t(void) {
    clang_disposeTranslationUnit(this->unit);
    clang_disposeIndex(this->index);
}

std::vector<int> tokenizer_t::get_tokens(void) {
    struct tokenizer_data_t data = {std::vector<int>(), this};
    clang_visitChildren(clang_getTranslationUnitCursor(this->unit), 
            [](CXCursor c, CXCursor parent, CXClientData client_data) {
                tokenizer_data_t* data = 
                    reinterpret_cast<tokenizer_data_t*>(client_data);
                if (data->tokenizer->is_from_main_file(c)) {
                    int token = static_cast<int>(clang_getCursorKind(c));
                    data->tokens.push_back(token);
                }
                return CXChildVisit_Recurse;
            }, reinterpret_cast<CXClientData>(&data));
    return data.tokens;
}

bool tokenizer_t::is_from_main_file(CXCursor __cursor) {
    CXFile cursor_file;
    unsigned line, column, offset;
    clang_getSpellingLocation(clang_getCursorLocation(__cursor),
            &cursor_file, &line, &column, &offset);
    CXFile main_file = clang_getFile(this->unit, this->file_name.c_str());
    return clang_File_isEqual(cursor_file, main_file);
}

std::string get_cursor_kind_spelling(int kind) {
    return clang_getCString(clang_getCursorKindSpelling(
        static_cast<CXCursorKind>(kind)));
} 

#ifdef BUILD_EXECUTABLE

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./tokenizer <source_file.cpp>\n";
        return 1;
    }

    try {
        std::string filename = argv[1];
        tokenizer_t tokenizer(filename);
        std::vector<int> tokens = tokenizer.get_tokens();

        for (int token : tokens) {
            std::string spelling = get_cursor_kind_spelling(token);
            std::cout << token << " - " << spelling << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}

#endif
