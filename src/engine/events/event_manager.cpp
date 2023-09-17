#include "event_manager.hpp"

int px::EventManager::EventppPolicy::getEvent(const EventPtr &event)
{
  return event->getType();
}

px::EventType px::EventManager::registerNewEventType(const std::string &eventStrId)
{
  return m_enum.add(eventStrId);
}

px::EventType px::EventManager::getOrRegisterEventType(const std::string &eventStrId)
{
  return m_enum.getOrCreate(eventStrId);
}

px::EventType px::EventManager::getEventId(const std::string &eventStrId) const
{
  return m_enum.get(eventStrId);
}

std::string px::EventManager::getEventId(px::EventType type) const
{
  return m_enum.get(type);
}

px::EventManager::Handle px::EventManager::appendListener(px::EventType type, px::EventCallback callback)
{
  return m_queue.appendListener(type, callback);
}

px::EventManager::Handle px::EventManager::appendListener(const std::string &type, EventCallback callback)
{
  return appendListener(getEventId(type), callback);
}

void px::EventManager::enqueueEventPtr(std::shared_ptr<px::Event> &&event)
{
  m_queue.enqueue(std::move(event));
}

bool px::EventManager::process()
{
  return m_queue.process();
}
