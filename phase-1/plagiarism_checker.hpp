#pragma once
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>

//──────────────────────────────────────────────────────────────
//  Token structure  (kind + source location)
//──────────────────────────────────────────────────────────────
struct TokenInfo {
    int      kind;          // numeric cursor kind
    unsigned line;          // 1‑based line number
    unsigned column;        // 1‑based column number
};

// Aliases
using Tokens    = std::vector<TokenInfo>;
using Positions = std::vector<std::pair<int,int>>;   // (indexA,indexB) pairs

//──────────────────────────────────────────────────────────────
//  Core helpers
//──────────────────────────────────────────────────────────────
Tokens tokenizeFile(const std::string& cppFile);

std::pair<int,Positions>
lcsWithPositions(const Tokens& A, const Tokens& B);

int longestExactRunFrom(std::map<int,std::set<int>>& idx2,
                        const Tokens& A,
                        int posA, int sizeB);

int longestFuzzyBlock(const Positions& pos,
                      int& startA, int& startB);   // +23 rule inside

double plagiarismScore(const Tokens& A,
                       const Tokens& B,
                       int totalRunLen);

// Utility printers
void printTokens(const std::string& label, const Tokens& t);

// High‑level driver
void analyzePair(const std::string& fileA,
                 const std::string& fileB);
