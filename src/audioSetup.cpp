#include "audioSetup.hpp"

#include <ranges>


std::vector<Button> getButtons(const std::vector<std::string>& devices, std::function<void(const std::string&)> pick_audio_device)
{  
   std::vector<Button> buttons;
   for (const std::string& device : devices)
   {
      buttons.push_back(Button{device, std::bind(pick_audio_device, device)});
   }

   return buttons;
}


AudioSetup::AudioSetup(
      const std::vector<std::string>& devices, 
      std::function<void(const std::string&)> pick_audio_device,
      std::function<void(const sf::Vector2f&)> resize_callback)
   :m_buttons{getButtons(devices, pick_audio_device)}
{
   float longest = 0.f;
   for (auto [idx, button] : std::ranges::views::enumerate(m_buttons))
   {
      button.move({0,idx*50.f});
      longest = std::max(longest, button.getSize().x);
   }

   resize_callback({longest + 10.f,50.f*m_buttons.size() - 10.f});
}

void AudioSetup::mouseMoved(const sf::Vector2f& pos)
{
   for (Button& button : m_buttons)
   {
      button.mouseMoved(pos);
   }
}

void AudioSetup::mouseDown(const sf::Vector2f& pos)
{
   for (Button& button : m_buttons)
   {
      button.mouseDown(pos);
   }
}

void AudioSetup::mouseUp(const sf::Vector2f& pos)
{
   for (Button& button : m_buttons)
   {
      button.mouseUp(pos);
   }
}

void AudioSetup::draw(sf::RenderTarget& target, sf::RenderStates) const
{
   for (const Button& button : m_buttons)
   {
      target.draw(button);
   }
}