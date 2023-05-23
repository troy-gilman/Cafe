#ifndef ARRAYUTILS_H
#define ARRAYUTILS_H

namespace ArrayUtils {

    template <typename T>
    inline void  shiftArrayRight(T* array, int numElements, int startIndex) {
        for (int i = numElements - 1; i >= startIndex; i--) {
            array[i] = array[i - 1];
        }
    }

}

#endif // ARRAYUTILS_H