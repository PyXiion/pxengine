#pragma once

namespace px::dw
{
  // Стихия магии
  enum ManaElement
  {
    // Пустая стихия (или универсальная)
    ManaElement_None
  };

  // Мана
  struct Mana
  {
    constexpr Mana(ManaElement element, double potential, double power) 
      : element(element)
      , potential(potential)
      , power(power) {}

    // Стихия
    ManaElement element = ManaElement_None;

    // Потенциал
    double potential = 0.0;
    // Сила
    double power = 0.0;
  };
}