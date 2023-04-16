#pragma once

#include "../Event.h"
#include <stack>
#include <mutex>

template <class T>
class ObjectPool {
private:
    T* objects;
    bool* objectsInUse;
    int numObjects;
    int nextObjectIndex;
    int numObjectsInUse;
    std::mutex mtx;

public:
    ObjectPool(int numObjects) {
        this->numObjects = numObjects;
        this->objects = new T[numObjects];
        this->objectsInUse = new bool[numObjects];
        this->nextObjectIndex = 0;
    }

    ~ObjectPool() {
        delete[] objects;
        delete[] objectsInUse;
    }

    T* getObject() {
        std::unique_lock<std::mutex> lock(mtx);
        int numTries = 0;
        while (objectsInUse[nextObjectIndex]) {
            nextObjectIndex = (nextObjectIndex + 1) % numObjects;
            numTries++;
            if (numTries > numObjects) {
                return nullptr;
            }
        }
        objectsInUse[nextObjectIndex] = true;
        numObjectsInUse++;
        T* obj = &objects[nextObjectIndex];
        lock.unlock();
        return obj;
    }

    void returnObject(T* obj) {
        std::unique_lock<std::mutex> lock(mtx);
        int index = obj - objects;
        if (index > 0 && index < numObjects) {
            objectsInUse[index] = false;
            numObjectsInUse--;
        }
        lock.unlock();
    }

    int getNumObjectsInUse() {
        return numObjectsInUse;
    }

    T* waitForObject() {
        T* obj = nullptr;
        while (obj == nullptr) {
            obj = getObject();
        }
        return obj;
    }
};