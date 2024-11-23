#pragma once

#include "audioProcessor.hpp"
#include "staff.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector3.hpp>
#include <string>

class Main
{
public:
   Main();

   void loop();
private:
   enum class Stage {
      SETUP,
      RUNNING
   };

   void pollEvents();
   void pickAudioDevice(int idx);
   void setWindowSize(const sf::Vector2f& size);

   sf::RenderWindow m_window;
   Staff m_staff;
   AudioProcessor m_audio_processor;
   Stage m_stage;
   
   sf::Font m_font;
   std::vector<std::string> m_audio_devices;
   sf::Text m_audio_devices_text;
};