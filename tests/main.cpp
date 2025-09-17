#include <gtest/gtest.h>
#include "scanner/scanner.hpp"
#include <filesystem>
#include <fstream>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>
#include <regex>

namespace fs = std::filesystem;

std::string calculateMD5(const std::string& content) {
    MD5_CTX md5_context;
    MD5_Init(&md5_context);
    MD5_Update(&md5_context, content.c_str(), content.size());
    
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_Final(hash, &md5_context);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    
    return ss.str();
}

class ScannerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir_ = "test_dir";
        fs::create_directories(test_dir_);
        
        std::ofstream file1(test_dir_ + "/file1.txt");
        file1 << "Hello CGSG";
        file1.close();
        
        std::ofstream file2(test_dir_ + "/file2.txt");
        file2 << "30 47 102";
        file2.close();
        
        fs::create_directories(test_dir_ + "/subdir");
        std::ofstream file3(test_dir_ + "/subdir/file3.txt");
        file3 << "CGSG FOREVER!!!";
        file3.close();
        
        hash1_ = calculateMD5("Hello CGSG");
        
        std::ofstream csv_file("test_base.csv");
        csv_file << hash1_ << ";Test\n";
        csv_file << "d077f244def8a70e5ea758bd8352fcd8;Another\n";
        csv_file.close();
        
        log_file_ = "test_scanner.log";
    }
    
    void TearDown() override {
        if (fs::exists(test_dir_)) {
            fs::remove_all(test_dir_);
        }
        if (fs::exists("test_base.csv")) {
            fs::remove("test_base.csv");
        }
        if (fs::exists(log_file_)) {
            fs::remove(log_file_);
        }
    }
    
    std::string test_dir_;
    std::string log_file_;
    std::string hash1_;
};

TEST_F(ScannerTest, NonExistentBaseFile) {
    Scanner scanner("nonexistent.csv", test_dir_, log_file_);
    
    EXPECT_THROW(scanner.run(), std::runtime_error);
}

TEST_F(ScannerTest, NonExistentScanDirectory) {
    Scanner scanner("test_base.csv", "nonexistent_dir", log_file_);
    
    EXPECT_THROW(scanner.run(), fs::filesystem_error);
}

TEST_F(ScannerTest, LogContent) {
    Scanner scanner("test_base.csv", test_dir_, log_file_);
    scanner.run();
    
    std::ifstream log_stream(log_file_);
    std::string line;
    std::string content;
    int line_count = 0;
    
    while (std::getline(log_stream, line)) {
        content += line + "\n";
        line_count++;
    }
    
    EXPECT_TRUE(content.find("file1.txt") != std::string::npos);
    EXPECT_TRUE(content.find("file2.txt") != std::string::npos);
    EXPECT_TRUE(content.find("subdir/file3.txt") != std::string::npos);
    
    EXPECT_TRUE(content.find(hash1_) != std::string::npos);
    EXPECT_TRUE(content.find("Test") != std::string::npos);
    
    EXPECT_EQ(line_count, 3);
}

TEST(ScannerEmptyTest, EmptyDirectory) {
    std::string empty_dir = "empty_dir";
    fs::create_directories(empty_dir);
    
    std::ofstream csv_file("empty_base.csv");
    csv_file << "5d41402abc4b2a76b9719d911017c592;TestMalware\n";
    csv_file.close();
    
    std::string log_file = "empty_test.log";
    
    Scanner scanner("empty_base.csv", empty_dir, log_file);
    
    EXPECT_NO_THROW(scanner.run());
    
    EXPECT_TRUE(fs::exists(log_file));
    
    std::ifstream log_stream(log_file);
    std::string line;
    int line_count = 0;
    
    while (std::getline(log_stream, line)) {
        line_count++;
    }
    
    EXPECT_EQ(line_count, 0);
    
    fs::remove_all(empty_dir);
    fs::remove("empty_base.csv");
    fs::remove(log_file);
}

TEST_F(ScannerTest, ConsoleOutput) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    Scanner scanner("test_base.csv", test_dir_, log_file_);
    scanner.run();
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    
    std::cout << "Actual output: " << output << std::endl;
    
    EXPECT_TRUE(output.find("files") != std::string::npos || 
                output.find("completed") != std::string::npos ||
                output.find("ms") != std::string::npos);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}