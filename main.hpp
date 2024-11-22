#pragma once

#include "audioProcessor.hpp"
#include "staff.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class Main
{
public:
   Main(const std::string& audio_device);

   void loop();
private:
   void pollEvents();

   sf::RenderWindow m_window;
   Staff m_staff;
   AudioProcessor m_audio_processor;
};