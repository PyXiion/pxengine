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

  /// A class that allows you to simplify the management of callback handles.
  /// Inherit your class from it, or use it as a class member, or as just a variable.
  /// Destroys all handles in the destructor.
  class EventListener {
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
    /// \param fun
    template<class TCallback>
      requires px::is_base_of_template_v<eventpp::CallbackList, TCallback>

    void listen(TCallback &callback, TCallback::Callback &&fun) {
      // Append the callback and get a listener
      typename TCallback::Handle handle = callback.append(fun);

      // Plan the destruction
      m_destructors.push_back([&callback, handle]{
        callback.remove(handle);
      });
    }

  private:
    std::vector<std::function<void()>> m_destructors;
  };

} // px

#endif //ENGINE_EVENT_LISTENER_HPP
