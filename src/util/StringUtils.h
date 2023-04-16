#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <cstddef>
#include <cstring>

namespace StringUtils {

    void copyStringToBuffer(const char* string, char* buffer, size_t bufferSize) {
        size_t stringLength = strlen(string);
        if (stringLength > bufferSize) {
            stringLength = bufferSize;
        }
        strncpy(buffer, string, stringLength);
        buffer[stringLength] = '\0';
    }
}

#endif // STRINGUTILS_H