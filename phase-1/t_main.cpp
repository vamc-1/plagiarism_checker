// t_main.cpp  – stand‑alone tokenizer executable
// ------------------------------------------------
//  * Requires LLVM / libclang.
//  * Does NOT touch tokenizer.hpp / tokenizer.cpp.
//  * Compile with:
//      g++ -std=c++20 -fexceptions t_main.cpp \
//          $(llvm-config --cxxflags --ldflags --system-libs) \
//          -lclang -o tokenizer
//
//  Usage:
//      ./tokenizer source.cpp
//
//  Output line format:
//      <kind> - <spelling> <line> <column>
// ------------------------------------------------
#include <clang-c/Index.h>
#include <iostream>
#include <string>

static CXChildVisitResult printer(CXCursor cur, CXCursor /*parent*/, CXClientData)
{
    // we only care about tokens from the *main* file (ignore <built‑in> etc.)
    if (!clang_Location_isFromMainFile(clang_getCursorLocation(cur)))
        return CXChildVisit_Recurse;

    // numeric cursor kind
    int kind = static_cast<int>(clang_getCursorKind(cur));

    // spelling (human readable name)
    const char* spelling_cstr =
        clang_getCString(clang_getCursorKindSpelling(
            static_cast<CXCursorKind>(kind)));
    std::string spelling = spelling_cstr ? spelling_cstr : "";

    // source location (line / column)
    CXFile file; unsigned line = 0, column = 0, offset = 0;
    clang_getSpellingLocation(clang_getCursorLocation(cur),
                              &file, &line, &column, &offset);

    std::cout << kind << " - " << spelling << ' '
              << line << ' ' << column << '\n';

    return CXChildVisit_Recurse;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./tokenizer <source_file.cpp>\n";
        return 1;
    }
    const char* src = argv[1];

    // 1. create index
    CXIndex idx = clang_createIndex(/*excludeDeclsFromPCH=*/0,
                                    /*displayDiagnostics=*/0);

    // 2. parse translation unit
    const char* args[] = { "-std=c++20" };
    CXTranslationUnit tu =
        clang_parseTranslationUnit(idx, src,
                                   args, 1,
                                   nullptr, 0,
                                   CXTranslationUnit_None);

    if (!tu) {
        std::cerr << "Error: Could not parse " << src << '\n';
        clang_disposeIndex(idx);
        return 1;
    }

    // 3. traverse and print every cursor
    CXCursor root = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(root, printer, nullptr);

    // 4. clean up
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(idx);
    return 0;
}
