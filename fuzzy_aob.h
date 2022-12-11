
#ifndef FUZZY_AOB_H_INCLUDED
#define FUZZY_AOB_H_INCLUDED

#include<cstdint>
#include<vector>

// Fuzzy byte struct
struct fuzzybyte {
  uint8_t byte;
  double range;
};

/**!
You can use the get_candidates function like this:
    // Create the kernel as a vector of fuzzybyte objects
    std::vector<fuzzybyte> kernel = {{0x00, 0.1}, {0x01, 0.2}, {0x02, 0.3}};

    // Get the candidates in the memory space
    std::vector<void*> candidates = get_candidates(start_address, memory_space_size, kernel);
!**/
//! Returns the candidates that match the kernel in the memory space
std::vector<void*> get_candidates(void* start_address, size_t memory_space_size, const std::vector<fuzzybyte>& kernel);




#endif // FUZZY_AOB_H_INCLUDED
