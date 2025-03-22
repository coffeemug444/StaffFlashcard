#include "checkbox.hpp"
#include "constants.hpp"

Checkbox::Checkbox(const std::string& text)
   : m_text{FONT, text}
   , m_box{{30.f, 30.f}}
   , m_pressing{false}
   , m_pressed{false}
{
   m_box.setFillColor(STANDARD_COLOR);
   m_text.move({40.f, -5.f});
}

bool Checkbox::checked() const
{
   return m_pressed;
}

void Checkbox::setPosition(const sf::Vector2f& pos)
{
   sf::Vector2f current_pos = m_box.getPosition();
   move(pos - current_pos);
}

void Checkbox::move(const sf::Vector2f& diff)
{
   m_box.move(diff);
   m_text.move(diff);
}

void Checkbox::mouseMoved(const sf::Vector2f& pos)
{
   if (m_pressing) return;

   if (m_pressed)
   {
      m_box.setFillColor(hovering(pos) ? HOVER_PRESSED_COLOR : PRESSED_COLOR);
   }
   else
   {
      m_box.setFillColor(hovering(pos) ? HOVER_COLOR : STANDARD_COLOR);
   }
}

void Checkbox::mouseDown(const sf::Vector2f& pos)
{
   m_pressing = hovering(pos);
   if (m_pressing)
   {
      m_box.setFillColor(PRESSING_COLOR);
   }
}

void Checkbox::mouseUp(const sf::Vector2f& pos)
{
   if (not m_pressing) return;

   m_pressed ^= m_pressing and hovering(pos);
   m_pressing = false;

   m_box.setFillColor(m_pressed ? HOVER_PRESSED_COLOR : HOVER_COLOR);
}

void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates) const
{
   target.draw(m_box);
   target.draw(m_text);
}

bool Checkbox::hovering(const sf::Vector2f& pos) const
{
   return m_box.getGlobalBounds().contains(pos);
}
