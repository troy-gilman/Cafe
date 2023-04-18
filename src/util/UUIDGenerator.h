#ifndef UUIDGENERATOR_H
#define UUIDGENERATOR_H

#include <random>
#include <chrono>
#include <cstdint>
#include "Types.h"

class UUIDGenerator {
private:
    static UUIDGenerator* instance;

    std::random_device rd;
    std::mt19937_64 mt;
    std::uniform_int_distribution<uint64_t> dist;

    UUIDGenerator() : rd(), mt(rd()), dist(1, UINT64_MAX) {
        // Constructor with member initialization syntax
    }

public:
    static UUIDGenerator* getInstance();
    UUID generateUUID();
};

#endif // UUIDGENERATOR_H