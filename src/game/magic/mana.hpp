#pragma once

namespace px
{
  /// @brief Стихия магии
  enum ManaElement
  {
    /// @brief Пустая стихия (или универсальная)
    ManaElement_None
  };

  /// @brief Мана
  struct Mana
  {
    constexpr Mana(ManaElement element, double potential, double power) 
      : element(element)
      , potential(potential)
      , power(power) {}

    /// @brief Стихия
    ManaElement element = ManaElement_None;

    /// @brief Потенциал
    float potential = 0.0;
    /// @brief Сила
    float power = 0.0;
  };
}