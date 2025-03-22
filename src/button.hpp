#pragma once

#include "pressable.hpp"
#include <SFML/Graphics.hpp>
#include <functional>


class Button : public sf::Drawable, public Pressable
{
public:
    Button(const std::string& text, std::function<void()> on_click, sf::Vector2f size = {});

    void setPosition(const sf::Vector2f& pos);
    void move(const sf::Vector2f& diff);
    sf::Vector2f getSize() const;

    void mouseMoved(const sf::Vector2f& pos) override;
    void mouseDown(const sf::Vector2f& pos) override;
    void mouseUp(const sf::Vector2f& pos) override;

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
