#pragma once

#include "button.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class AudioSetup : public sf::Drawable
{
public:
   AudioSetup(
      std::function<void(const std::string&)> pick_audio_device,
      std::function<void(const sf::Vector2f&)> resize_callback);

   void setupButtons();

   void mouseMoved(const sf::Vector2f& pos);
   void mouseDown(const sf::Vector2f& pos);
   void mouseUp(const sf::Vector2f& pos);

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
   std::function<void(const std::string&)> m_pick_audio_device_callback;
   std::function<void(const sf::Vector2f&)> m_resize_callback_callback;
   std::vector<Button> m_buttons;
};