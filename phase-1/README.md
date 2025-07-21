```
sudo apt update 
sudo apt install build-essential llvm-dev libclang-dev
```


 1. Build the tokenizer executable : 
 ----------------------
    g++ -std=c++20 t_main.cpp tokenizer.cpp $(llvm-config --cxxflags --ldflags --system-libs) -fexceptions -lclang -o tokenizer
    
 check: ( /*  Gives the Tokens for 1st 10-Lines of code  */ )
   ./tokenizer sample.cpp | head
   
----------------------
2.Compile the plagiarism‑engine objects : 

     g++ -std=c++20  -I/usr/lib/llvm-16/include -L/usr/lib/llvm-16/lib tokenizer.cpp plagiarism_checker.cpp p_main.cpp -o plagiarism_checker -lclang -pthread -Wl,-rpath,/usr/lib/llvm-16/lib
     
3.Run a single comparison

     ./plagiarism_checker 
     
eg.(output of phase-1)
     
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
          Fuzzy block spans  test1.cpp lines 29‑104  and  test2.cpp lines 34‑85

