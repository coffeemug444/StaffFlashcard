#pragma once

#include <SFML/Graphics.hpp>
#include <functional>


class Button : public sf::Drawable
{
public:
    Button(const std::string& text, const sf::Vector2f& size, std::function<void()> on_click);
    Button(const std::string& text, std::function<void()> on_click);

    void setPosition(const sf::Vector2f& pos);
    void move(const sf::Vector2f& diff);
    sf::Vector2f getSize() const;

    void mouseMoved(const sf::Vector2f& pos);
    void mouseDown(const sf::Vector2f& pos);
    void mouseUp(const sf::Vector2f& pos);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
    sf::Text m_text;
    sf::RectangleShape m_background;
    std::function<void()> m_on_click;

    const static inline sf::Color STANDARD_COLOR = sf::Color::Blue;
    const static inline sf::Color HOVER_COLOR = sf::Color(100, 100, 255);
    const static inline sf::Color PRESS_COLOR = sf::Color::Magenta;

    bool m_pressing;
};
