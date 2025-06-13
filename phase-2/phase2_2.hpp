#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>  
#include <unordered_map>  
#include <unordered_set>        
#include <mutex>            // For std::mutex and std::unique_lock
#include <condition_variable> 
#include <sstream>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::vector<std::shared_ptr<submission_t>> database; // Stores all submissions
    std::queue<std::shared_ptr<submission_t>> submission_queue; // Queue for asynchronous processing
    std::unordered_map<long, std::chrono::time_point<std::chrono::steady_clock>>submission_timestamps;
    std::mutex mtx; // Mutex for shared resources
    std::condition_variable cv; // Condition variable for synchronization
    std::thread worker_thread; // Thread for processing submissions
    bool stop_processing = false; // Flag to terminate the thread

    void process_submissions(); // Worker thread function
    bool check_plagiarism(const std::shared_ptr<submission_t>& new_submission,
                          const std::shared_ptr<submission_t>& old_submission);
    bool is_patchwork_plagiarism(const std::shared_ptr<submission_t>& new_submission);

    void flag_plagiarized(std::shared_ptr<submission_t> plagiarized, std::shared_ptr<submission_t> source);
    bool are_timestamps_close(const std::shared_ptr<submission_t>& s1,const std::shared_ptr<submission_t>& s2);
std::string join_tokens(const std::vector<int>& tokens, size_t start, size_t end);
    bool is_later(const std::shared_ptr<submission_t>& s1,
                  const std::shared_ptr<submission_t>& s2);
    // End TODO
};