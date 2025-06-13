#include "phase2_2.hpp"
#include "utils.hpp"

// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

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

        std::cout << "🔍 Checking submission ID=" << submission->id << " from "
                  << (submission->student ? submission->student->get_name() : "Unknown Student")
                  << " for Prof. "
                  << (submission->professor ? submission->professor->get_name() : "Unknown Professor")
                  << " (" << submission->codefile << ")\n";

        // ✅ Early tokenization check
        auto token_check = safe_tokenize(submission->codefile);
        if (!token_check.has_value()) {
            std::cout << "❌ Submission ID=" << submission->id << " could not be processed (tokenization failed).\n"<<"\n";
            
            // Optional: push even invalid submissions to DB if needed
            {
                std::lock_guard<std::mutex> lock(mtx);
                database.push_back(submission);
            }

            continue; // skip rest of logic
        }

        bool plagiarism_detected = false;
        bool patchwork_flagged = false;

        for (const auto& existing_submission : database) {
            if (check_plagiarism(submission, existing_submission)) {
                plagiarism_detected = true;

                if (are_timestamps_close(submission, existing_submission)) {
                    flag_plagiarized(submission, existing_submission);
                    std::cout << "🚩 Exact plagiarism detected: ID=" << submission->id
                              << " copied from ID=" << existing_submission->id << "\n"<< "\n";
                } else {
                    if (is_later(submission, existing_submission)) {
                        flag_plagiarized(submission, nullptr);
                        std::cout << "🚩 Plagiarism (submitted later): ID=" << submission->id
                                  << " suspected source ID=" << existing_submission->id << "\n"<< "\n";
                    } else {
                        flag_plagiarized(existing_submission, nullptr);
                        std::cout << "🚩 Plagiarism (submitted earlier): Source ID=" << existing_submission->id
                                  << " flagged due to ID=" << submission->id << "\n"<< "\n";
                    }
                }
                break;
            }
        }

        if (!plagiarism_detected) {
            if (is_patchwork_plagiarism(submission)) {
                flag_plagiarized(submission, nullptr);
                patchwork_flagged = true;
                std::cout << "⚠️  Patchwork plagiarism detected in submission ID="
                          << submission->id << "\n"<<"\n";
            }
        }

        if (!plagiarism_detected && !patchwork_flagged) {
            std::cout << "✅ Submission ID=" << submission->id << " is clean.\n"<< "\n";
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
    // std::vector<int> new_stream, old_stream;

   auto new_tokens_opt = safe_tokenize(new_submission->codefile);
auto old_tokens_opt = safe_tokenize(old_submission->codefile);

if (!new_tokens_opt.has_value() || !old_tokens_opt.has_value()) {
    return false;
}
std::vector<int> new_stream = new_tokens_opt.value();
std::vector<int> old_stream = old_tokens_opt.value();



    size_t match_count = 0;
    for (size_t i = 0; i < new_stream.size(); ++i) {
        for (size_t j = 0; j < old_stream.size(); ++j) {
            size_t match_length = 0;
            while (i + match_length < new_stream.size() &&
                   j + match_length < old_stream.size() &&
                   new_stream[i + match_length] == old_stream[j + match_length]) {
                ++match_length;
            }

            if (match_length >= 75) return true;

            if (match_length >= 15) {
                ++match_count;
                if (match_count >= 10) return true;

                i += match_length - 1;
                j += match_length - 1;
            }
        }
    }

    return false;
}

// Check for patchwork plagiarism
bool plagiarism_checker_t::is_patchwork_plagiarism(const std::shared_ptr<submission_t>& new_submission) {
    if (!new_submission) return false;

    // std::vector<int> new_stream;
 auto new_tokens_opt = safe_tokenize(new_submission->codefile);
if (!new_tokens_opt.has_value()) return false;

std::vector<int> new_stream = new_tokens_opt.value();  



    if (new_stream.size() < 15) return false;

    std::unordered_set<std::string> matched_patterns;

    for (const auto& old_submission : database) {
        if (!old_submission) continue;

        // std::vector<int> old_stream;
     auto old_tokens_opt = safe_tokenize(old_submission->codefile);
if (!old_tokens_opt.has_value()) continue;

std::vector<int> old_stream = old_tokens_opt.value();


        if (old_stream.size() < 15) continue;

        for (size_t i = 0; i <= new_stream.size() - 15;) {
            std::string new_pattern = join_tokens(new_stream, i, i + 15);
            bool match_found = false;

            for (size_t j = 0; j <= old_stream.size() - 15; ++j) {
                std::string old_pattern = join_tokens(old_stream, j, j + 15);
                if (new_pattern == old_pattern) {
                    matched_patterns.insert(new_pattern);
                    i += 15;
                    match_found = true;
                    if (matched_patterns.size() >= 20) return true;
                    break;
                }
            }

            if (!match_found) ++i;
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
        if (i > start) result << " ";
        result << tokens[i];
    }
    return result.str();
}

// Timestamp checks
bool plagiarism_checker_t::are_timestamps_close(const std::shared_ptr<submission_t>& s1,
                                                const std::shared_ptr<submission_t>& s2) {
    auto timestamp1 = submission_timestamps[s1->id];
    auto timestamp2 = submission_timestamps[s2->id];

    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp1 - timestamp2).count();
    return std::abs(diff) <= 100;  // treat as "close" if within 100ms
}


bool plagiarism_checker_t::is_later(const std::shared_ptr<submission_t>& s1,
                                    const std::shared_ptr<submission_t>& s2) {
    auto timestamp1 = submission_timestamps[s1->id];
    auto timestamp2 = submission_timestamps[s2->id];

    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp1 - timestamp2).count();

    if (std::abs(diff) <= 100) {
        return s1->id > s2->id;  // fallback to submission order if timestamps are too close
    }

    return diff > 0;  // true if s1 is definitely later
}


// End of file
