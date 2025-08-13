#include "plagiarism_checker.hpp"
#include <fstream>
#include <iostream>

int main()
{
    std::string fileA, fileB;

    std::cout << "Enter first fileA : ";
    std::cin  >> fileA;
    if (!std::ifstream(fileA).good()) {
        std::cerr << "Error: File \"" << fileA
                  << "\" not found in current directory.\n";
        return 1;
    }

    std::cout << "Enter second fileB : ";
    std::cin  >> fileB;
    if (!std::ifstream(fileB).good()) {
        std::cerr << "Error: File \"" << fileB
                  << "\" not found in current directory.\n";
        return 1;
    }

    // Tokenize & print (just like the original code)
    Tokens sub1 = tokenizeFile(fileA);
    Tokens sub2 = tokenizeFile(fileB);
    // printTokens("tokenized_vector_of_fileA (submission1)", sub1);
    // printTokens("tokenized_vector_of_fileB (submission2)", sub2);

    // Run the full plagiarism analysis (prints 5 metrics)
    analyzePair(fileA, fileB);
    return 0;
}
