#include <queue>
#include <mutex>

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mtx;

public:
    // Constructor
    ThreadSafeQueue() = default;

    // Destructor
    ~ThreadSafeQueue() = default;

    // Add an item to the queue
    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mtx);
        queue.push(item);
        lock.unlock();
    }

    // Remove and return the front item from the queue
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        T frontItem = queue.front();
        queue.pop();
        lock.unlock();
        return frontItem;
    }

    // Check if the queue is empty
    bool empty() const {
        return queue.empty();;
    }

    // Get the size of the queue
    int size() const {
        std::unique_lock<std::mutex> lock(mtx);
        int size = queue.size();
        lock.unlock();
        return size;
    }
};