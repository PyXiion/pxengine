// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#ifndef ENGINE_EVENT_LISTENER_HPP
#define ENGINE_EVENT_LISTENER_HPP
#include <vector>
#include <functional>
#include <eventpp/callbacklist.h>
#include <px/templates.hpp>

namespace px {
  class EventManager;

  /// A class that allows you to simplify the management of callback handles.
  /// Inherit your class from it, or use it as a class member, or as just a variable.
  /// Destroys all handles in the destructor.
  class EventListener {
    friend class EventManager;

  public:
    EventListener() = default;
    ~EventListener() {
      for (const auto& f : m_destructors)
        f();
    }

  protected:

    /// Add a callback.
    ///
    /// Example                                         <br/>
    /// <code>                                          <br/>
    ///     // default                                  <br/>
    ///     auto handle = onEvent.append([]{            <br/>
    ///         // do something                         <br/>
    ///     });                                         <br/>
    ///     ...                                         <br/>
    ///     onEvent.remove(handle);                     <br/>
    ///                                                 <br/>
    ///     // with this class                          <br/>
    ///     EventListener listener;                     <br/>
    ///     listener.listen(onEvent, []{                <br/>
    ///         // do something                         <br/>
    ///     });                                         <br/>
    /// </code>
    /// \param callback Your CallbackList
    /// \param fun Your lambda or class member
    template<class TCallback, class T>
    void listen(TCallback &callback, T fun) {
      typename TCallback::Handle handle;
      using TCallbackFunction = typename TCallback::Callback;

      // checking types
      using FunctionTraits = px::function_traits<T>;

      // check if is a lambda or static function
      if constexpr (std::is_convertible_v<T, TCallbackFunction &&>) {
        handle = callback.append(fun);
      } else if constexpr (FunctionTraits::is_member) { // is class member
        using Object = typename FunctionTraits::instance_type;
        static_assert(std::is_base_of_v<EventListener, Object>, "The class of the method does not inherit from EventListener.");

        handle = callback.append([this, fun](auto &&...args) {
          auto self = reinterpret_cast<Object *>(this); // everything is fine (maybe) =D (I'm not sure)
          (self->*fun)(std::forward<decltype(args)>(args)...);
        });
      }

      // Plan the destruction
      appendDestructor([&callback, handle]{
        callback.remove(handle);
      });
    }

  private:
    std::vector<std::function<void()>> m_destructors;

    inline void appendDestructor(std::function<void()> &&f);
  };

  void EventListener::appendDestructor(std::function<void()> &&f) {
    m_destructors.push_back(f);
  }

} // px

#endif //ENGINE_EVENT_LISTENER_HPP
