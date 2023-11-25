// Copyright (c) 2023.

#pragma once
#include "px/engine/math/vector.hpp"
#include "px/engine/math/matrix.hpp"
#include "component.hpp"

namespace px
{
  class Transform : public Component
  {
    PX_COMPONENT_DECLARATION
  public:
    Transform(Vector3 position = {0, 0, 0}, Vector3 eulerAngles = {0, 0, 0});

    void move(Vector3 offset);
    void rotate(Vector3 angles);

    void setPosition(Vector3 position);
    void setRotation(Vector3 eulerAngles);

    [[nodiscard]] Vector3 getRotation() const;
    [[nodiscard]] Vector3 getPosition() const;

    const glm::mat4 &getTransformMatrix();

    void guiEditor() override;

  private:
    Matrix4x4 m_transform;
    bool m_isCached;

    Vector3 m_position;
    Vector3 m_rotation;

    void calculate();
  };
}
