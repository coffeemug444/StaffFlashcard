#include "staff.hpp"
#include <random>
#include <deque>
#include <iostream>
#include <map>

Staff::Staff(float height)
   :m_height{height}
   ,m_y1{-height*0.87f}
   ,m_note_height{height*.167f}
   ,m_font{}
   ,m_cleff{L"\U0001D11E", m_font, static_cast<unsigned>(height)}
   ,m_lines{L"\U0001D11A\U0001D11A", m_font, static_cast<unsigned>(height)}
   ,m_note{L"\U0001D15D", m_font, static_cast<unsigned>(height)}
   ,m_display_note{false}
{
   m_font.loadFromFile("font.ttf");
   m_cleff.move(height*.8,-height*.2); // lol
   m_lines.move(0,-height*.2); // lol
   m_note.move(0,-height*.2); // lol

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
   m_note.move(d.x,d.y);
}

void Staff::clearNote()
{
   m_display_note = false;
}

void Staff::drawNote(int position)
{
   m_display_note = true;

   float x = m_position.x + m_height*0.8;
   float y = m_position.y + m_y1 + position*m_note_height/2;
   m_note.setPosition(x,y);
}

void Staff::setRandomNote()
{
   static std::random_device rd; // obtain a random number from hardware
   static std::mt19937 gen(rd()); // seed the generator
   static std::uniform_int_distribution<> distr(-2, 5); // define the range

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


   int num = distr(gen);
   while (numWasRecent(num) or sameAsLastNote(num))
   {
      num = distr(gen);
   }

   prev_numbers.push_back(num);
   while(prev_numbers.size() > 2)
   {
      prev_numbers.pop_front();
   }

   drawNote(num);

   m_current_note = getNote(num);
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
   }

   target.draw(m_cleff);
   target.draw(m_lines);
}