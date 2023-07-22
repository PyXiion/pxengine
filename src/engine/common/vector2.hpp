#pragma once
#include <cmath>

namespace px
{

  /**
   * @brief Generic 2D vector class
   */
  struct Vector2 {
    Vector2();

    Vector2(float x, float y);

    float dot(const Vector2 &other) const;

    float length() const;

    Vector2 getRight() const;

    Vector2 getLeft() const;

    Vector2 normalized() const;

    Vector2 operator-() const;

    Vector2 operator+(const Vector2 &right) const;

    Vector2 operator-(const Vector2 &right) const;

    Vector2 operator*(float right) const;

    Vector2 operator/(float right) const;

    Vector2 &operator+=(const Vector2 &right);

    Vector2 &operator-=(const Vector2 &right);

    Vector2 &operator*=(float right);

    Vector2 &operator/=(float right);

    bool operator==(const Vector2 &right) const;

    float x;
    float y;
  };

}

