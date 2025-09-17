#pragma once

#ifdef _WIN32
    #ifdef SCANNER_EXPORTS
        #define SCANNER_API __declspec(dllexport)
    #else
        #define SCANNER_API __declspec(dllimport)
    #endif
#else
    #define SCANNER_API
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <queue>
#include <filesystem>
#include "logger/logger.hpp"

class SCANNER_API Scanner {
public:
    Scanner(const std::filesystem::path &path_to_csv, const std::filesystem::path &path_to_dir, const std::filesystem::path &path_to_log)
    : path_to_csv_(path_to_csv), path_to_dir_(path_to_dir), path_to_log_(path_to_log), 
      logger_(path_to_log), current_index_(0) {
    }

    void run();

    ~Scanner() {
        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

private:
    std::filesystem::path path_to_csv_;
    std::filesystem::path path_to_dir_;
    std::filesystem::path path_to_log_;
    std::unordered_map<std::string, std::string> hashs_table_;
    std::vector<std::filesystem::path> file_paths_;
    Logger logger_;
    std::vector<std::thread> threads_;
    std::atomic<size_t> current_index_;

    void processFile(const std::filesystem::path &file_path);
    bool loadBase();
    std::string checkHash(const std::string& hash) const;
    void workerThread();
};