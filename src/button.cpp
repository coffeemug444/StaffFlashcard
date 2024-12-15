#include "button.hpp"
#include "constants.hpp"


Button::Button(const std::string& text, const sf::Vector2f& size, std::function<void()> on_click)
    :m_text{text, FONT} // size??
    ,m_background{size}
    ,m_on_click{on_click}
    ,m_pressing{false}
{
    m_background.setFillColor(STANDARD_COLOR);
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