 
Phase 1 :  Algorithm Description & Analysis
────────────────────────────────────────────────────────────────────────────
1.  INITIALISATION AND MAIN DRIVER
────────────────────────────────────────────────────────────────────────────
Driver prototype
    void analyzePair(const std::string& fileA,
                     const std::string& fileB);

Steps
  1.1  Tokenisation
       • Launches the stand‑alone executable  ./tokenizer <file.cpp>
       • Each line printed by the tokenizer has the form
           <kind> - <spelling> <line> <column>
       • Parsed into:
           struct TokenInfo {
               int      kind;      // CXCursorKind
               unsigned line;      // 1‑based
               unsigned column;    // 1‑based
           };
         A vector<TokenInfo> is aliased as  Tokens.

  1.2  Exact‑run map
       Builds   std::map<int, std::set<int>> submission2_positions
       that stores **all indices** of every token‑kind appearing in submission 2.

  1.3  Main scan over submission 1
       For each index i:
         int match_len =
             longestExactRunFrom(submission2_positions, submission1, i, sizeB);

         if (match_len >= 10)
             – Print heading once:
                 Pattern matches of length >= 10 from submission1:
             – Print the match:
                 Match starting at submission1[i]: Length = match_len
             – Add match_len to global SUM
             – Skip ahead  (i += match_len‑1)  to avoid overlap

────────────────────────────────────────────────────────────────────────────
2.  CORE FUNCTIONS
────────────────────────────────────────────────────────────────────────────
  • Tokens tokenizeFile(const std::string& cppFile);
       ‑ Shells out to ./tokenizer and fills TokenInfo{kind,line,column}.

  • std::pair<int, Positions>
    lcsWithPositions(const Tokens& A, const Tokens& B);
       ‑ Classic DP O(n×m).  Back‑track to get vector of aligned positions.

  • int longestExactRunFrom(map<int,set<int>>& idx2,
                            const Tokens& A,
                            int posA, int sizeB);
       ‑ Finds longest contiguous identical run starting at posA.
       ‑ If run ≥ 10, erases those indices from idx2 to prevent re‑count.

  • int longestFuzzyBlock(const Positions& P,
                          int& startA, int& startB);
       ‑ Scans P for the largest range [i..j] s.t.
           blockLen ≥ 24  AND
           blockLen > 0.8·(P[j].first – P[i].first + 1)  AND
           blockLen > 0.8·(P[j].second– P[i].second+ 1)
       ‑ Returns blockLen and start indices in each submission.

  • double plagiarismScore(const Tokens& A,
                           const Tokens& B,
                           int SUM);
         n1 = |A|, n2 = |B|
         score = (n1/(n1+n2))·(SUM/n2) + (n2/(n1+n2))·(SUM/n1)

────────────────────────────────────────────────────────────────────────────
3.  CONSOLE OUTPUT 
────────────────────────────────────────────────────────────────────────────
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
Fuzzy block spans  fileA.cpp lines 37‑52  and  fileB.cpp lines 19‑34

(If no ≥10‑token runs exist, prints:
 No pattern matches of length >= 10 were found in submission1.)

────────────────────────────────────────────────────────────────────────────
4.  COMPLEXITY ANALYSIS
────────────────────────────────────────────────────────────────────────────
Stage                                   Time Complexity   Space
--------------------------------------  ----------------  ----------------
Tokenisation (external process)         O(N) per file     O(1) in checker
LCS dynamic programming                 O(n × m)          O(n × m)
Fuzzy‑block scan over positions         O(L²) (L ≤ LCS)   O(1)
Exact‑run search (greedy + sets)        Σ O(pi · log pj)  O(kinds + m)
Overall dominant                        O(n × m)          O(n × m)


Symbol | Meaning
-------|---------
n      | |A| — number of tokens in submission 1
m      | |B| — number of tokens in submission 2
k      | |K| — number of distinct cursor‑kinds across both submissions
L      | length of the LCS between A and B ( L ≤ min(n,m) )
R      | number of ≥ 10‑token exact runs detected
pᵢ     | for run rᵢ, number of candidate start positions in B considered
Σnᵢ    | sum(match_len) — the total length of all exact runs (printed as sum_of_all_matched_patterns)

1. Tokenisation
Step         | Operation                      | Time     | Space (in checker)
-------------|--------------------------------|----------|--------------------
A‑tokens     | ./tokenizer A.cpp              | O(n)     | O(1)
B‑tokens     | ./tokenizer B.cpp              | O(m)     | O(1)

Peak RAM inside the checker is O(n + m) to hold the two Tokens vectors.

2. Exact‑Run Map Construction
for j = 0 … m‑1:  submission2_positions[B[j].kind].insert(j);

Metric | Complexity
--------|------------
Time    | O(m log d), d = avg bucket size (set)
Space   | O(m + k)

3. LCS with Positions
Dynamic programming table dp[n+1][m+1]
Phase      | Time        | Space     | Notes
------------|-------------|-----------|------
Fill DP     | O(n×m)      | O(n×m)    | Integer per cell
Back-track  | O(L)        | O(L)      | Builds Positions vector

4. Fuzzy‑Block Scan
Double loop over Positions vector, worst-case ≈ L²/2

Time  | Space
------|------
O(L²) | O(1)

L is far smaller than n/m in practice.

5. Greedy Exact‑Run Detection
for each i in A:
  for j0 in positions_of_kind(A[i].kind)
    extend run

Time per i: O(pᵢ + xᵢ log pᵢ)
Sum pᵢ ≤ m, Sum xᵢ = Σnᵢ

Aggregate:
Time: O(m log d  +  Σnᵢ)
Space: uses existing map

6. Score & Printing
O(1) (arithmetic & I/O)

7. Overall Worst‑Case Bounds
Phase               | Time          | Space
--------------------|---------------|---------
Tokenisation        | O(n + m)      | O(n + m)
Build Map           | O(m log d)    | O(m + k)
LCS + backtracking  | O(n × m)      | O(n × m)
Fuzzy‑block         | O(min(n,m)²)  | O(1)
Exact-run           | O(m log d)    | —
Total               | O(n × m)      | O(n × m)

Typical scenario:
- L is small → L² subquadratic
- d is small → buckets are short
- Σnᵢ is modest

Memory Breakdown
Component             | Size
----------------------|-----------------------------
Tokens vectors        | (n + m) × sizeof(TokenInfo)
DP table (int32)      | (n+1) × (m+1) × 4
Position map          | m × 16 ≈ 16m




