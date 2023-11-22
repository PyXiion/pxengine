// Copyright (c) 2023.

//
// Created by pyxiion on 03.11.23.
//

#ifndef ENGINE_TASK_QUEUE_HPP
#define ENGINE_TASK_QUEUE_HPP
#include <mutex>
#include <queue>
#include <condition_variable>

namespace px {

  /// The queue for tasks.
  /// \tparam T The type of tasks.
  template <class T>
  class TaskQueue {
  public:
    explicit TaskQueue(std::size_t maxSize = 0)
      : m_maxSize(maxSize) {}
    ~TaskQueue() = default;

    /// Get the max size of queue.
    /// \return The max size.
    std::size_t maxSize() const {
      std::lock_guard lk(m_mutex);
      return m_maxSize;
    }

    /// Set the max size of the queue.
    /// \param newSize The new max size.
    void setMaxSize(std::size_t newSize) {
      std::lock_guard lk(m_mutex);
      m_maxSize = newSize;
    }

    /// \return True if the queue is empty, false otherwise.
    bool empty() const {
      std::lock_guard lk(m_mutex);
      return m_queue.empty();
    }

    /// \return True if there are maxSize items in the queue. If the maxSize = 0 (by default), then never returns true.
    bool full() const {
      std::lock_guard lk(m_mutex);
      if (m_maxSize == 0)
        return false;
      return m_queue.size() >= m_maxSize;
    }

    /// Remove and return an item from the queue. If the queue is empty, wait until an item available.
    T get() {
      std::unique_lock lk(m_mutex);
      // Check for a value
      if (not m_queue.empty()) {
        return pop();
      }
      // If it is not present, wait
      m_tasksCv.wait(lk, [this]{ return not m_queue.empty(); });
      return pop();
    }

    /// Remove and return an item from the queue.
    T tryGet() {
      std::lock_guard lk(m_mutex);
      return pop();
    }

    /// Block until all items in the queue have been received and processed.
    /// The count of unfinished tasks goes up whenever an item is added to the queue. The count goes down
    /// whenever a consumer calls taskDone() to indicate that the item was retrieved and all work
    /// on it is complete. When the count of unfinished tasks drops to zero, it unblocks.
    void join() {
      std::unique_lock lk(m_mutex);
      // Wait if the tasks are not completed.
      if (m_tasksCount >= 0) {
        m_tasksCv.wait(lk, [this] { return m_tasksCount == 0; });
      }
    }

    /// Put an item into the queue. If the queue is full, wait until a free slot is available before adding the item.
    /// \param item
    void put(T &&item) {
      std::unique_lock lk(m_mutex);
      // Wait for a free slot if the queue is full
      if (m_maxSize > 0 && m_queue.size() >= m_maxSize) {
        m_tasksCv.wait(lk, [this] { return m_queue.size() < m_maxSize; });
      }
      m_queue.push(std::move(item));

      m_tasksCount++;
      m_tasksCv.notify_one();
    }

    /// \return The number of items in the queue.
    std::size_t size() const {
      std::lock_guard lk(m_mutex);
      return m_queue.size();
    }

    /// Indicate that a formerly enqueued task is complete.
    /// Used by queue consumers. For each get() used to fetch a task, a subsequent call to taskDone() tells the queue that the processing on the task is complete.
    void taskDone() {
      std::unique_lock lk(m_mutex);
      if (m_tasksCount == 0) {
        return;
      }
      m_tasksCount -= 1;
      m_tasksCv.notify_one();
    }

  private:
    std::size_t m_maxSize;
    std::size_t m_tasksCount{};
    std::queue<T> m_queue;

    std::condition_variable m_tasksCv;
    mutable std::mutex m_mutex;

    T pop() {
      T value = std::move(m_queue.front());
      m_queue.pop();
      return value;
    }
  };

} // px

#endif //ENGINE_TASK_QUEUE_HPP
