#include <iostream>
#include <string>
#include "scanner/scanner.hpp"

void printUsage() {
    std::cout << "Usage: scanner.exe --base base.csv --log report.log --path c:\\folder" << std::endl;
}

void parseArguments(int argc, char* argv[], std::string& base, std::string& log, std::string& path) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--base" && i + 1 < argc) {
            base = argv[++i];
        } else if (arg == "--log" && i + 1 < argc) {
            log = argv[++i];
        } else if (arg == "--path" && i + 1 < argc) {
            path = argv[++i];
        } else if (arg == "--help") {
            printUsage();
            exit(0);
        }
    }
}

int main(int argc, char* argv[]) {
    std::string base, log, path;
    
    parseArguments(argc, argv, base, log, path);

    if (base.empty() || log.empty() || path.empty()) {
        std::cerr << "Error: not enough arguments" << std::endl;
        printUsage();
        return 1;
    }

    try {
        Scanner scanner(base, path, log);
        scanner.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}