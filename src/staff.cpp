#include "staff.hpp"
#include "types.hpp"

#include <SFML/Graphics/Color.hpp>
#include <deque>
#include <map>
#include <random>




Staff::Staff(float height)
   :m_selectable_notes{}
   ,m_height{height}
   ,m_y1{height*0.05f}
   ,m_note_height{height*.167f}
   ,m_font{}
   ,m_cleff{std::wstring{CLEF}, m_font, static_cast<unsigned>(height)}
   ,m_lines{std::wstring(2, STAFF), m_font, static_cast<unsigned>(height)}
   ,m_extended_down_1_staff{std::wstring{STAFF_SINGLE}, m_font, static_cast<unsigned>(height)}
   ,m_extended_down_2_staff{std::wstring{STAFF_DOUBLE}, m_font, static_cast<unsigned>(height)}
   ,m_extended_up_1_staff{std::wstring{STAFF_SINGLE}, m_font, static_cast<unsigned>(height)}
   ,m_extended_up_2_staff{std::wstring{STAFF_DOUBLE}, m_font, static_cast<unsigned>(height)}
   ,m_extended_up_3_staff{std::wstring{STAFF_TRIPLE}, m_font, static_cast<unsigned>(height)}
   ,m_note{std::wstring{' '} + WHOLE_NOTE, m_font, static_cast<unsigned>(height)}
   ,m_modifier{std::wstring{' '}, m_font, static_cast<unsigned>(height*.8)}
   ,m_draw_extended_down_1_staff{false}
   ,m_draw_extended_down_2_staff{false}
   ,m_draw_extended_up_1_staff{false}
   ,m_draw_extended_up_2_staff{false}
   ,m_draw_extended_up_3_staff{false}
   ,m_display_note{false}
{
   m_font.loadFromFile("font.ttf");
   m_cleff.setPosition(height*.8,height*.35); // lol

   m_lines.setPosition(0,height*.35); // lol
   m_extended_down_1_staff.setPosition(height*.5,height*1.185); // lol
   m_extended_down_1_staff.setScale({0.8, 1.0});
   m_extended_down_2_staff.setPosition(height*.5,height*1.185); // lol
   m_extended_down_2_staff.setScale({0.8, 1.0});
   
   m_extended_up_1_staff.setPosition(height*.5, height*0.170);
   m_extended_up_1_staff.setScale({0.8, 1.0});
   m_extended_up_2_staff.setPosition(height*.5, height*0.005);
   m_extended_up_2_staff.setScale({0.8, 1.0});
   m_extended_up_3_staff.setPosition(height*.5, -height*0.160);
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

void Staff::setNotes(const std::vector<BetterNote>& notes)
{
   m_selectable_notes = notes;
   setRandomNote();
}

void Staff::drawCurrentNote()
{
   int position = mapNoteToStaffIndex(m_current_note);

   float x = m_position.x + m_height*0.4;
   float y = m_position.y + m_y1 - position*m_note_height/2 + m_height*.55;

   switch(getModifier(m_current_note.first))
   {
   case NoteModifier::FLAT:
      m_modifier.setString(std::wstring{FLAT});
      m_modifier.setPosition(x + m_height*0.15,y + m_height*0.25);
      break;
   case NoteModifier::SHARP:
      m_modifier.setString(std::wstring{SHARP});
      m_modifier.setPosition(x + m_height*0.15,y + m_height*0.32);
      break;
   case NoteModifier::NATURAL:
      m_modifier.setString("");
     break;
   }

   m_display_note = true;

   m_note.setPosition(x,y);

   m_draw_extended_down_2_staff = position == 0;
   m_draw_extended_down_1_staff =  0 < position && position <=  2;
   m_draw_extended_up_1_staff   = 13 < position && position <= 15;
   m_draw_extended_up_2_staff   = 15 < position && position <= 17;
   m_draw_extended_up_3_staff   = 17 < position && position <= 19;
}

void Staff::setRandomNote()
{
   static std::random_device rd; // obtain a random number from hardware
   static std::mt19937 gen(rd()); // seed the generator

   std::uniform_int_distribution<> note_distr(0, m_selectable_notes.size() - 1);
   std::uniform_int_distribution<> octave_distr(0, 2);

   static std::vector<NoteOctave> prev_notes (3,{BetterNote::A,-1});

   auto numWasRecent = [](const NoteOctave& note)
   {
      return ;
   };

   NoteOctave last_note = m_current_note;
   NoteModifier modifier{};
   while (true) 
   {
      BetterNote note = m_selectable_notes.at(note_distr(gen));
      m_current_note = {note, octave_distr(gen)};

      if (mapNoteToStaffIndex(m_current_note) > 18) continue;  // off the staff
      if (std::ranges::contains(prev_notes, m_current_note)) continue;  // too recent
      if (mapNoteToToneIndex(m_current_note) == mapNoteToToneIndex(last_note)) continue;  // equivalent note

      break;
   }

   std::ranges::rotate(prev_notes, std::prev(end(prev_notes)));
   prev_notes.front() = m_current_note;

   drawCurrentNote();
}

void Staff::guessNote(int tone_index)
{
   if (tone_index == mapNoteToToneIndex(m_current_note))
   {
      setRandomNote();
   }
}

void Staff::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
   if (m_display_note)
   {
      target.draw(m_note);
      target.draw(m_modifier);
   }

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