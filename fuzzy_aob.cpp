
#include "fuzzy_aob.h"

//note a "kernel" is an array of "fuzzybytes" which is just an array of bytes where each byte can be at most "range" away from the "byte" value.

// Returns the candidates that match the kernel in the memory space
std::vector<void*> get_candidate_structs_from_fuzzy_AOB(void* start_address, size_t memory_space_size, const std::vector<fuzzybyte>& kernel) {
  // Create a vector to hold the candidates
  std::vector<void*> candidates;

  // Create a lambda function that acts as the fuzzy predicate
  // It takes two bytes (one from the memory space and one from the kernel)
  // and returns true if the byte from the memory space is within the range of
  // the byte from the kernel, false otherwise
  auto fuzzy_predicate = [](uint8_t memory_byte, uint8_t kernel_byte, double range) {
    return std::abs(memory_byte - kernel_byte) <= range;
  };

  // Iterate over the memory space and search for the kernel using the fuzzy predicate
  for (size_t i = 0; i < memory_space_size - kernel.size(); i++) {
    // Create a pointer to the current location in the memory space
    void* current_address = static_cast<void*>(static_cast<uint8_t*>(start_address) + i);

    // Check if the kernel matches at the current location using the fuzzy predicate
    bool match = true;
    for (size_t j = 0; j < kernel.size(); j++) {
      uint8_t memory_byte = *(static_cast<uint8_t*>(current_address) + j);
      uint8_t kernel_byte = kernel[j].byte;
      double range = kernel[j].range;
      if (!fuzzy_predicate(memory_byte, kernel_byte, range)) {
        match = false;
        break;
      }
    }

    // If the kernel matches, add the current location to the candidates vector
    if (match) {
      candidates.push_back(current_address);
    }
  }

  // Return the candidates vector
  return candidates;
}
