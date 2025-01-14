#pragma once

#include "types.hpp"

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class Staff : public sf::Drawable
{
public:
   Staff(float height);

   void setPos(float x, float y);
   void move(float x, float y);

   void clearNote();

   void setNotes(const std::vector<NoteOctave>& notes);

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   void guessNote(int tone_index);
   void cheat();
private:
   void drawCurrentNote();

   void setRandomNote();
   float noteHeight() { return m_height*0.135; };

   constexpr static inline wchar_t FLAT = L'\U0000266D';          // 𝅝
   constexpr static inline wchar_t SHARP = L'\U0000266F';         // ♯
   constexpr static inline wchar_t WHOLE_NOTE = L'\U0001D15D';    // ♭
   constexpr static inline wchar_t CLEF = L'\U0001D11E';          // 𝄞
   constexpr static inline wchar_t STAFF = L'\U0001D11A';         // 𝄚
   constexpr static inline wchar_t STAFF_SINGLE = L'\U0001D116';  // 𝄖
   constexpr static inline wchar_t STAFF_DOUBLE = L'\U0001D117';  // 𝄗
   constexpr static inline wchar_t STAFF_TRIPLE = L'\U0001D118';  // 𝄘

   std::vector<NoteOctave> m_selectable_notes;

   NoteOctave m_current_note;
   sf::Vector2f m_position;

   float m_height;

   float m_y1;

   float m_note_height;

   sf::Font m_font;
   sf::Text m_cleff;
   sf::Text m_lines;
   sf::Text m_extended_down_1_staff;
   sf::Text m_extended_down_2_staff;
   sf::Text m_extended_down_3_staff;
   sf::Text m_extended_up_1_staff;
   sf::Text m_extended_up_2_staff;
   sf::Text m_extended_up_3_staff;
   sf::Text m_note;
   sf::Text m_modifier;

   bool m_draw_extended_down_1_staff;
   bool m_draw_extended_down_2_staff;
   bool m_draw_extended_down_3_staff;
   bool m_draw_extended_up_1_staff;
   bool m_draw_extended_up_2_staff;
   bool m_draw_extended_up_3_staff;

   bool m_display_note;

   sf::Clock m_clock;
   sf::Int32 m_timeout;

   static constexpr inline int NUM_LINES = 5;
};