//
// Created by aichao on 2025/9/30.
//

#pragma once


#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() { shutdown(); }

    // 禁用拷贝
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(T&& item) {
        std::shared_ptr<T> new_item = std::make_shared<T>(std::move(item));
        std::lock_guard<std::mutex> lock(mutex_);
        if (shutdown_) return;
        queue_.push(new_item);
        condition_.notify_one();
    }

    bool try_pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty() || shutdown_) {
            return false;
        }
        item = std::move(*queue_.front());
        queue_.pop();
        return true;
    }

    bool wait_and_pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty() || shutdown_; });
        if (shutdown_ || queue_.empty()) {
            return false;
        }
        item = std::move(*queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_ = true;
        }
        condition_.notify_all();
    }

    bool is_shutdown() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return shutdown_;
    }

private:
    mutable std::mutex mutex_;
    std::queue<std::shared_ptr<T>> queue_;
    std::condition_variable condition_;
    bool shutdown_ = false;
};
