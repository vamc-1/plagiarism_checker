// tokenizer.cpp
#include "tokenizer.hpp"
#include <iostream>
#include <stdexcept>

tokenizer_t::tokenizer_t(std::string __file_name)
    : file_name(std::move(__file_name)) {
    index = clang_createIndex(0, 0);
    const char* args[] = {"-std=c++20"};
    unit = clang_parseTranslationUnit(index, file_name.c_str(), args, 1, nullptr, 0, CXTranslationUnit_None);

    if (!unit) {
        throw std::invalid_argument("Unable to parse file: " + file_name);
    }
}

tokenizer_t::~tokenizer_t() {
    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}

bool tokenizer_t::is_from_main_file(CXCursor cursor) {
    CXFile cursor_file;
    unsigned line, column, offset;
    clang_getSpellingLocation(clang_getCursorLocation(cursor), &cursor_file, &line, &column, &offset);
    CXFile main_file = clang_getFile(unit, file_name.c_str());
    return clang_File_isEqual(cursor_file, main_file);
}

std::vector<int> tokenizer_t::get_tokens() {
    tokenizer_data_t data = {std::vector<int>(), this};
    clang_visitChildren(clang_getTranslationUnitCursor(unit),
        [](CXCursor cursor, CXCursor /*parent*/, CXClientData client_data) {
            auto* data = static_cast<tokenizer_data_t*>(client_data);
            if (data->tokenizer->is_from_main_file(cursor)) {
                int token_kind = static_cast<int>(clang_getCursorKind(cursor));
                data->tokens.push_back(token_kind);
            }
            return CXChildVisit_Recurse;
        }, &data);
    return data.tokens;
}

void tokenizer_t::print_tokens() {
    clang_visitChildren(clang_getTranslationUnitCursor(unit),
        [](CXCursor cursor, CXCursor, CXClientData client_data) {
            tokenizer_t* self = static_cast<tokenizer_t*>(client_data);
            if (!self->is_from_main_file(cursor))
                return CXChildVisit_Recurse;

            int kind = static_cast<int>(clang_getCursorKind(cursor));
            const char* spelling_cstr = clang_getCString(clang_getCursorKindSpelling((CXCursorKind)kind));
            std::string spelling = spelling_cstr ? spelling_cstr : "";

            CXFile file;
            unsigned line = 0, column = 0, offset = 0;
            clang_getSpellingLocation(clang_getCursorLocation(cursor), &file, &line, &column, &offset);

            std::cout << kind << " - " << spelling << " " << line << " " << column << "\n";
            return CXChildVisit_Recurse;
        }, this);
}

std::string get_cursor_kind_spelling(int kind) {
    return clang_getCString(clang_getCursorKindSpelling(static_cast<CXCursorKind>(kind)));
}
