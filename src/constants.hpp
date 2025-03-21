#pragma once

#include <SFML/Graphics.hpp>
#include <cassert>

const inline sf::Font FONT = [](){ sf::Font font; assert(font.openFromFile("font.ttf")); return font; }();

constexpr inline wchar_t FLAT = L'\U0000266D';          // ♭
constexpr inline wchar_t SHARP = L'\U0000266F';         // ♯
constexpr inline wchar_t WHOLE_NOTE = L'\U0001D15D';    // 𝅝
constexpr inline wchar_t CLEF = L'\U0001D11E';          // 𝄞
constexpr inline wchar_t STAFF = L'\U0001D11A';         // 𝄚
constexpr inline wchar_t STAFF_SINGLE = L'\U0001D116';  // 𝄖
constexpr inline wchar_t STAFF_DOUBLE = L'\U0001D117';  // 𝄗
constexpr inline wchar_t STAFF_TRIPLE = L'\U0001D118';  // 𝄘