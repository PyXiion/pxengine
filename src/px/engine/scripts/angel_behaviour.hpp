// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#ifndef PX_ENGINE_ANGEL_BEHAVIOUR_HPP
#define PX_ENGINE_ANGEL_BEHAVIOUR_HPP

#include <mutex>
#include "px/engine/components/component.hpp"
#include "px/script/object_type.hpp"

namespace px {

  class AngelBehaviour final : public BaseComponent<"core.angel_behaviour"> {
  private:
    using Method = script::Method<void ()>;

  public:
    AngelBehaviour();
    explicit AngelBehaviour(const std::string &scriptId);

    void loadFromScript(const std::string &scriptId);
    void loadFromAsType(script::ObjectType);

  private:
    // TODO caching methods
    Method m_start;
    Method m_update;
    Method m_finish;

    static std::unique_ptr<script::ObjectType> baseAngelBehaviourType;
    static std::once_flag staticInit;
    static void init();
  };

} // px

#endif //PX_ENGINE_ANGEL_BEHAVIOUR_HPP
