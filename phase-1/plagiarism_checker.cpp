// #include <iostream>
// #include <vector>
// #include <utility>
// #include <algorithm>
// #include <map>
// #include <fstream>
// #include <set>
// #include <sstream>
// std::vector<int> getTokenSequence(const std::string& filename) {
//     std::vector<int> tokens;
//     std::string command = "./tokenizer " + filename;
//     FILE* pipe = popen(command.c_str(), "r");
//     if (!pipe) {
//         std::cerr << "Failed to run tokenizer on " << filename << std::endl;
//         return tokens;
//     }

//     char buffer[128];
//     while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//         std::istringstream iss(buffer);
//         int code;
//         std::string dash, label;
//         if (iss >> code >> dash >> label) {
//             tokens.push_back(code); // use just the code for LCS
//         }
//     }
//     pclose(pipe);
//     return tokens;
// }
// std::pair<int, std::vector<std::pair<int,int>>> longestCommonSubsequenceWithPositions(const std::vector<int>& submission1, const std::vector<int>& submission2) {
//     int size_1=submission1.size();
//     int size_2=submission2.size();
//     std::vector<std::vector<int>> dp(size_1 + 1, std::vector<int>(size_2 + 1, 0));
//     for (int i=1;i<=size_1;i++){
//         for (int j=1;j<=size_2;j++){
//             if(submission1[i-1]==submission2[j-1]){ 
//                 dp[i][j]=dp[i-1][j-1] + 1;
//             } 
//             else{
//                 dp[i][j]=std::max(dp[i-1][j],dp[i][j-1]);
//             }
//         }
//     }
//     // Positions in the respective vectors that are matched as subsequence(longest) in the form (a,b) and Backtracking to find the positions of elements in the LCS
//     std::vector<std::pair<int, int>> lcs_positions; // a is the position in vector 1 and b is the position in vector2 in pair(a,b)
//     int i=size_1;
//     int j=size_2;
//     while(i>0 && j>0){
//         if(submission1[i-1]==submission2[j-1]){
//             lcs_positions.push_back({i-1,j-1});
//             i=i-1;
//             j=j-1;
//         }
//         else if(dp[i-1][j]>=dp[i][j-1]){
//             i=i-1;
//         }
//         else{
//             j=j-1;
//         }
//     }
//     std::reverse(lcs_positions.begin(), lcs_positions.end());
//     return {dp[size_1][size_2],lcs_positions};
// }
// // Function to find the maximum matching pattern length from a given position (i) in submission1. Also updates the positions in submission2 such that the matched things are not recounted twice.
// //std::map<int,std::set<int>>& submission2_positions , used set to track positions , since O(logn) deletion and O(logn) search for set.
// int find_max_string_from(std::map<int,std::set<int>>& submission2_positions,std::vector<int>& submission1,int i,int size){
//     int count=0;
//     std::vector<int> remover;
//     for(int index_in_2:submission2_positions[submission1[i]]){
//         std::vector<int> tester;
//         int rotor=1;
//         tester.push_back(index_in_2);
// //iterating over the next indexes and checking whether the index needed is present in the next numbers positions set in submission2
//         while(i+rotor<submission1.size() && index_in_2+rotor<size && submission2_positions[submission1[i+rotor]].count(index_in_2+rotor)){
//             tester.push_back(index_in_2+rotor);
//             rotor=rotor+1;
//         }
//         if(rotor>count){
//             remover=tester;
//             count=rotor;
//         }
//     }
//     //erasing already matched positions .to get no overlap
//     if(count>=10){
//         for(int j=0;j<remover.size();j++){
//             submission2_positions[submission1[i+j]].erase(remover[j]);
//         }
//     }
//     else{
//         return 1;
//     }
//     return count;
// }
// // ---------- Main Function ----------
// int main() {

//     std::string fileA, fileB;
//     std::cout << "Enter first fileA : ";
//     std::cin >> fileA;
//     std::ifstream fileCheckA(fileA);
//     if (!fileCheckA.is_open()) {
//     std::cerr << "Error: File \"" << fileA << "\" not found in current directory.\n";
//     return 1;
// }
//     std::cout << "Enter second fileB : ";
//     std::cin >> fileB;
//        std::ifstream fileCheckB(fileB);
//        if (!fileCheckB.is_open()) {
//     std::cerr << "Error: File \"" << fileB << "\" not found in current directory.\n";
//     return 1;
// }
//     std::vector<int> submission1 =    getTokenSequence(fileA);
   
//     std::vector<int> submission2 =    getTokenSequence(fileB);
//   std::cout<<"tokenized_vector_of_fileA(submission1) : ";
//     for(int i=0;i<submission1.size();i++) std::cout<<submission1[i]<<" ";
//     std::cout<<std::endl<<std::endl;
//     std::cout<<"tokenized_vector_of_fileB(submission2) : ";
//     for(int i=0;i<submission2.size();i++) std::cout<<submission2[i]<<" ";
//     std::cout<<std::endl<<std::endl;
 

