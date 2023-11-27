#pragma once
#include <unordered_map>
#include <vector>
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
     * @brief Получает значение элемента или создает его по ключу.
     * @param key Ключ элемента.
     * @return Значение элемента.
     */
    EnumValue getOrCreate(const std::string &key);

    /**
     * @brief Получает ключ элемента по его значению.
     * @param val Значение элемента.
     * @return Ключ элемента.
     * @throw std::out_of_range если элемент с данным значением отсутствует в перечислении.
     */
    const std::string& get(EnumValue val) const;

    bool contains(const std::string &key) const;
    bool contains(EnumValue val) const;

    std::vector<std::pair<std::string, EnumValue>> getPairs() const;

  private:
    std::unordered_map<std::string, EnumValue> m_values;
    std::unordered_map<EnumValue, const std::string*> m_keys;

    /**
     * @brief Получить следующее свободное значение элемента. 
     */
    EnumValue claimValue();
  };
}
