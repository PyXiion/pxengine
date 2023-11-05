#include "event_manager.hpp"

int px::EventManager::EventppPolicy::getEvent(const EventPtr &event)
{
  return event->getType();
}

px::EventType px::EventManager::registerNewEventType(const std::string &eventStrId)
{
  std::lock_guard lk(m_enumMutex);
  return m_enum.add(eventStrId);
}

px::EventType px::EventManager::getOrRegisterEventType(const std::string &eventStrId)
{
  std::lock_guard lk(m_enumMutex);
  return m_enum.getOrCreate(eventStrId);
}

px::EventType px::EventManager::getEventId(const std::string &eventStrId) const
{
  std::lock_guard lk(m_enumMutex);
  return m_enum.get(eventStrId);
}

std::string px::EventManager::getEventId(px::EventType type) const
{
  std::lock_guard lk(m_enumMutex);
  return m_enum.get(type);
}

px::EventManager::Handle px::EventManager::appendListener(px::EventType type, const px::EventCallback& callback)
{
  std::lock_guard lk(m_queueMutex);
  return m_queue.appendListener(type, callback);
}

px::EventManager::Handle px::EventManager::appendListener(const std::string &type, const EventCallback& callback)
{
  std::lock_guard lk(m_queueMutex);
  return appendListener(getEventId(type), callback);
}

void px::EventManager::enqueueEventPtr(std::shared_ptr<px::Event> &&event)
{
  std::lock_guard lk(m_queueMutex);
  m_queue.enqueue(std::move(event));
}

bool px::EventManager::process()
{
  std::lock_guard lk(m_queueMutex);
  return m_queue.process();
}
