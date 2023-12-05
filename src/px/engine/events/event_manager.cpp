#include "event_manager.hpp"
#include "event_listener.hpp"

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

px::EventManager::Handle px::EventManager::appendListener(px::EventType type, const px::DefaultEventCallback& callback)
{
  std::lock_guard lk(m_queueMutex);
  return m_queue.appendListener(type, callback);
}

px::EventManager::Handle px::EventManager::appendListener(const std::string &type, const DefaultEventCallback& callback)
{
  std::lock_guard lk(m_queueMutex);
  return appendListener(getEventId(type), callback);
}

void px::EventManager::removeListener(px::EventType type, const EventQueue::Handle &handle) {
  std::lock_guard lk(m_queueMutex);
  m_queue.removeListener(type, handle);
}

void px::EventManager::enqueueEvent(px::EventPtr &&event)
{
  std::lock_guard lk(m_queueMutex);
  m_queue.enqueue(std::move(event));
}

bool px::EventManager::process()
{
  std::lock_guard lk(m_queueMutex);
  return m_queue.process();
}

void px::EventManager::listen(px::EventListener *listener, px::EventType type, const px::DefaultEventCallback &callback) {
  if (not listener) return;

  auto handle = appendListener(type, callback);

  std::weak_ptr<bool> wptr = m_existence;
  listener->appendDestructor([this, type, handle, wptr = std::move(wptr)] {
    // Check is this still exists
    if (not wptr.expired()) {
      this->removeListener(type, handle);
    }
  });
}

void px::EventManager::listen(px::EventListener *listener, const std::string &strType,
                              const px::DefaultEventCallback &callback) {
  if (not listener) return;
  EventType type = getEventId(strType);
  listen(listener, type, callback);
}

px::EventManager::EventManager()
  : m_existence(std::make_shared<bool>(false)){

}

px::EventManager::~EventManager() {
}

std::vector<std::pair<std::string, px::EventType>> px::EventManager::getEventIds() const {
  std::lock_guard lk(m_enumMutex);
  return m_enum.getPairs();
}
