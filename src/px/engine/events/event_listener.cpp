// Copyright (c) 2024.

//
// Created by pyxiion on 14.01.24.
//
#include "event_listener.hpp"

namespace px {
  EventListener::~EventListener() {
    unsubscribeAll();
  }

  void EventListener::unsubscribeAll() {
    for (const auto &f : m_destructors)
      f();
    m_destructors.clear();
  }

  void EventListener::appendDestructor(std::function<void()> &&f) {
    m_destructors.push_back(std::move(f));
  }
}
