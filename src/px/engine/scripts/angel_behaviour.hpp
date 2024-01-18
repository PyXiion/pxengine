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
  namespace script {
    class AngelScript;
  }

  class AngelBehaviour final : public BaseComponent<"core.angel_behaviour"> {
    using Method = script::Method<void ()>;

  public:
    explicit AngelBehaviour(asIScriptObject *object);

    ~AngelBehaviour() override;

    void loadFromScript(const std::string &scriptId);

    void loadFromAsType(script::ObjectType);

    static void bindToEngine(script::AngelScript &as);

  private:
    PX_NO_COPY(AngelBehaviour)

    static std::shared_ptr<AngelBehaviour> factory();

    asIScriptObject *m_obj;
    asILockableSharedBool *m_isDead;
  };
  using AngelBehaviourPtr = std::shared_ptr<AngelBehaviour>;
} // px

PX_AS_TYPENAME(px::AngelBehaviourPtr, "AngelBehaviour_t")

#endif //PX_ENGINE_ANGEL_BEHAVIOUR_HPP
