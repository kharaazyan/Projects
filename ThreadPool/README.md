```markdown
# 🚀 ThreadPool - High-Performance C++ Thread Pool

A robust and efficient thread pool implementation in modern C++ designed to handle concurrent tasks with minimal overhead and maximum performance. This project leverages C++11 features and beyond to provide a flexible, safe, and high-throughput solution for task execution.

---

## 🔥 Key Features

- **High Performance**: Uses a fixed-size pool of threads to execute tasks concurrently, reducing context switching and synchronization overhead.
- **Thread-Safe Task Queue**: Employs `std::mutex` and `std::condition_variable` for safe and efficient task management.
- **Asynchronous Task Execution**: Returns `std::future<T>` for each enqueued task, enabling seamless integration with asynchronous workflows.
- **RAII Compliance & Graceful Shutdown**: Automatically cleans up resources ensuring all threads are properly joined during destruction.
- **Move Semantics Support**: Implements move constructors and move assignment for efficient resource transfer.
- **Exception Safety**: Provides mechanisms to handle exceptions thrown by tasks without compromising thread stability.

---

## 🚀 Getting Started

### Prerequisites

- **C++ Compiler**: Requires a compiler that supports C++11 or later.
- **Platform**: Cross-platform support (tested on Linux, macOS, and Windows).

### Installation

Clone the repository and include `ThreadPool.hpp` in your project:

```bash
git clone https://github.com/kharaazyan/Project/ThreadPool.git
```

### Usage Example

```cpp
#include <iostream>
#include "ThreadPool.hpp"

int main() {
    // Create a thread pool with 4 worker threads
    ThreadPool pool(4);

    // Enqueue a simple task that returns a string
    auto futureMessage = pool.enqueue([]() {
        return std::string("Hello from ThreadPool!");
    });

    // Enqueue a task that computes the sum of two integers
    auto futureSum = pool.enqueue([](int a, int b) {
        return a + b;
    }, 5, 15);

    // Retrieve and print the results
    std::cout << futureMessage.get() << std::endl;
    std::cout << "Sum: " << futureSum.get() << std::endl;

    return 0;
}
```

---

## 📚 API Documentation

### Constructors & Destructor

- **Constructor**
  ```cpp
  explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
  ```
  - Initializes the thread pool with a specified number of threads (defaults to hardware concurrency).
  - Ensures at least 2 threads if `numThreads` is zero.

- **Destructor**
  ```cpp
  ~ThreadPool();
  ```
  - Ensures a graceful shutdown by stopping all threads and cleaning up resources.

### Task Enqueuing

- **enqueue**
  ```cpp
  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::future<typename std::invoke_result<F, Args...>::type>;
  ```
  - Adds a task to the queue, returns a `std::future` holding the task’s result.
  - Guarantees thread-safety while adding new tasks.

### Thread & Task Status

- **activeThreads**
  ```cpp
  size_t activeThreads() const;
  ```
  - Returns the count of threads actively executing tasks.

- **taskQueueSize**
  ```cpp
  size_t taskQueueSize() const;
  ```
  - Returns the current number of tasks waiting in the queue.

### Copy and Move Semantics

- **Deleted Copy Operations**
  ```cpp
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ```
  - Prevents copying to avoid duplicating thread management resources.

- **Move Operations**
  ```cpp
  ThreadPool(ThreadPool&& other) noexcept;
  ThreadPool& operator=(ThreadPool&& other) noexcept;
  ```
  - Enables efficient resource transfer through move semantics.

---

## ⚙️ Implementation Details

- **Task Management**: Uses `std::queue` to maintain a FIFO list of tasks.
- **Thread Synchronization**: Leverages `std::mutex` for exclusive access and `std::condition_variable` for efficient thread wake-ups.
- **Concurrency Control**: Atomic flags (`std::atomic<bool>`) manage pool shutdown, while `std::atomic<size_t>` tracks active threads.
- **Worker Lifecycle**: Worker threads continuously loop, waiting for tasks and executing them, until a shutdown is signaled.

---

## 📊 Performance Considerations

- **Minimal Lock Contention**: Carefully designed locking strategy minimizes bottlenecks in high-load scenarios.
- **Efficient Resource Use**: Utilizes modern C++ memory management techniques to reduce overhead.
- **Scalability**: Adaptable to varying workloads by adjusting the number of worker threads dynamically based on system capabilities.

---