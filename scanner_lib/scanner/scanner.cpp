#include "scanner.hpp"
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

using steady_clock = std::chrono::steady_clock;
namespace fs = std::filesystem;

void Scanner::run() {
    auto start_time = steady_clock::now();

    if (!loadBase()) {
        throw std::runtime_error("Failed to load base: " + path_to_csv_);
    }

    for (const auto& entry : fs::recursive_directory_iterator(path_to_dir_)) {
        if (entry.is_regular_file()) {
            file_paths_.push_back(entry.path().string());
        }
    }

    unsigned int num_threads = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < num_threads; ++i) {
        threads_.emplace_back(&Scanner::workerThread, this);
    }

    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    auto end_time = steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Scan completed in " << duration.count() << " ms" << std::endl;
    std::cout << "Processed " << file_paths_.size() << " files" << std::endl;
}

void Scanner::workerThread() {
    while (true) {
        size_t index = current_index_.fetch_add(1, std::memory_order_relaxed);
        
        if (index >= file_paths_.size()) {
            break;
        }
        
        processFile(file_paths_[index]);
    }
}

void Scanner::processFile(const std::string &file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << file_path << std::endl;
        return;
    }
    
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    
    char buffer[1024 * 16];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        MD5_Update(&md5Context, buffer, file.gcount());
    }
    
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_Final(hash, &md5Context);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    
    std::string hash_str = ss.str();
    if (hash_str.empty()) {
        std::cerr << "Failed to calculate hash for file: " << file_path << std::endl;
        return;
    }
    
    std::string verdict = checkHash(hash_str);
    logger_.logResult(file_path, hash_str, verdict, !verdict.empty());
}

std::string Scanner::checkHash(const std::string& hash) const {
    auto it = hashs_table_.find(hash);
    if (it != hashs_table_.end()) {
        return it->second;
    }
    return "";
}

bool Scanner::loadBase() {
    std::ifstream file(path_to_csv_);
    if (!file.is_open()) {
        std::cerr << "Failed to open base file: " << path_to_csv_ << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t delimiter_pos = line.find(';');
        if (delimiter_pos != std::string::npos) {
            std::string hash = line.substr(0, delimiter_pos);
            std::string verdict = line.substr(delimiter_pos + 1);
            hashs_table_[hash] = verdict;
        }
    }
    
    file.close();
    return true; 
}