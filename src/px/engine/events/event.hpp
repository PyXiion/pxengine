#pragma once
#include <memory>

namespace px
{
  typedef int EventType;

  class Event
  {
    template<class TEvent>
    friend class EventFabric;
  public:
    virtual ~Event() = default;

    [[nodiscard]] EventType getType() const;
  private:
    EventType m_type{};
  };

  typedef std::shared_ptr<Event> EventPtr;

}

inline px::EventType px::Event::getType() const
{
  return m_type;
}