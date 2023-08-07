#pragma once
#include <unordered_map>
#include <string>

namespace px
{
  /**
   * @brief Класс для создания динамического перечисления.
   */
  class DynamicEnum
  {
  public:
    using EnumValue = int;

    /**
     * @brief Конструктор по умолчанию.
     */
    DynamicEnum() = default;

    /**
     * @brief Добавить новый элемент в перечисление.
     * @param key Ключ элемента.
     * @return Значение элемента.
     */
    EnumValue add(const std::string &key);

    /**
     * @brief Получает значение элемента по его ключу.
     * @param key Ключ элемента.
     * @return Значение элемента.
     * @throw std::out_of_range если элемент с данным ключом отсуствует в перечислении.
     */
    EnumValue get(const std::string &key) const;

    /**
     * @brief Получает ключ элемента по его значению.
     * @param val Значение элемента.
     * @return Ключ элемента.
     * @throw std::out_of_range если элемент с данным значением отсутствует в перечислении.
     */
    std::string get(EnumValue val) const;

  private:
    std::unordered_map<std::string, EnumValue> m_values;
    std::unordered_map<EnumValue, std::string> m_keys;

    /**
     * @brief Получить следующее свободное значение элемента. 
     */
    EnumValue claimValue();
  };
}
