#include "dynamic_enum.hpp"
#include <algorithm>
#include <stdexcept>

px::DynamicEnum::EnumValue px::DynamicEnum::add(const std::string& key)
{
  EnumValue val = claimValue();

  auto pair = std::make_pair(key, val);

  m_keys.insert(std::make_pair(val, &pair.first));
  m_values.insert(std::move(pair));

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

px::DynamicEnum::EnumValue px::DynamicEnum::getOrCreate(const std::string& key)
{
  auto it = m_values.find(key);
  if (it == m_values.end())
  {
    return add(key);
  }
  return it->second;
}

const std::string& px::DynamicEnum::get(px::DynamicEnum::EnumValue val) const
{
  auto it = m_keys.find(val);
  if (it == m_keys.end())
  {
    throw std::out_of_range("Значение перечисления не найдено.");
  }
  return *it->second;
}

bool px::DynamicEnum::contains(const std::string &key) const
{
  return m_values.contains(key);
}

bool px::DynamicEnum::contains(EnumValue val) const
{
  return m_keys.contains(val);
}