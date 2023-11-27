#ifndef PX_EVENT_HPP
#define PX_EVENT_HPP
#include <memory>

#define PX_EVENT_DECLARATION(classname, eventStrId)          \
private:                                                  \
  friend class ::px::EventManager;                        \
  inline static ::px::EventFabric<classname> fabric;      \
public:                                                   \
  inline static constexpr std::string_view eventId = eventStrId;

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

  typedef std::shared_ptr<Event> EventPtr;

  template<typename T>
  concept EventBasedType = std::is_base_of_v<Event, T>;
}

inline px::EventType px::Event::getType() const
{
  return m_type;
}

#endif // PX_EVENT_HPP