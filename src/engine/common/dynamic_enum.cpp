#include "dynamic_enum.hpp"

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
  return m_values.at(key);
}

std::string px::DynamicEnum::get(px::DynamicEnum::EnumValue val) const
{
  return m_keys.at(val);
}
