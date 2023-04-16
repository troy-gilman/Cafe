#pragma once 

#include <random>
#include <chrono>
#include <cstdint>

class UUIDGenerator {
private:
    std::random_device rd;
    std::mt19937_64 mt;
    std::uniform_int_distribution<uint64_t> dist;

public:
    UUIDGenerator() : rd(), mt(rd()), dist(1, UINT64_MAX) {
        // Constructor with member initialization syntax
    }

    uint64_t generateUUID() {
        // Get the current system time since epoch in milliseconds
        auto now = std::chrono::system_clock::now();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        // Combine the current system time with the random number to generate UUID
        uint64_t uuid = (millis << 16) | (dist(mt) & 0xFFFF);

        return uuid;
    }
};