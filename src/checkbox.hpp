#pragma once

#include "pressable.hpp"
#include <SFML/Graphics.hpp>

class Checkbox : public sf::Drawable, public Pressable
{
public:
   Checkbox(const std::string& text);

   bool checked() const;

   void setPosition(const sf::Vector2f& pos);
   void move(const sf::Vector2f& diff);

   void mouseMoved(const sf::Vector2f& pos) override;
   void mouseDown(const sf::Vector2f& pos) override;
   void mouseUp(const sf::Vector2f& pos) override;

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
   bool hovering(const sf::Vector2f& pos) const;

   sf::Text m_text;
   sf::RectangleShape m_box;

   const static inline sf::Color STANDARD_COLOR = sf::Color::Blue;
   const static inline sf::Color HOVER_COLOR = sf::Color(100, 100, 255);
   const static inline sf::Color HOVER_PRESSED_COLOR = sf::Color(178, 100, 178);
   const static inline sf::Color PRESSING_COLOR = sf::Color::Magenta;
   const static inline sf::Color PRESSED_COLOR = sf::Color(178, 0, 178);

   bool m_pressing;
   bool m_pressed;
};
