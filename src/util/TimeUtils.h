#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <chrono>

namespace TimeUtils {

    inline std::chrono::milliseconds getCurrentTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    }

}

#endif //TIMEUTILS_H