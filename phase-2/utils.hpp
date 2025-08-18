#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include "tokenizer.hpp"  // Required for tokenizer_t

// Log error to errors.log
inline void log_error(const std::string& message) {
    std::ofstream error_log("errors.log", std::ios_base::app);
    if (error_log.is_open()) {
        error_log << message << std::endl;
    }
}

// Wrap tokenizer in a try-catch block and return optional
inline std::optional<std::vector<int>> safe_tokenize(const std::string& file) {
    try {
        tokenizer_t t(file);
        return t.get_tokens();
    } catch (const std::invalid_argument& e) {
        log_error("Failed to parse file: " + file + " | " + e.what());
        return std::nullopt;
    }
}

#endif
