#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    /// Конструктор: по умолчанию создаёт 6 потоков.
    explicit ThreadPool(size_t numThreads = 6);
    
    /// Деструктор: корректно завершает все потоки.
    ~ThreadPool();

    /**
     * Добавление задачи в пул.
     * Реализовано как шаблонная функция – её реализация находится в заголовке.
     */
    template <class F, class... Args>
    inline auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using return_type = typename std::invoke_result<F, Args...>::type;

        // Оборачиваем функцию в packaged_task
        auto taskPtr = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = taskPtr->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([taskPtr]() { (*taskPtr)(); });
        }
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers;             // рабочие потоки
    std::queue<std::function<void()>> tasks;        // очередь задач

    std::mutex queueMutex;                          // мьютекс для синхронизации доступа к очереди
    std::condition_variable condition;              // условная переменная для уведомления потоков
    bool stop;                                      // флаг остановки пула
};

ThreadPool::ThreadPool(size_t numThreads) 
    : stop(false)
{
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    // Ожидаем появления задачи или сигнала остановки
                    condition.wait(lock, [this] {
                        return stop || !tasks.empty();
                    });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (auto &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

