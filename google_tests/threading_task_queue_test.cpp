//
// Created by pyxiion on 03.11.23.
//
#include <gtest/gtest.h>
#include "../src/px/threading/task_queue.hpp"

// Test case for maxSize() function
TEST(TaskQueueTest, MaxSizeTest) {
  px::TaskQueue<int> queue(5);
  ASSERT_EQ(queue.maxSize(), 5);
}

// Test case for setMaxSize() function
TEST(TaskQueueTest, SetMaxSizeTest) {
  px::TaskQueue<int> queue;
  queue.setMaxSize(10);
  ASSERT_EQ(queue.maxSize(), 10);
}

// Test case for empty() function
TEST(TaskQueueTest, EmptyTest) {
  px::TaskQueue<int> queue;
  ASSERT_TRUE(queue.empty());

  queue.put(42);
  ASSERT_FALSE(queue.empty());
}

// Test case for full() function
TEST(TaskQueueTest, FullTest) {
  px::TaskQueue<int> queue(3);
  ASSERT_FALSE(queue.full());

  queue.put(1);
  queue.put(2);
  queue.put(3);
  ASSERT_TRUE(queue.full());
}

// Test case for get() function
TEST(TaskQueueTest, GetTest) {
  px::TaskQueue<int> queue;
  std::thread producer([&queue]() {
    queue.put(42);
  });

  int value = queue.get();
  ASSERT_EQ(value, 42);

  producer.join();
}

// Test case for tryGet() function
TEST(TaskQueueTest, TryGetTest) {
  px::TaskQueue<int> queue;
  queue.put(42);

  int value = queue.tryGet();
  ASSERT_EQ(value, 42);
}

// Test case for join() function
TEST(TaskQueueTest, JoinTest) {
  px::TaskQueue<int> queue;
  std::thread producer([&queue]() {
    queue.put(42);
    queue.taskDone();
  });

  queue.join();

  producer.join();
}

// Test case for put() function
TEST(TaskQueueTest, PutTest) {
  px::TaskQueue<int> queue(3);
  queue.put(1);
  queue.put(2);
  queue.put(3);

  std::thread producer([&queue]() {
    queue.put(4);
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ASSERT_EQ(queue.size(), 3);

  queue.get(); queue.taskDone();

  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ASSERT_EQ(queue.size(), 3);

  producer.join();
}

// Test case for size() function
TEST(TaskQueueTest, SizeTest) {
  px::TaskQueue<int> queue;
  ASSERT_EQ(queue.size(), 0);

  queue.put(1);
  ASSERT_EQ(queue.size(), 1);

  queue.put(2);
  ASSERT_EQ(queue.size(), 2);
}

// Test case for taskDone() function
TEST(TaskQueueTest, TaskDoneTest) {
  px::TaskQueue<int> queue(1);
  std::thread producer([&queue]() {
    queue.put(42);
    queue.put(84);
    queue.put(16);
  });

  int value = queue.get();
  ASSERT_EQ(value, 42);
  ASSERT_EQ(queue.size(), 0);
  queue.taskDone();
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ASSERT_EQ(queue.size(), 1);

  value = queue.get();
  ASSERT_EQ(value, 84);
  ASSERT_EQ(queue.size(), 0);
  queue.taskDone();
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ASSERT_EQ(queue.size(), 1);

  value = queue.get();
  ASSERT_EQ(value, 16);
  ASSERT_EQ(queue.size(), 0);
  queue.taskDone();
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ASSERT_EQ(queue.size(), 0);

  queue.taskDone();
  producer.join();
}