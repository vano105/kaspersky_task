#include "logger.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

Logger::Logger(const std::string& log_path) {
    if (!log_path.empty()) {
        log_file_.open(log_path, std::ios::out | std::ios::app);
        if (!log_file_.is_open()) {
            std::cerr << "Failed to open log file: " << log_path << std::endl;
        }
    }
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::logResult(const std::string& file_path, const std::string& hash, 
                      const std::string& verdict, bool is_malicious) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] ";
    ss << "File: " << file_path << ", ";
    ss << "Hash: " << hash << ", ";
    
    if (is_malicious) {
        ss << "Verdict: " << verdict;
    } else {
        ss << "Verdict: Clean";
    }
    
    std::string message = ss.str();
    if (log_file_.is_open()) {
        log_file_ << message << std::endl;
    }
    std::cout << message << std::endl;
}