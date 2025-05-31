#include "phase2_2.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Constructor
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        database = __submissions;
        for (auto& submission : database) {
            submission_timestamps[submission->id] = std::chrono::steady_clock::time_point();
        }
    }
    worker_thread = std::thread(&plagiarism_checker_t::process_submissions, this);
}

// Destructor
plagiarism_checker_t::~plagiarism_checker_t() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop_processing = true;
    }
    cv.notify_all();
    if (worker_thread.joinable()) {
        worker_thread.join();
    }
}

// Add a new submission to the queue
void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> submission) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        submission_queue.push(submission);
        submission_timestamps[submission->id] = std::chrono::steady_clock::now();
    }
    cv.notify_one();
}

// Worker thread process
void plagiarism_checker_t::process_submissions() {
    while (true) {
        std::shared_ptr<submission_t> submission;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return !submission_queue.empty() || stop_processing; });
if (stop_processing && submission_queue.empty()) {
               
                break;
            }
            

            submission = submission_queue.front();
            submission_queue.pop();
        }
         
bool plagiarism_detected = false;
       
        // Check plagiarism
        for (const auto& existing_submission : database) {
            // Debug: Print tokens being compared
             
            if (check_plagiarism(submission, existing_submission)) {
                plagiarism_detected = true;

                // If timestamps differ >= 1 second, flag the later submission
                if (are_timestamps_close(submission, existing_submission)) {
                    
                    flag_plagiarized(submission, existing_submission);
                } else {
                    if (is_later(submission, existing_submission)) {
                       
                        flag_plagiarized(submission, nullptr);
                    } else {
                       
                        flag_plagiarized(existing_submission, nullptr);
                    }
                }
                break;
            }
        }
          
if(!plagiarism_detected){
        if (is_patchwork_plagiarism(submission)) {
            
            flag_plagiarized(submission, nullptr);
            
        }
}

        // Add the submission to the database
        {
            std::lock_guard<std::mutex> lock(mtx);
            database.push_back(submission);
             
        }
    }
}

// Check for exact plagiarism
bool plagiarism_checker_t::check_plagiarism(const std::shared_ptr<submission_t>& new_submission,
                                            const std::shared_ptr<submission_t>& old_submission) {
    tokenizer_t new_tokens(new_submission->codefile);
    tokenizer_t old_tokens(old_submission->codefile);

    auto new_stream = new_tokens.get_tokens();
    auto old_stream = old_tokens.get_tokens();

    size_t match_count = 0;
    for (size_t i = 0; i < new_stream.size(); ++i) {
        for (size_t j = 0; j < old_stream.size(); ++j) {
            size_t match_length = 0;

            // Find the length of the current matching pattern
            while (i + match_length < new_stream.size() &&
                   j + match_length < old_stream.size() &&
                   new_stream[i + match_length] == old_stream[j + match_length]) {
                ++match_length;
            }

            

            if (match_length >= 75) {
                return true; // Exact match of length >= 75 found
            }

            if (match_length >= 15) {
                ++match_count;
                if (match_count >= 10) {
                    return true; // Found 10 or more pattern matches of length >= 15
                }

                // Skip over the matched pattern to avoid redundant checks
                i += match_length - 1;
                j += match_length - 1;
            }
        }
    }

    return false;
}

// Check for patchwork plagiarism
bool plagiarism_checker_t::is_patchwork_plagiarism(const std::shared_ptr<submission_t>& new_submission) {
    if (!new_submission) {
        return false;
    }

    tokenizer_t new_tokens(new_submission->codefile);
    auto new_stream = new_tokens.get_tokens();

    if (new_stream.size() < 15) {
        return false;
    }

    std::unordered_set<std::string> matched_patterns;

    for (const auto& old_submission : database) {
        if (!old_submission) {
            continue;
        }

        tokenizer_t old_tokens(old_submission->codefile);
        auto old_stream = old_tokens.get_tokens();

        if (old_stream.size() < 15) {
            continue;
        }

        for (size_t i = 0; i <= new_stream.size() - 15; ) { // Increment handled conditionally
    std::string new_pattern = join_tokens(new_stream, i, i + 15);

    bool match_found = false; // Track if a match is found in the inner loop
    for (size_t j = 0; j <= old_stream.size() - 15; ++j) {
        std::string old_pattern = join_tokens(old_stream, j, j + 15);

        if (new_pattern == old_pattern) {
            matched_patterns.insert(new_pattern);

            // Skip over the matched pattern to avoid overlap
            i += 15; // Move i forward by the length of the pattern
            match_found = true;

            if (matched_patterns.size() >= 20) {
                return true; // Detected patchwork plagiarism
            }

            break; // Stop further comparisons for the current new_pattern
        }
    }

    // If no match found, increment i normally
    if (!match_found) {
        ++i;
    }
}

    }

    return false;
}

// Flag plagiarized submissions
void plagiarism_checker_t::flag_plagiarized(std::shared_ptr<submission_t> plagiarized, std::shared_ptr<submission_t> source) {
    if (plagiarized) {
        if (plagiarized->student) {
            plagiarized->student->flag_student(plagiarized);
        }
        if (plagiarized->professor) {
            plagiarized->professor->flag_professor(plagiarized);
        }
    }

    if (source) {
        if (source->student) {
            source->student->flag_student(source);
        }
        if (source->professor) {
            source->professor->flag_professor(source);
        }
    }
}

// Utility function to join tokens into a pattern string
std::string plagiarism_checker_t::join_tokens(const std::vector<int>& tokens, size_t start, size_t end) {
    std::ostringstream result;
    for (size_t i = start; i < end && i < tokens.size(); ++i) {
        if (i > start) {
            result << " ";
        }
        result << tokens[i];
    }
    return result.str();
}
// Check if timestamps differ by less than one second
bool plagiarism_checker_t::are_timestamps_close(const std::shared_ptr<submission_t>& s1,
                                                const std::shared_ptr<submission_t>& s2) {
    auto timestamp1 = submission_timestamps[s1->id];
    auto timestamp2 = submission_timestamps[s2->id];
    return std::chrono::duration_cast<std::chrono::seconds>(timestamp1 - timestamp2).count() < 1;
}

bool plagiarism_checker_t::is_later(const std::shared_ptr<submission_t>& s1,
                                    const std::shared_ptr<submission_t>& s2) {
    auto timestamp1 = submission_timestamps[s1->id];
    auto timestamp2 = submission_timestamps[s2->id];
    return timestamp1 > timestamp2; // Returns true if s1 was submitted later than s2
}

// End of file