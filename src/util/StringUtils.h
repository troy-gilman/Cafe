#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <cstddef>
#include <cstring>
#include <cstdio>

namespace StringUtils {

    inline void copyStringToBuffer(const char* string, char* buffer, size_t bufferSize) {
        size_t stringLength = strlen(string);
        if (stringLength > bufferSize) {
            stringLength = bufferSize;
        }
        strncpy(buffer, string, stringLength);
        buffer[stringLength] = '\0';
    }

    inline const char* loadFileAsString(const char* filePath) {
        FILE* file = fopen(filePath, "rt");
        if (file == nullptr) {
            return nullptr;
        }

        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        rewind(file);

        char* fileContents = new char[fileSize + 1];
        fread(fileContents, 1, fileSize, file);
        fclose(file);

        fileContents[fileSize] = '\0';
        return fileContents;
    }
}

#endif // STRINGUTILS_H