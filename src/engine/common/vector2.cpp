#include "vector2.hpp"

px::Vector2::Vector2()
  : x(0.0f)
  , y(0.0f)
{}

px::Vector2::Vector2(float x, float y)
  : x(x)
  , y(y)
{}

float px::Vector2::dot(const px::Vector2 &other) const
{
  return x * other.x + y * other.y;
}

float px::Vector2::length() const
{
  return std::sqrt(x * x + y * y);
}


px::Vector2 px::Vector2::getRight() const
{
    return Vector2(y, -x);
}

px::Vector2 px::Vector2::getLeft() const
{
    return Vector2(-y, x);
}

px::Vector2 px::Vector2::normalized() const
{
  return *this / length();
}

px::Vector2 px::Vector2::operator-() const
{
  return Vector2(-x, -y);
}

px::Vector2 px::Vector2::operator+(const px::Vector2 &right) const
{
  return Vector2(x + right.x, y + right.y);
}

px::Vector2 px::Vector2::operator-(const px::Vector2 &right) const
{
  return Vector2(x - right.x, y - right.y);
}

px::Vector2 px::Vector2::operator*(float right) const
{
  return Vector2(x * right, y * right);
}

px::Vector2 px::Vector2::operator/(float right) const
{
  return Vector2(x / right, y / right);
}

px::Vector2 &px::Vector2::operator+=(const px::Vector2 &right)
{
  x += right.x;
  y += right.y;
  return *this;
}

px::Vector2 &px::Vector2::operator-=(const px::Vector2 &right)
{
  x -= right.x;
  y -= right.y;
  return *this;
}

px::Vector2 &px::Vector2::operator*=(float right)
{
  x *= right;
  y *= right;
  return *this;
}

px::Vector2 &px::Vector2::operator/=(float right)
{
  x /= right;
  y /= right;
  return *this;
}

bool px::Vector2::operator==(const px::Vector2 &right) const
{
  return x == right.x && y == right.y;
}
