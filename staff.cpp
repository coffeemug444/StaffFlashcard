#include "staff.hpp"
#include "types.hpp"

#include <deque>
#include <map>
#include <random>

Staff::Staff(float height)
   :m_height{height}
   ,m_y1{-height*0.87f}
   ,m_note_height{height*.167f}
   ,m_font{}
   ,m_cleff{std::wstring{CLEF}, m_font, static_cast<unsigned>(height)}
   ,m_lines{std::wstring(2, STAFF), m_font, static_cast<unsigned>(height)}
   ,m_extended_down_1_staff{std::wstring{STAFF_SINGLE}, m_font, static_cast<unsigned>(height)}
   ,m_extended_down_2_staff{std::wstring{STAFF_DOUBLE}, m_font, static_cast<unsigned>(height)}
   ,m_note{std::wstring{' '} + WHOLE_NOTE, m_font, static_cast<unsigned>(height)}
   ,m_modifier{std::wstring{' '}, m_font, static_cast<unsigned>(height*.8)}
   ,m_draw_extended_down_1_staff{false}
   ,m_draw_extended_down_2_staff{false}
   ,m_display_note{false}
{
   m_font.loadFromFile("font.ttf");
   m_cleff.setPosition(height*.8,height*.1); // lol
   m_lines.setPosition(0,height*.1); // lol
   m_extended_down_1_staff.setPosition(height*.5,height*.935); // lol
   m_extended_down_1_staff.setScale({0.8, 1.0});
   m_extended_down_2_staff.setPosition(height*.5,height*.935); // lol
   m_extended_down_2_staff.setScale({0.8, 1.0});

   setRandomNote();
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

void Staff::drawNote(int position, NoteModifier modifier)
{
   float x = m_position.x + m_height*0.4;
   float y = m_position.y + m_y1 + position*m_note_height/2 + m_height*.3;

   switch(modifier)
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
}

void Staff::setRandomNote()
{
   static std::random_device rd; // obtain a random number from hardware
   static std::mt19937 gen(rd()); // seed the generator

   // -2, -1,  0  | high e
   //  1,  2,  3  | B
   //  4,  5      | G
   //  6,  7,  8  | D
   //  9, 10, 11  | A
   // 12, 13, 14  | low E
   static std::uniform_int_distribution<> distr(-2, 11); // define the range of strings

   static std::uniform_int_distribution<> offset_distr(-1, 1); // define the range for flats/sharps/naturals

   static std::deque<int> prev_numbers;

   const auto note_map = std::map<int, Note> {
      {0, Note::E},
      {1, Note::D},
      {2, Note::C},
      {3, Note::B},
      {4, Note::A},
      {5, Note::G},
      {6, Note::F},
   };

   auto getNote = [note_map](int idx) -> Note {
      while (idx < 0)
      {
         idx += 7;
      }
      return note_map.at(idx % 7);
   };


   auto sameAsLastNote = [getNote](int idx) {
      if (prev_numbers.empty()) return false;
      Note last = getNote(prev_numbers.back());
      Note current = getNote(idx);
      return last == current;
   };

   auto numWasRecent = [](int idx)
   {
      return std::find(prev_numbers.begin(), prev_numbers.end(), idx) != prev_numbers.end();
   };


   auto last_note = m_current_note;
   int num{};
   NoteModifier modifier{};
   do 
   {
      num = distr(gen);
      modifier = static_cast<NoteModifier>(offset_distr(gen));
      int idx = static_cast<int>(getNote(num)) + static_cast<int>(modifier) + 12;
      m_current_note = static_cast<Note>(idx % 12);
   }
   while (numWasRecent(num) or m_current_note == last_note);

   prev_numbers.push_back(num);
   while(prev_numbers.size() > 2)
   {
      prev_numbers.pop_front();
   }

   drawNote(num, modifier);

   m_draw_extended_down_2_staff = num >= 11;
   m_draw_extended_down_1_staff = not m_draw_extended_down_2_staff and num >= 9;
}

void Staff::guessNote(Note note)
{
   if (note == m_current_note)
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
}