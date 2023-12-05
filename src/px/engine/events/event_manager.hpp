#pragma once
#include <experimental/memory>
#include <eventpp/eventqueue.h>
#include "../common/dynamic_enum.hpp"
#include "event.hpp"
#include "event_fabric.hpp"

namespace px
{
  using DefaultEventCallbackSign = void(const EventPtr &);
  using DefaultEventCallback = std::function<DefaultEventCallbackSign>;

  template<EventBasedType T>
  using EventCallbackSign = void(const CustomEventPtr<T> &);
  template<EventBasedType T>
  using EventCallback = std::function<EventCallbackSign<T>>;


  class EventListener;

  /**
   * @brief Класс для управления событиями.
   */
  class EventManager
  {
  private:
    struct EventppPolicy {
      static EventType getEvent(const EventPtr &event);
    };

  public:
    using EventQueue = eventpp::EventQueue<EventType, DefaultEventCallbackSign, EventppPolicy>;
    using Handle = EventQueue::Handle;

  public:
    /**
     * @brief Стандартный конструктор.
     */
    EventManager();
    ~EventManager();

    template <EventBasedType T>
    void registerEventClass();

    /**
     * @brief Регистрирует новый тип события и возвращает его внутренний числовой ID.
     * @param newEventStrId Строковой ID события.
     * @return Внутренний числовой ID события. 
     */
    EventType registerNewEventType(const std::string &newEventStrId);

    /**
     * @brief Получить тип события по его строковому ID или создать новый.
     * @param eventStrId Строковой ID события.
     * @return Внутренний числовой ID события. 
    */
    EventType getOrRegisterEventType(const std::string &eventStrId);
    
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
    Handle appendListener(EventType type, const DefaultEventCallback& callback);

    /**
     * @brief Добавляет прослушиватель событий.
     * @param type Строковой ID события.
     * @param callback Функция-прослушиватель.
     * @return Дескриптор прослушивателя.
     * @throw std::out_of_range если строковой ID не существует.
     */
    Handle appendListener(const std::string &type, const DefaultEventCallback& callback);

    void removeListener(px::EventType type, const Handle &handle);

    void listen(EventListener *listener, EventType type, const DefaultEventCallback &callback);
    void listen(EventListener *listener, const std::string &type, const DefaultEventCallback &callback);

    template<EventBasedType T>
    void listen(EventListener *listener, const EventCallback<T> &callback);

    /**
     * @brief Добавляет событие в очередь.
     * @param event Указатель на событие.
     */
    void enqueueEvent(EventPtr &&event);

    template<EventBasedType T, typename ...TArgs>
    void emplaceEvent(TArgs ...args);

    /**
     * @brief Обработка всех событий и их прослушивателей.
     * @return true если хоть одно событие было обработано, иначе false.
     */
    bool process();

    std::vector<std::pair<std::string, EventType>> getEventIds() const;

  private:
    EventQueue m_queue;
    DynamicEnum m_enum;

    mutable std::mutex m_enumMutex;
    mutable std::mutex m_queueMutex;

    std::shared_ptr<bool> m_existence; ///< Existence counter
  };

  typedef EventManager::Handle EventListenerHandle;
}

template<px::EventBasedType T>
void px::EventManager::registerEventClass() {
  static_assert(requires {
    { T::eventId } -> std::convertible_to<std::string_view>;
    { T::fabric } -> std::convertible_to<px::EventFabric<T>>;
  }, "Невозможно зарегистрировать этот тип автоматически. Он не имеет ID и встроенной фабрики. Наследуйте его от класса px::BaseEvent<тип, ID>");

  EventType type = getOrRegisterEventType(std::string{T::eventId.data(), T::eventId.size()});
  T::fabric.setEventType(type);
}

template<px::EventBasedType T>
void px::EventManager::listen(px::EventListener *listener, const px::EventCallback<T> &callback) {
  static_assert(requires {
    { T::eventId } -> std::convertible_to<std::string_view>;
  }, "У класса события отсутствует статическое поле std::string_view eventId. Наследуйте класс от px::BaseEvent.");

  listen(listener, std::string{T::eventId}, *reinterpret_cast<const DefaultEventCallback *>(&callback)); // TODO replace reinterpret_cast
}

template<px::EventBasedType T, typename... TArgs>
void px::EventManager::emplaceEvent(TArgs... args) {
  static_assert(requires {
    { T::fabric } -> std::convertible_to<px::EventFabric<T>>;
  }, "Невозможно найти фабрику класса. Наследуйте его от класса px::BaseEvent<тип, ID>");

  enqueueEvent(T::fabric.make(std::forward<TArgs>(args)...));
}