// // Call LCS function
//    auto result = longestCommonSubsequenceWithPositions(submission1,submission2);// Obtaining the LCS length and positions of matched elements
//     int length = result.first;
//     std::cout << "Length of LCS: " << length << "\n";
   
//     std::cout << "LCS Matching Positions (submission1_index, submission2_index):\n";
//     for (const auto& [idx1, idx2] : result.second) {
//         std::cout << "(" << idx1 << ", " << idx2 << ") ";
//     }
//     std::cout << "\n\n";

    
//     // std::cout << "\n\n";
//     std::vector<std::pair<int, int>> positions = result.second;

//     int max_len=0;
//     int position_1;
//     int position_2;
//     for(int i=0;i<positions.size();i++){
//         for(int j=i+23;j<positions.size();j++){
//             if(j-i+1>(double)(0.8*(positions[j].first-positions[i].first+1))){
//                 if(j-i+1>(double)(0.8*(positions[j].second-positions[i].second+1))){
//                     if(std::max(positions[j].first-positions[i].first+1,positions[j].second-positions[i].second+1)>max_len){
//                         //updating the start positions whenever new max is found
//                         position_1=positions[i].first;
//                         position_2=positions[i].second;
//                         max_len=std::max({positions[j].first-positions[i].first+1,positions[j].second-positions[i].second+1});
//                     }
//                 }
//             }
//         }
//     }

//     // Prepare map for pattern matching
//     std::map<int, std::set<int>> submission2_positions;
//     for (int i = 0; i < submission2.size(); ++i) {
//         submission2_positions[submission2[i]].insert(i);
//     }
// int sum=0;
//     // Call pattern-matching function from every index
//    int cnt=0;
//     for (int i = 0; i < submission1.size(); ++i) {
//         int match_len = find_max_string_from(submission2_positions, submission1, i, submission2.size());
//         if (match_len >= 10) {
//             cnt++;
//             if(cnt==1)  std::cout << "Pattern matches of length >= 10 from submission1:\n";
//             sum +=match_len;
//             std::cout << "Match starting at submission1[" << i << "]: Length = " << match_len << "\n";
//             i += match_len - 1; // Skip matched part
//         }
//     }
// //     std::cout<<std::endl;
// //   std::cout<<submission1.size()<<std::endl;
// int plag=0;
// int n1=submission1.size();
// int n2=submission2.size();
//   // used method determining plag 
//     //(n1/n1+n2)(x/n2)+(n2/n1+n2)(x/n1)) , where n1,n2 are sizes and x is the sum attained;
// double a=((double)n1/(n1+n2));
// double b=((double)n2/(n1+n2));
// double val=((double)sum/n1)*a + ((double)sum/n2)*b;
// if(val > 0.25) plag=1;
// std::cout<<"plag : "<<plag<<std::endl;
// std::cout<<"sum_of_all_matched_patterns : "<<sum<<std::endl;
// std::cout<<"max_len : "<<max_len<<std::endl;
// std::cout<<"position_1 : "<<position_1<<std::endl;
// std::cout<<"position_2 : "<<position_2<<std::endl;

//     return 0;
// }


#include "plagiarism_checker.hpp"
#include <bits/stdc++.h>

// ──────────────────────────────────────────────────────────────
Tokens tokenizeFile(const std::string& file)
{
    Tokens toks;

    std::string cmd = "./tokenizer " + file;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "Tokenizer failed on " << file << '\n';
        return toks;
    }

    char buf[256];
    while (fgets(buf, sizeof(buf), pipe)) {
        std::istringstream is(buf);

        int id; std::string dash, label;
        unsigned line = 0, column = 0;

        // Expect:  <id> - <label> <line> <column>
        if (is >> id >> dash >> label >> line >> column) {
            toks.push_back({id, line, column});
        }
    }
    pclose(pipe);
    return toks;
}


// ──────────────────────────────────────────────────────────────
std::pair<int,Positions>
lcsWithPositions(const Tokens& A, const Tokens& B)
{
    int n=A.size(), m=B.size();
    std::vector<std::vector<int>> dp(n+1, std::vector<int>(m+1,0));
    for (int i=1;i<=n;++i)
        for (int j=1;j<=m;++j)
            dp[i][j] = (A[i-1].kind == B[j-1].kind) ? dp[i-1][j-1]+1
                                         : std::max(dp[i-1][j], dp[i][j-1]);

    Positions pos;
    for (int i=n,j=m; i&&j; )
        if (A[i-1].kind == B[j-1].kind) { pos.push_back({i-1,j-1}); --i; --j; }
        else if (dp[i-1][j]>=dp[i][j-1]) --i;
        else                             --j;
    std::reverse(pos.begin(), pos.end());
    return {dp[n][m],pos};
}

