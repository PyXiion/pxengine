#pragma once
#include <eventpp/eventqueue.h>
#include "../common/dynamic_enum.hpp"
#include "event.hpp"

namespace px
{
  using EventCallbackSign = void(const EventPtr &);
  using EventCallback = std::function<EventCallbackSign>;

  /**
   * @brief Класс для управления событиями.
   */
  class EventManager
  {
  private:
    struct EventppPolicy {
      static EventType getEvent(const EventPtr &event);
    };

    using EventQueue = eventpp::EventQueue<EventType, EventCallbackSign, EventppPolicy>;
    using Handle = EventQueue::Handle;

  public:
    /**
     * @brief Стандартный конструктор.
     */
    EventManager() = default;
    ~EventManager() = default;

    /**
     * @brief Регистрирует новый тип события и возвращает его внутренний числовой ID.
     * @param newEventStrId Строковой ID события.
     * @return Внутренний числовой ID события. 
     */
    EventType registerNewEventType(const std::string &newEventStrId);
    
    /**
     * @brief Возвращает внутренний числовой ID события по его строковому ID.
     * @param newEventStrId Строковой ID события.
     * @return Внутренний числовой ID события. 
     * @throw std::out_of_range если строковой ID не существует.
     */
    EventType getEventId(const std::string &eventStrId) const;

    /**
     * @brief Возвращает строковой ID события по его внутреннему числовому ID.
     * @param type Внутренний числовой ID события.
     * @return Строковой ID события.
     * @throw std::out_of_range если строковой ID не существует.
     */
    std::string getEventId(EventType type) const;

    /**
     * @brief Добавляет прослушиватель событий. Является более быстрым вариантом этого метода, так как не требуется поиск строкового ID.
     * @param type Внутренний числовой ID события.
     * @param callback Функция-прослушиватель.
     * @return Дескриптор прослушивателя.
     */
    Handle appendListener(EventType type, EventCallback callback);

    /**
     * @brief Добавляет прослушиватель событий.
     * @param type Строковой ID события.
     * @param callback Функция-прослушиватель.
     * @return Дескриптор прослушивателя.
     * @throw std::out_of_range если строковой ID не существует.
     */
    Handle appendListener(const std::string &type, EventCallback callback);

    /**
     * @brief Добавляет событие в очередь.
     * @param event Указатель на событие.
     */
    void enqueueEventPtr(std::shared_ptr<px::Event> &&event);

    /**
     * @brief Обработка всех событий и их прослушивателей.
     * @return true если хоть одно событие было обработано, иначе false.
     */
    bool process();

  private:
    EventQueue m_queue;
    DynamicEnum m_enum;

  };
}