#ifndef MEMORYHANDLE_H_INCLUDED
#define MEMORYHANDLE_H_INCLUDED

#include <cstdint>
#include <optional>
#include <vector>

struct MemoryHandleEntry {
    unsigned long int address;  // Offset in virtual memory space
    unsigned long int size;     // Size in bytes
    uint32_t generation = 0;
    bool active = true;
};

class MemoryHandleTable {
public:
    // Same as before...
    uint64_t createHandle(unsigned long int address, unsigned long int size);

    // New version: returns false if invalid
    bool getHandle(uint64_t rawHandle, MemoryHandleEntry& out) const;

    void destroyHandle(uint64_t rawHandle);

private:
    std::vector<MemoryHandleEntry> entries;
    std::vector<uint32_t> freeList;

    uint64_t composeHandle(uint32_t index, uint32_t generation);
};



#endif // MEMORYHANDLE_H_INCLUDED
