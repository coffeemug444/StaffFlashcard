#include "toggleButton.hpp"
#include "constants.hpp"

ToggleButton::ToggleButton(const std::string& text, sf::Vector2f size)
    :m_text{FONT, text} // size??
    ,m_background{size != sf::Vector2f{} ? size : sf::Vector2f{m_text.getGlobalBounds().size.x + 5.f, 40.f}}
    ,m_pressing{false}
{
    m_background.setFillColor(STANDARD_COLOR);
    m_text.move({(size.x - m_text.getGlobalBounds().size.x)/2.f, 0.f});
}

bool ToggleButton::checked() const
{
   return m_pressed;
}

void ToggleButton::setPosition(const sf::Vector2f& pos)
{
   sf::Vector2f current_pos = m_background.getPosition();
   move(pos - current_pos);
}

void ToggleButton::move(const sf::Vector2f& diff)
{
   m_background.move(diff);
   m_text.move(diff);
}

void ToggleButton::mouseMoved(const sf::Vector2f& pos)
{
   if (m_pressing) return;

   if (m_pressed)
   {
      m_background.setFillColor(hovering(pos) ? HOVER_PRESSED_COLOR : PRESSED_COLOR);
   }
   else
   {
      m_background.setFillColor(hovering(pos) ? HOVER_COLOR : STANDARD_COLOR);
   }
}

void ToggleButton::mouseDown(const sf::Vector2f& pos)
{
   m_pressing = hovering(pos);
   if (m_pressing)
   {
      m_background.setFillColor(PRESSING_COLOR);
   }
}

void ToggleButton::mouseUp(const sf::Vector2f& pos)
{
   if (not m_pressing) return;

   m_pressed ^= m_pressing and hovering(pos);
   m_pressing = false;

   m_background.setFillColor(m_pressed ? HOVER_PRESSED_COLOR : HOVER_COLOR);
}

void ToggleButton::draw(sf::RenderTarget& target, sf::RenderStates) const
{
   target.draw(m_background);
   target.draw(m_text);
}

bool ToggleButton::hovering(const sf::Vector2f& pos) const
{
   return m_background.getGlobalBounds().contains(pos);
}
