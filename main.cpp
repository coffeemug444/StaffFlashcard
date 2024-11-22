#include "main.hpp"

#include "audioProcessor.hpp"
#include "staff.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>

using complex = std::complex<double>;


void Main::pollEvents() {
   sf::Event event;
   while (m_window.pollEvent(event))
   {
      switch (event.type)
      {
      case sf::Event::Closed:
         m_window.close();
         break;
      case sf::Event::KeyPressed:
         if (event.key.code == sf::Keyboard::Escape)
         {
            m_window.close();
         }
         break;
      default:
         break;
      }
   }
}


std::string getAudioDevice()
{
   std::vector<std::string> available_devices = AudioProcessor::getAvailableDevices();
   for (int i = 0; i < available_devices.size(); i++)
   {
      std::cout << i << ". " << available_devices.at(i) << std::endl;
   }

   int selected_device;
   std::cin >> selected_device;

   return available_devices.at(selected_device);
}

Main::Main(const std::string& audio_device)
   :m_window{sf::VideoMode(270, 330), "Staff flashcard"}
   ,m_staff{200}
   ,m_audio_processor{std::bind(&Staff::guessNote, &m_staff, std::placeholders::_1)}
{
   m_window.setFramerateLimit(60);

   m_audio_processor.setDevice(audio_device); // should probably check for success
   m_audio_processor.start();
}

void Main::loop()
{
   while (m_window.isOpen())
   {
      pollEvents();

      m_window.clear();
      m_window.draw(m_staff);
      m_window.display();
   }
}

int main()
{
   Main m{getAudioDevice()};
   m.loop();

   return 0;
}

