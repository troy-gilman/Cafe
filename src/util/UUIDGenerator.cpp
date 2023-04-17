#include "UUIDGenerator.h"

UUIDGenerator* UUIDGenerator::instance = nullptr;

UUIDGenerator* UUIDGenerator::getInstance() {
    if (instance == nullptr) {
        instance = new UUIDGenerator();
    }
    return instance;
}

uint64_t UUIDGenerator::generateUUID() {
    // Get the current system time since epoch in milliseconds
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // Combine the current system time with the random number to generate UUID
    uint64_t uuid = (millis << 16) | (dist(mt) & 0xFFFF);

    return uuid;
}