// ──────────────────────────────────────────────────────────────
// void printTokens(const std::string& label, const Tokens& t)
// {
//     std::cout << label << " : ";
//     for (const TokenInfo& tok : t)
//         std::cout << tok.kind << ' ';          // or also print line/column
//     std::cout << "\n\n";
// }


// ──────────────────────────────────────────────────────────────
int longestExactRunFrom(std::map<int,std::set<int>>& idx2,
                        const Tokens& A, int i, int Bsize)
{
    int best=0; std::vector<int> bestPos;
    for (int j0: idx2[A[i].kind]) {
        int run=1; std::vector<int> cur{j0};
        while (i+run<A.size() && j0+run<Bsize &&
               idx2[A[i+run].kind].count(j0+run))
        { cur.push_back(j0+run); ++run; }
        if (run>best){ best=run; bestPos.swap(cur);}
    }
    if (best>=10)
        for (int k=0;k<best;++k) idx2[A[i+k].kind].erase(bestPos[k]);
    else best=1;
    return best;
}

// ──────────────────────────────────────────────────────────────
//  ***  +23 / 80 % fuzzy‑block rule lives here  ***
int longestFuzzyBlock(const Positions& v,int& sA,int& sB)
{
    int maxLen=0;
    for (int i=0;i<(int)v.size();++i)
        for (int j=i+23;j<(int)v.size();++j)
            if ( (j-i+1) > 0.8*(v[j].first  - v[i].first  +1) &&
                 (j-i+1) > 0.8*(v[j].second - v[i].second +1))
            {
                int cand = std::max(v[j].first -v[i].first +1,
                                    v[j].second-v[i].second+1);
                if (cand>maxLen){ maxLen=cand; sA=v[i].first; sB=v[i].second; }
            }
    return maxLen;
}

// ──────────────────────────────────────────────────────────────
double plagiarismScore(const Tokens& A,const Tokens& B,int sum)
{
    double n1=A.size(), n2=B.size();
    return (n1/(n1+n2))*(sum/n2) + (n2/(n1+n2))*(sum/n1);
}

// ──────────────────────────────────────────────────────────────
void analyzePair(const std::string& fA, const std::string& fB)
{
    Tokens A = tokenizeFile(fA);
    Tokens B = tokenizeFile(fB);

    auto [lcsLen, pos] = lcsWithPositions(A, B);
    std::cout << "Length of LCS: " << lcsLen << "\n";

    int position_1 = 0, position_2 = 0;
    int max_len = longestFuzzyBlock(pos, position_1, position_2);

    std::map<int, std::set<int>> submission2_positions;
    for (int j = 0; j < (int)B.size(); ++j)
        submission2_positions[B[j].kind].insert(j);

    int sum = 0, cnt = 0;
    for (int i = 0; i < (int)A.size(); ++i) {
        int match_len = longestExactRunFrom(submission2_positions, A, i, B.size());
        if (match_len >= 10) {
             /* ----------  PRINT BLOCK  ---------- */
        if (cnt == 0) {
            std::cout << "Pattern matches of length >= 10 from submission1:\n";
        }
        std::cout << "Match starting at submission1[" << i
                  << "]: Length = " << match_len << "\n";
        /* ----------------------------------- */
            cnt++;
            sum += match_len;
            i += match_len - 1;
        }
    }
if (cnt == 0) {
    std::cout << "No pattern matches of length >= 10 were found in submission1.\n";
}

    double score = plagiarismScore(A, B, sum);
    int plag = (score > 0.25) ? 1 : 0;

     // Check if a fuzzy block was found
if (max_len > 0 &&
    position_1 + max_len - 1 < A.size() &&
    position_2 + max_len - 1 < B.size()) {

    // line‑range for the longest fuzzy block
    unsigned startLineA = A[position_1].line;
    unsigned endLineA   = A[position_1 + max_len - 1].line;
    unsigned startLineB = B[position_2].line;
    unsigned endLineB   = B[position_2 + max_len - 1].line;

    // Match your original print format
    std::cout << "plag : " << plag << std::endl;
    std::cout << "sum_of_all_matched_patterns : " << sum << std::endl;
    std::cout << "max_len : " << max_len << std::endl;
    std::cout << "position_1 : " << position_1 << std::endl;
    std::cout << "position_2 : " << position_2 << std::endl;

    // ----- NEW Tester‑readable range -----
    std::cout << "Fuzzy block spans "
              << fA << " lines " << startLineA << "‑" << endLineA
              << "  and  "
              << fB << " lines " << startLineB << "‑" << endLineB << '\n';

} else {
    std::cout << "No valid fuzzy block found or indices out of range.\n";
    std::cout << "plag : " << plag << std::endl;
    std::cout << "sum_of_all_matched_patterns : " << sum << std::endl;
    std::cout << "max_len : 0\n";
}
}

