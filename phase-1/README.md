# C++ Plagiarism Checker (Phase 1)

## Overview

Phase 1 is a **single-pair plagiarism detector** for C++ source files. Given two `.cpp` submissions, it tokenizes both files into AST cursor-kind sequences using **libclang**, then applies two independent similarity analyses:

1. **LCS + Fuzzy-block rule** – finds the longest contiguous region of the two files that is highly similar.
2. **Exact-run pattern matching** – sums every verbatim run of >= 10 matching tokens, weighted by file sizes to yield a plagiarism score.

Both signals are combined to produce a binary `plag` flag (0 = clean, 1 = plagiarised).

---

## Algorithm Components

### 1. Tokenizer (`tokenizer.cpp / tokenizer.hpp`)

- Uses **libclang** (`CXCursor` API) to parse each `.cpp` file with C++20 semantics.
- Visits every AST node that belongs to the *main file* (ignoring included headers).
- For each cursor it records:
  - `kind` – the integer `CXCursorKind` (e.g. `FunctionDecl = 8`, `BinaryOperator = 114`).
  - `line` / `column` – source location for block-range reporting.
- Output format per token: `<kind> - <spelling> <line> <column>`

Using AST cursor kinds instead of raw text means that **variable renaming**, **whitespace changes**, and **comment differences are ignored** – only structural patterns are compared.

### 2. LCS with Positions (`lcsWithPositions`)

- Computes the classic **O(n x m) DP Longest Common Subsequence** of the two kind-sequences.
- Returns both the LCS *length* and the list of matched index pairs `(i, j)`.
- The matched pairs are the input to the fuzzy-block rule below.

### 3. Fuzzy-Block Rule (`longestFuzzyBlock`)

Iterates over all pairs `(i, j)` with `j >= i + 23` in the LCS position list:

```
if  (j - i + 1) > 0.8 x (posA[j] - posA[i] + 1)   AND
    (j - i + 1) > 0.8 x (posB[j] - posB[i] + 1)
```

This means: at least **80% of the token range** in *both* files must participate in the LCS match, and the window must contain **at least 24 matched tokens** (the `+23` rule). Any window satisfying both conditions is a *fuzzy block*.

The largest such window (by `max(span_A, span_B)`) is recorded with its start positions (`position_1`, `position_2`) and converted to source line numbers for the final report.

### 4. Exact-Run Pattern Matching (`longestExactRunFrom`)

- Builds an inverted index: `token_kind -> set<positions_in_B>`.
- For every position `i` in file A, finds the **longest verbatim (exact, contiguous) run** starting at `A[i]` that also appears starting at some `j` in B.
- Runs of **< 10 tokens** are ignored (noise threshold).
- Matched positions are removed from the index so each token in B is counted at most once.
- All run lengths >= 10 are summed into `sum_of_all_matched_patterns`.

### 5. Plagiarism Score (`plagiarismScore`)

```
score = (n1 / (n1 + n2)) x (sum / n2)
      + (n2 / (n1 + n2)) x (sum / n1)
```

where `n1`, `n2` are the token counts of files A and B respectively and `sum` is the total matched pattern length. This is a **size-weighted fraction** of matching tokens: weighting by the *other* file's size penalises large files that share only a small absolute overlap. If `score > 0.25` the pair is flagged as plagiarised.

---

## File Structure

| File | Role |
|---|---|
| `tokenizer.hpp / .cpp` | libclang-based AST tokenizer |
| `plagiarism_checker.hpp` | Public interface declarations |
| `plagiarism_checker.cpp` | Core algorithm implementations |
| `t_main.cpp` | Standalone tokenizer binary entry point |
| `p_main.cpp` | Plagiarism checker binary entry point |
| `test1.cpp` / `test2.cpp` | Example submissions for testing |

---

## Requirements

```bash
sudo apt update
sudo apt install build-essential llvm-dev libclang-dev
```

---

## Build & Run

### Step 1 - Build the tokenizer

```bash
g++ -std=c++20 t_main.cpp tokenizer.cpp \
    $(llvm-config --cxxflags --ldflags --system-libs) \
    -fexceptions -lclang -o tokenizer
```

Verify (prints the first 10 AST tokens of a file):

```bash
./tokenizer sample.cpp | head
```

### Step 2 - Build the plagiarism engine

```bash
g++ -std=c++20 \
    -I/usr/lib/llvm-16/include \
    -L/usr/lib/llvm-16/lib \
    tokenizer.cpp plagiarism_checker.cpp p_main.cpp \
    -o plagiarism_checker -lclang -pthread \
    -Wl,-rpath,/usr/lib/llvm-16/lib
```

### Step 3 - Run a single comparison

```bash
./plagiarism_checker
# Enter first fileA : test1.cpp
# Enter second fileB : test2.cpp
```

---

## Example Output

```
Length of LCS: 467
Pattern matches of length >= 10 from submission1:
Match starting at submission1[195]: Length = 101
Match starting at submission1[296]: Length = 260
Match starting at submission1[576]: Length = 17
Match starting at submission1[593]: Length = 45
plag : 1
sum_of_all_matched_patterns : 423
max_len : 432
position_1 : 128
position_2 : 77
Fuzzy block spans test1.cpp lines 29-104  and  test2.cpp lines 34-85
```

### Output Field Reference

| Field | Meaning |
|---|---|
| `Length of LCS` | Number of tokens in the longest common subsequence of the two files |
| `Pattern matches ...` | Each verbatim run of >= 10 tokens found in both files, with its start index in file A and its length |
| `plag` | `1` = plagiarism detected (score > 0.25), `0` = clean |
| `sum_of_all_matched_patterns` | Total tokens covered by exact runs of >= 10; input to the score formula |
| `max_len` | Span (in tokens) of the longest fuzzy block satisfying the +23 / 80% rule |
| `position_1` | Token index in file A where the longest fuzzy block starts |
| `position_2` | Token index in file B where the longest fuzzy block starts |
| `Fuzzy block spans ...` | Source-line range corresponding to the longest fuzzy block in each file |
