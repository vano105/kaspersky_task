#pragma once

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    Logger(const std::string& log_path);
    ~Logger();
    
    void logResult(const std::string& file_path, const std::string& hash, 
                   const std::string& verdict, bool is_malicious);
    
private:
    std::ofstream log_file_;
    std::mutex log_mutex_;
};