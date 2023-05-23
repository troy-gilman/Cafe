#ifndef UUIDGENERATOR_H
#define UUIDGENERATOR_H

#include <random>
#include <chrono>
#include <cstdint>
#include "../util/Types.h"

class UUIDGenerator {
private:
    static UUIDGenerator* instance;

    std::random_device rd;
    std::mt19937_64 mt;
    std::uniform_int_distribution<int32_t> dist;

    UUIDGenerator() : rd(), mt(rd()), dist(1, INT32_MAX) {
        // Constructor with member initialization syntax
    }

public:
    static UUIDGenerator* getInstance();
    UUID generateUUID();
};

#endif // UUIDGENERATOR_H