#ifndef PX_EVENT_HPP
#define PX_EVENT_HPP
#include <memory>

namespace px
{
  typedef int EventType;

  class EventManager;
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

  using EventPtr = std::unique_ptr<Event>;

  template<typename T>
  concept EventBasedType = std::is_base_of_v<Event, T>;

  template<EventBasedType T>
  using CustomEventPtr = std::unique_ptr<T>;
}

inline px::EventType px::Event::getType() const
{
  return m_type;
}

#include <px/templates.hpp>
#include "event_fabric.hpp"

namespace px {
  template<typename T, string_literal ID>
  class BaseEvent : public Event {
  public:
    inline static EventFabric<T> fabric;

    constexpr inline static std::string_view eventId = static_cast<std::string_view>(ID.value);
  };
}

#endif // PX_EVENT_HPP