#include "staff.hpp"
#include "types.hpp"
#include "constants.hpp"

#include <SFML/Graphics/Color.hpp>
#include <deque>
#include <map>
#include <random>
#include <algorithm>

const int32_t TIMEOUT_MILLISECONDS = 500;

const float HEIGHT_OFFSET = 50;


Staff::Staff(float height)
   :m_note_sets{}
   ,m_height{height}
   ,m_y1{m_height*0.05f}
   ,m_note_height{m_height*.167f}
   ,m_set_name{FONT}
   ,m_cleff{FONT, std::wstring{CLEF}, static_cast<unsigned>(height)}
   ,m_lines{FONT, std::wstring(2, STAFF), static_cast<unsigned>(height)}
   ,m_extended_down_1_staff{FONT, std::wstring{STAFF_SINGLE}, static_cast<unsigned>(height)}
   ,m_extended_down_2_staff{FONT, std::wstring{STAFF_DOUBLE}, static_cast<unsigned>(height)}
   ,m_extended_down_3_staff{FONT, std::wstring{STAFF_TRIPLE}, static_cast<unsigned>(height)}
   ,m_extended_up_1_staff{FONT, std::wstring{STAFF_SINGLE}, static_cast<unsigned>(height)}
   ,m_extended_up_2_staff{FONT, std::wstring{STAFF_DOUBLE}, static_cast<unsigned>(height)}
   ,m_extended_up_3_staff{FONT, std::wstring{STAFF_TRIPLE}, static_cast<unsigned>(height)}
   ,m_note{FONT, std::wstring{' '} + WHOLE_NOTE, static_cast<unsigned>(height)}
   ,m_modifier{FONT, std::wstring{' '}, static_cast<unsigned>(height*.8)}
   ,m_draw_extended_down_1_staff{false}
   ,m_draw_extended_down_2_staff{false}
   ,m_draw_extended_down_3_staff{false}
   ,m_draw_extended_up_1_staff{false}
   ,m_draw_extended_up_2_staff{false}
   ,m_draw_extended_up_3_staff{false}
   ,m_display_note{false}
   ,m_clock{}
   ,m_timeout{m_clock.getElapsedTime().asMilliseconds()}
{
   m_cleff.setPosition({height*.8f,HEIGHT_OFFSET + height*.35f}); // lol

   m_lines.setPosition({0.f,HEIGHT_OFFSET + height*.35f}); // lol
   m_extended_down_1_staff.setPosition({height*.5f,HEIGHT_OFFSET + height*1.185f}); // lol
   m_extended_down_1_staff.setScale({0.8, 1.0});
   m_extended_down_2_staff.setPosition({height*.5f,HEIGHT_OFFSET + height*1.185f}); // lol
   m_extended_down_2_staff.setScale({0.8, 1.0});
   m_extended_down_3_staff.setPosition({height*.5f,HEIGHT_OFFSET + height*1.185f}); // lol
   m_extended_down_3_staff.setScale({0.8, 1.0});
   
   m_extended_up_1_staff.setPosition({height*.5f, HEIGHT_OFFSET + height*0.170f});
   m_extended_up_1_staff.setScale({0.8, 1.0});
   m_extended_up_2_staff.setPosition({height*.5f, HEIGHT_OFFSET + height*0.005f});
   m_extended_up_2_staff.setScale({0.8, 1.0});
   m_extended_up_3_staff.setPosition({height*.5f, HEIGHT_OFFSET - height*0.160f});
   m_extended_up_3_staff.setScale({0.8, 1.0});
}

void Staff::setPos(float x, float y)
{
   sf::Vector2f d = sf::Vector2f{x,y} - m_position;
   move(d.x, d.y);
}

void Staff::move(float x, float y)
{
   sf::Vector2f d{x,y};
   m_position += d;

   m_cleff.move(d);
   m_lines.move(d);
   m_extended_down_1_staff.move(d);
   m_extended_down_2_staff.move(d);
   m_note.move(d);
   m_modifier.move(d);
}

