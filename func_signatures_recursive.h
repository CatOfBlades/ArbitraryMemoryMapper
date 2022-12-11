#ifndef FUNC_SIGNATURES_RECURSIVE_H
#define FUNC_SIGNATURES_RECURSIVE_H

#include <fstream>
#include <string>
#include <vector>
#include <regex>

// The function signature structure
struct CPP_Sig {
    std::string return_type;
    std::string name;
    std::string args;
};

std::vector<CPP_Sig> func_signatures_recursive(const std::string& filename);

#endif // FUNC_SIGNATURES_RECURSIVE_H
