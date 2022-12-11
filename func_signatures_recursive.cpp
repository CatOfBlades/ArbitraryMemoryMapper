#include "func_signatures_recursive.h"

// The function for extracting function signatures recursively
std::vector<CPP_Sig> func_signatures_recursive(const std::string& filename) {
    std::regex pattern("^([^\\(]*)\\(([^\\)]*)\\)\\s*\\{");
    std::vector<CPP_Sig> signatures;
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Handle the error
    }
    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_match(line, pattern)) {
            std::smatch matches;
            std::regex_search(line, matches, pattern);
            signatures.emplace_back(CPP_Sig{ matches[1], matches[2] });
        }
        else if (line.find("#include") == 0) {
            std::string include = line.substr(8);
            include = std::regex_replace(include, std::regex("^\\s*"), "");
            include = std::regex_replace(include, std::regex("\\s*$"), "");
            std::vector<CPP_Sig> included_signatures = func_signatures_recursive(include);
            signatures.insert(signatures.end(), included_signatures.begin(), included_signatures.end());
        }
    }
    file.close();
    return signatures;
}
