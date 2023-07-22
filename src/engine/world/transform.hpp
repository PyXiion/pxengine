#pragma once
#include "../common/vector3.hpp"
#include "../common/matrix4x4.hpp"

namespace px
{
  class Transform
  {
  public:
    Transform(Vector3 position, Vector3 eulerAngles);

    const Matrix4x4 &getTransformMatrix() const;

  private:
    Matrix4x4 m_transform;
    bool m_isCached;

    Vector3 m_position;
    Vector3 m_rotation;

    void calculate();
  };
}
