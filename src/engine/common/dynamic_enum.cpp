#include "dynamic_enum.hpp"
#include <algorithm>
#include <stdexcept>

px::DynamicEnum::EnumValue px::DynamicEnum::add(const std::string& key)
{
  EnumValue val = claimValue();

  m_values[key] = val;
  m_keys[val] = key;

  return val;
}

px::DynamicEnum::EnumValue px::DynamicEnum::claimValue()
{
  return m_values.size();
}

px::DynamicEnum::EnumValue px::DynamicEnum::get(const std::string& key) const
{
  auto it = m_values.find(key);
  if (it == m_values.end())
  {
    throw std::out_of_range("Ключ перечисления не найден.");
  }
  return it->second;
}

std::string px::DynamicEnum::get(px::DynamicEnum::EnumValue val) const
{
  auto it = m_keys.find(val);
  if (it == m_keys.end())
  {
    throw std::out_of_range("Значение перечисления не найдено.");
  }
  return it->second;
}
