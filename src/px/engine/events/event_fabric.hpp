#ifndef PX_EVENT_FABRIC_HPP
#define PX_EVENT_FABRIC_HPP

#include "event.hpp"

namespace px
{
  /**
   * @brief Фабрика для упрощенного создания экземпляров событий.
   * @tparam TEvent Класс-наследник события
   */
  template<class TEvent>
  class EventFabric
  {
  public:
    inline static const EventType kUndefinedEventType = INT_MIN;

  public:
    EventFabric()
      : m_eventType(kUndefinedEventType) {}
    explicit EventFabric(EventType eventType)
      : m_eventType(eventType) {}

    /**
     * @brief Установить внутренний числовой ID события для его последующего создания.
     * @param newEventType Внутренний числовой ID события.
     */
    void setEventType(EventType newEventType);

    /// @brief Создать экземпляр класса события.
    /// @param ...args Аргументы, передаваемые в конструктор класса события.
    /// @return Указатель на экземпляр класса.
    /// @throw std::runtime_error Если не установлен ID события.
    template<class ...TArgs>
    EventPtr make(TArgs ...args);

  private:
    EventType m_eventType;
  };
}

template <class TEvent>
inline void px::EventFabric<TEvent>::setEventType(px::EventType newEventType)
{
  m_eventType = newEventType;
}

template<class TEvent>
template<class ...TArgs>
inline px::EventPtr px::EventFabric<TEvent>::make(TArgs ...args)
{
  if (m_eventType == kUndefinedEventType)
    throw std::runtime_error("Внутренний числовой ID события не назначен к фабрике событий. Может вы забыли EventFabric::setEventType(eventType)?");
  EventPtr eventPtr = std::make_shared<TEvent>(args...);
  eventPtr->m_type = m_eventType;

  return std::move(eventPtr);
}

#endif // PX_EVENT_FABRIC_HPP