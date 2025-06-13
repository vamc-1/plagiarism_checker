#include "phase2_2.hpp"

#include <map>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iomanip>

// You should NOT modify class logic. Only logging and formatting are enhanced.

void student_t::flag_student(std::shared_ptr<submission_t> __submission) {
    int i = 0;
    while (__submission->codefile[i] != '/') i++;
    i++;
    std::cout << "🔴 Student Flagged: I was flagged in " << __submission->codefile.substr(i)
              << " and must defend myself in front of Prof. "
              << __submission->professor->get_name() << "." << std::endl;
}

void professor_t::flag_professor(std::shared_ptr<submission_t> __submission) {
    int i = 0;
    while (__submission->codefile[i] != '/') i++;
    i++;
    std::cout << "⚠️  Plagiarism Detected: Student " << __submission->student->get_name()
              << " has plagiarized in " << __submission->codefile.substr(i)
              << " and will be receiving an FR grade." << std::endl << std::endl;
}

namespace testcase {
    std::string test_dir;
    std::map<std::string, std::shared_ptr<student_t>> students;
    std::map<std::string, std::shared_ptr<professor_t>> professors;
    std::vector<std::shared_ptr<submission_t>> pre_existing_codes;
    void get_students(void);
    void get_professors(void);
    void get_pre_existing_codes(void);
    void execute_testcase(std::string test_dir);
}

void testcase::get_students(void) {
    std::string filename = testcase::test_dir + "/students.txt";
    std::ifstream in(filename);
    std::string student_name;
    std::cout << "📘 Loading students from: " << filename << std::endl;
    while (in >> student_name) {
        testcase::students[student_name] = std::make_shared<student_t>(student_name);
        std::cout << "  - Student added: " << student_name << std::endl;
    }
    in.close();
}

void testcase::get_professors(void) {
    std::string filename = testcase::test_dir + "/professors.txt";
    std::ifstream in(filename);
    std::string professor_name;
    std::cout << "📗 Loading professors from: " << filename << std::endl;
    while (in >> professor_name) {
        testcase::professors[professor_name] = std::make_shared<professor_t>(professor_name);
        std::cout << "  - Professor added: " << professor_name << std::endl;
    }
    in.close();
}

void testcase::get_pre_existing_codes(void) {
    std::string filename = testcase::test_dir + "/originals.txt";
    std::ifstream in(filename);
    long id;
    std::string student_name, professor_name, code_file;
    std::cout << "📄 Loading original submissions from: " << filename << std::endl;
    while (in >> id >> student_name >> professor_name >> code_file) {
        auto submission = std::make_shared<submission_t>();
        submission->id = id;
        submission->student = testcase::students[student_name];
        submission->professor = testcase::professors[professor_name];
        submission->codefile = testcase::test_dir + "/" + code_file + ".cpp";
        testcase::pre_existing_codes.push_back(submission);
        std::cout << "  - Original submission: ID=" << id
                  << ", Student=" << student_name
                  << ", Professor=" << professor_name
                  << ", File=" << code_file << ".cpp" << std::endl;
    }
    in.close();
}

void testcase::execute_testcase(std::string __test_dir) {
    testcase::test_dir = __test_dir;
    std::cout << "==========================" << std::endl;
    std::cout << "🏁 Testcase Execution Started" << std::endl;
    std::cout << "==========================" << std::endl << std::endl;

    testcase::get_students();
    testcase::get_professors();
    testcase::get_pre_existing_codes();

    plagiarism_checker_t checker(testcase::pre_existing_codes);

    std::ifstream in(testcase::test_dir + "/submissions.txt");
    double timestamp, old_timestamp = 0.0;
    long id;
    std::string student_name, prof_name, code_file;

    std::cout << std::endl << "📝 Processing New Submissions:" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    while (in >> timestamp >> id >> student_name >> prof_name >> code_file) {
        double wait_time = timestamp - old_timestamp;
        std::cout << "⏱️  Time: " << std::fixed << std::setprecision(2) << timestamp
                  << " | Processing submission ID=" << id
                  << " from " << student_name
                  << " for Prof. " << prof_name
                  << " (" << code_file << ".cpp)" << std::endl;

        usleep(wait_time * 1000000);

        auto submission = std::make_shared<submission_t>();
        submission->id = id;
        submission->student = testcase::students[student_name];
        submission->professor = testcase::professors[prof_name];
        submission->codefile = testcase::test_dir + "/" + code_file + ".cpp";

        checker.add_submission(submission);
        old_timestamp = timestamp;
    }

    in.close();
    std::cout << std::endl << "✅ Testcase Execution Completed." << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "❌ Usage: " << argv[0] << " <test_directory>" << std::endl;
        return 1;
    }
    testcase::execute_testcase(argv[1]);
    return 0;
}
