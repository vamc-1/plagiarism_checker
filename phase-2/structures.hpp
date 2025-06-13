#pragma once
#include "tokenizer.hpp"

// Forward declarations
class student_t;
class professor_t;
struct submission_t;

// Submission structure
struct submission_t {
    long id;
    std::shared_ptr<student_t> student;
    std::shared_ptr<professor_t> professor;
    std::string codefile;
};

// Student class
class student_t {
public:
    student_t(std::string __name) : name(__name) {}
    std::string get_name(void) const { return name; }
    void flag_student(std::shared_ptr<submission_t> __submission);
protected:
    std::string name;
};

// Professor class
class professor_t {
public:
    professor_t(std::string __name) : name(__name) {}
    std::string get_name(void) const { return name; }
    void flag_professor(std::shared_ptr<submission_t> __submission);
protected:
    std::string name;
};
