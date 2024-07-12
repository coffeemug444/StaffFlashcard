#include "staff.hpp"


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

void Staff::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
   if (m_display_note)
   {
      target.draw(m_note);
   }

   target.draw(m_cleff);
   target.draw(m_lines);
}