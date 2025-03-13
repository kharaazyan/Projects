#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <cassert>
#include <chrono>
#include <exception>

class ThreadPool {
private:
    std::vector<std::thread> workers;               
    std::queue<std::function<void()>> tasks;       
    mutable std::mutex queueMutex;                  
    std::condition_variable condition;        
    std::atomic<bool> stopFlag{false};           
    std::atomic<size_t> activeThreadsCount{0};     

    void start(size_t numThreads);
    void stop();

public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>;

    size_t activeThreads() const;
    size_t taskQueueSize() const;

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&& other) noexcept;
    ThreadPool& operator=(ThreadPool&& other) noexcept;
};


ThreadPool::ThreadPool(size_t numThreads) {
    if (numThreads == 0) numThreads = 2; 
    start(numThreads);
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::start(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] { return this->stopFlag || !this->tasks.empty(); });
                    
                    if (this->stopFlag && this->tasks.empty())
                        return;
                    
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                activeThreadsCount.fetch_add(1, std::memory_order_relaxed);
                task(); 
                activeThreadsCount.fetch_sub(1, std::memory_order_relaxed);
            }
        });
    }
}

void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (stopFlag) return; 
        stopFlag = true;
    }
    condition.notify_all();

    for (auto& thread : workers) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workers.clear();
}

ThreadPool::ThreadPool(ThreadPool&& other) noexcept
    : workers(std::move(other.workers)),
      stopFlag(other.stopFlag.load()) {}

ThreadPool& ThreadPool::operator=(ThreadPool&& other) noexcept {
    if (this != &other) {
        stop();
        workers = std::move(other.workers);
        stopFlag.store(other.stopFlag.load());
    }
    return *this;
}

template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type> {
    
    using return_type = typename std::invoke_result<F, Args...>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (stopFlag.load(std::memory_order_acquire))
            throw std::runtime_error("ThreadPool has been stopped");
        tasks.push([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

size_t ThreadPool::activeThreads() const {
    return activeThreadsCount.load(std::memory_order_relaxed);
}

size_t ThreadPool::taskQueueSize() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return tasks.size();
}

#endif