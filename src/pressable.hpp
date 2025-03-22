#pragma once

#include <SFML/System/Vector2.hpp>

class Pressable
{
public:
   virtual void mouseMoved(const sf::Vector2f& pos) = 0;
   virtual void mouseDown(const sf::Vector2f& pos) = 0;
   virtual void mouseUp(const sf::Vector2f& pos) = 0;
};
