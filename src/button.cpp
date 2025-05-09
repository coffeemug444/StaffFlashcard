#include "button.hpp"
#include "constants.hpp"
#include <SFML/System/Vector2.hpp>


Button::Button(const std::string& text, std::function<void()> on_click, sf::Vector2f size)
    :m_text{FONT, text} // size??
    ,m_background{size != sf::Vector2f{} ? size : sf::Vector2f{m_text.getGlobalBounds().size.x + 5.f, 40.f}}
    ,m_on_click{on_click}
    ,m_pressing{false}
{
    m_background.setFillColor(STANDARD_COLOR);
    m_text.move({(m_background.getSize().x - m_text.getGlobalBounds().size.x)/2.f, 0.f});
}

void Button::setPosition(const sf::Vector2f& pos)
{
    sf::Vector2f current_pos = m_background.getPosition();
    move(pos - current_pos);
}

void Button::move(const sf::Vector2f& diff)
{
    m_background.move(diff);
    m_text.move(diff);
}

sf::Vector2f Button::getSize() const
{
    return m_background.getSize();
}

void Button::mouseMoved(const sf::Vector2f& pos)
{
    if (m_pressing) return;
    bool hovering = m_background.getGlobalBounds().contains(pos);

    m_background.setFillColor(hovering ? HOVER_COLOR : STANDARD_COLOR);
}

void Button::mouseDown(const sf::Vector2f& pos)
{
    m_pressing = m_background.getGlobalBounds().contains(pos);
    if (m_pressing)
    {
        m_background.setFillColor(PRESS_COLOR);
    }
}

void Button::mouseUp(const sf::Vector2f& pos)
{
    if (m_pressing and m_background.getGlobalBounds().contains(pos))
    {
        // clicked
        m_background.setFillColor(HOVER_COLOR);
        m_on_click();
    }
    else
    {
        m_background.setFillColor(STANDARD_COLOR);
    }
    m_pressing = false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
    target.draw(m_background);
    target.draw(m_text);
}