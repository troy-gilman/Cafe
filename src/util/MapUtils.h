#ifndef MAPUTILS_H
#define MAPUTILS_H

#include <unordered_map>

namespace MapUtils {

    template<typename T, typename Q>
    inline Q getValueOrDefault(std::unordered_map<T, Q> map, T key, Q defaultValue) {
        auto iter = map.find(key);
        if (iter == map.end()) {
            return defaultValue;
        }
        return iter->second;
    }

    template<typename T, typename Q>
    inline Q* getValueOrNullPtr(std::unordered_map<T, Q*> map, T key) {
        auto iter = map.find(key);
        if (iter == map.end()) {
            return nullptr;
        }
        return iter->second;
    }



}


#endif //MAPUTILS_H