void Staff::clearNote()
{
   m_display_note = false;
}

void Staff::setNotes(const std::vector<NoteSet>& note_sets)
{
   m_note_sets = note_sets;
   setRandomNote();
}

void Staff::drawCurrentNote()
{
   int position = mapNoteToStaffIndex(m_current_note);

   float x = m_position.x + m_height*0.4;
   float y = HEIGHT_OFFSET + m_position.y + m_y1 - position*m_note_height/2 + m_height*.8;

   switch(getModifier(m_current_note.first))
   {
   case NoteModifier::FLAT:
      m_modifier.setString(std::wstring{FLAT});
      m_modifier.setPosition({x + m_height*0.15f,y + m_height*0.25f});
      break;
   case NoteModifier::SHARP:
      m_modifier.setString(std::wstring{SHARP});
      m_modifier.setPosition({x + m_height*0.15f,y + m_height*0.32f});
      break;
   case NoteModifier::NATURAL:
      m_modifier.setString("");
     break;
   }

   m_display_note = true;

   m_note.setPosition({x,y});

   m_draw_extended_down_3_staff = position <= 1;
   m_draw_extended_down_2_staff =  not m_draw_extended_down_3_staff and position <= 3;
   m_draw_extended_down_1_staff =  not m_draw_extended_down_2_staff and position <= 5;

   m_draw_extended_up_3_staff   = position >= 21;
   m_draw_extended_up_2_staff   = not m_draw_extended_up_3_staff and position >= 19;
   m_draw_extended_up_1_staff   = not m_draw_extended_up_2_staff and position >= 17;
}

void Staff::setRandomNote()
{
   m_timeout = m_clock.getElapsedTime().asMilliseconds() + TIMEOUT_MILLISECONDS;

   static std::random_device rd; // obtain a random number from hardware
   static std::mt19937 gen(rd()); // seed the generator


   std::uniform_int_distribution<> note_set_distr(0, m_note_sets.size() - 1);


   static std::vector<NoteOctave> prev_notes (3,{Note::A,-1});

   NoteOctave last_note = m_current_note;
   while (true) 
   {
      const auto& note_set = m_note_sets.at(note_set_distr(gen));
      m_set_name.setString(note_set.name);

      std::uniform_int_distribution<> note_distr(0, note_set.notes.size() - 1);

      m_current_note = note_set.notes.at(note_distr(gen));

      if (mapNoteToStaffIndex(m_current_note) > 21) continue;  // off the staff
      if (std::ranges::contains(prev_notes, m_current_note)) continue;  // too recent
      if (notesAreEnharmonic(m_current_note.first, last_note.first)) continue;

      break;
   }

   std::ranges::rotate(prev_notes, std::prev(end(prev_notes)));
   prev_notes.front() = m_current_note;

   drawCurrentNote();
}

void Staff::guessNote(int tone_index)
{
   if (m_timeout > m_clock.getElapsedTime().asMilliseconds()) return;

   if (tone_index == mapNoteToToneIndex(m_current_note))
   {
      setRandomNote();
   }
}

void Staff::cheat()
{
   guessNote(mapNoteToToneIndex(m_current_note));
}

void Staff::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
   if (m_display_note)
   {
      target.draw(m_note);
      target.draw(m_modifier);
   }

   target.draw(m_set_name);
   target.draw(m_cleff);
   target.draw(m_lines);

   if (m_draw_extended_down_1_staff)
   {
      target.draw(m_extended_down_1_staff);
   }
   if (m_draw_extended_down_2_staff)
   {
      target.draw(m_extended_down_2_staff);
   }
   if (m_draw_extended_down_3_staff)
   {
      target.draw(m_extended_down_3_staff);
   }
   if (m_draw_extended_up_1_staff)
   {
      target.draw(m_extended_up_1_staff);
   }
   if (m_draw_extended_up_2_staff)
   {
      target.draw(m_extended_up_2_staff);
   }
   if (m_draw_extended_up_3_staff)
   {
      target.draw(m_extended_up_3_staff);
   }
}