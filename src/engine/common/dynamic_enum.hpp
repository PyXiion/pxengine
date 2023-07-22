#pragma once
#include <unordered_map>
#include <string>

namespace px
{
  class DynamicEnum
  {
  public:
    using EnumValue = int32_t;

  public:
    EnumValue add(const std::string &key);

    EnumValue get(const std::string &key) const;
    std::string get(EnumValue val) const;

  private:
    std::unordered_map<std::string, EnumValue> m_values;
    std::unordered_map<EnumValue, std::string> m_keys;

    EnumValue claimValue();
  };
}
