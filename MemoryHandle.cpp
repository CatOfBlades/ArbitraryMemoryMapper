#include "MemoryHandle.h"


uint64_t MemoryHandleTable::createHandle(unsigned long int address, unsigned long int size) {
    uint32_t index;
    if (!freeList.empty()) {
        index = freeList.back();
        freeList.pop_back();
        entries[index] = {address, size, entries[index].generation, true};
    } else {
        index = entries.size();
        entries.push_back({address, size, 0, true});
    }

    return composeHandle(index, entries[index].generation);
}

bool MemoryHandleTable::getHandle(uint64_t rawHandle, MemoryHandleEntry& out) const {
    uint32_t index = uint32_t(rawHandle & 0xFFFFFFFF);
    uint32_t gen   = uint32_t(rawHandle >> 32);

    if (index >= entries.size()) return false;

    const auto& entry = entries[index];
    if (!entry.active || entry.generation != gen) return false;

    out = entry;
    return true;
}

void MemoryHandleTable::destroyHandle(uint64_t rawHandle) {
    uint32_t index = uint32_t(rawHandle & 0xFFFFFFFF);
    uint32_t gen   = uint32_t(rawHandle >> 32);

    if (index >= entries.size()) return;

    auto& entry = entries[index];
    if (!entry.active || entry.generation != gen) return;

    entry.active = false;
    entry.generation++;
    freeList.push_back(index);
}

uint64_t MemoryHandleTable::composeHandle(uint32_t index, uint32_t generation) {
    return (uint64_t(generation) << 32) | index;
}


