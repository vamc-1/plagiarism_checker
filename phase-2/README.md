 
#bash
sudo apt update
sudo apt install build-essential llvm-dev libclang-dev
==========================
==========================
  1.Compile the plagiarism‑engine objects
#bash
     g++ -std=c++20     -I/usr/lib/llvm-16/include     -L/usr/lib/llvm-16/lib     tokenizer.cpp phase2_1.cpp main.cpp     -o plagiarism_checker     -lclang     -pthread     -Wl,-rpath,/usr/lib/llvm-16/lib
==========================

==========================
  2.Run Multiple comparisons
#bash
     ./plagiarism_checker .
 ==========================
  
eg.(output of phase-2)
     
==========================
🏁 Testcase Execution Started
==========================

📘 Loading students from: ./students.txt
  - Student added: Alice
  - Student added: Bob
  - Student added: Charlie
  - Student added: chintu
  - Student added: vamsi
  - Student added: kako3
📗 Loading professors from: ./professors.txt
  - Professor added: Dr.Smith
  - Professor added: Dr.Johnson
📄 Loading original submissions from: ./originals.txt
  - Original submission: ID=1, Student=Alice, Professor=Dr.Smith, File=phase2_1.cpp
  - Original submission: ID=2, Student=Bob, Professor=Dr.Johnson, File=tokenizer.cpp
==========================
📝 Processing New Submissions:
==========================

==========================
⏱️  Time: 1.50 | Processing submission ID=3 from chintu for Prof. Dr.Smith (phase2_1.cpp)
==========================
==========================
⏱️  Time: 2.10 | Processing submission ID=4 from Charlie for Prof. Dr.Smith (phase2_2.cpp)
==========================
==========================
🔍 Checking submission ID=3 from chintu for Prof. Dr.Smith (./phase2_1.cpp)
==========================
==========================
⏱️  Time: 2.10 | Processing submission ID=5 from vamsi for Prof. Dr.Johnson (main.cpp)
==========================
==========================
⏱️  Time: 3.30 | Processing submission ID=6 from kako3 for Prof. Dr.Johnson (tokenizer.cpp)
==========================
==========================
🔴 Student Flagged: I was flagged in phase2_1.cpp and must defend myself in front of Prof. Dr.Smith.
==========================
==========================
⚠️  Plagiarism Detected: Student chintu has plagiarized in phase2_1.cpp and will be receiving an FR grade.
==========================
🚩 Plagiarism (submitted later): ID=3 suspected source ID=1
==========================
==========================
🔍 Checking submission ID=4 from Charlie for Prof. Dr.Smith (./phase2_2.cpp)
==========================
==========================
❌ Submission ID=4 could not be processed (tokenization failed).
==========================
==========================
🔍 Checking submission ID=5 from vamsi for Prof. Dr.Johnson (./main.cpp)
==========================
==========================
✅ Submission ID=5 is clean.
==========================
==========================
🔍 Checking submission ID=6 from kako3 for Prof. Dr.Johnson (./tokenizer.cpp)
==========================
==========================
🔴 Student Flagged: I was flagged in tokenizer.cpp and must defend myself in front of Prof. Dr.Johnson.
==========================
==========================
⚠️  Plagiarism Detected: Student kako3 has plagiarized in tokenizer.cpp and will be receiving an FR grade.
==========================
==========================
🚩 Plagiarism (submitted later): ID=6 suspected source ID=2.
==========================

==========================
✅ Testcase Execution Completed.
==========================

-->Incase of any errors in parsing the submission.cpp files ,then the errors were noted in errorlog file

