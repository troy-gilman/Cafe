#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <cstddef>
#include <cstring>
#include <cstdio>

namespace StringUtils {

    inline void copyStringToBuffer(const char* string, char* buffer, ui64 bufferSize) {
        ui64 stringLength = strlen(string);
        if (stringLength > bufferSize) {
            stringLength = bufferSize;
        }
        strncpy(buffer, string, stringLength);
        buffer[stringLength] = '\0';
    }

    inline bool startsWith(const char* string, const char* prefix) {
        ui64 stringLength = strlen(string);
        ui64 prefixLength = strlen(prefix);
        if (stringLength < prefixLength) {
            return false;
        }
        return strncmp(string, prefix, prefixLength) == 0;
    }

    inline const char* loadFileAsString(const char* filePath) {
        FILE* file = fopen(filePath, "rt");
        if (file == nullptr) {
            return nullptr;
        }

        fseek(file, 0, SEEK_END);
        ui64 fileSize = ftell(file);
        rewind(file);

        char* fileContents = new char[fileSize + 1];
        fread(fileContents, 1, fileSize, file);
        fclose(file);

        fileContents[fileSize] = '\0';
        return fileContents;
    }
}

#endif // STRINGUTILS_H