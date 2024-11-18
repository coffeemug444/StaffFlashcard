#pragma once

#include "types.hpp"

#include <SFML/Graphics.hpp>

class Staff : public sf::Drawable
{
public:
   Staff(float height);

   void setPos(float x, float y);
   void move(float x, float y);

   void clearNote();

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   void guessNote(Note note);
private:
   void drawNote(int position);

   void setRandomNote();
   float noteHeight() { return m_height*0.135; };

   constexpr static inline wchar_t FLAT = L'\U0000266D';          // 𝅝
   constexpr static inline wchar_t SHARP = L'\U0000266F';         // ♯
   constexpr static inline wchar_t WHOLE_NOTE = L'\U0001D15D';    // ♭
   constexpr static inline wchar_t CLEF = L'\U0001D11E';          // 𝄞
   constexpr static inline wchar_t STAFF = L'\U0001D11A';         // 𝄚
   constexpr static inline wchar_t STAFF_SINGLE = L'\U0001D116';  // 𝄖
   constexpr static inline wchar_t STAFF_DOUBLE = L'\U0001D117';  // 𝄗

   Note m_current_note;
   sf::Vector2f m_position;

   float m_height;

   float m_y1;

   float m_note_height;

   sf::Font m_font;
   sf::Text m_cleff;
   sf::Text m_lines;
   sf::Text m_extended_down_1_staff;
   sf::Text m_extended_down_2_staff;
   sf::Text m_note;

   bool m_draw_extended_down_1_staff;
   bool m_draw_extended_down_2_staff;

   bool m_display_note;

   static constexpr inline int NUM_LINES = 5;
};