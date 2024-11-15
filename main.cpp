#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <deque>
#include "staff.hpp"
#include <complex>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <span>
#include "audioProcessor.hpp"
#include "main.hpp"
#include <functional>

using complex = std::complex<double>;


void Main::pollEvents() {
   sf::Event event;
   while (m_window->pollEvent(event))
   {
      switch (event.type)
      {
      case sf::Event::Closed:
         m_window->close();
         break;
      case sf::Event::KeyPressed:
         if (event.key.code == sf::Keyboard::Escape)
         {
            m_window->close();
         }
         break;
      default:
         break;
      }
   }
}


void Main::setupAudio()
{
   std::vector<std::string> available_devices = AudioProcessor::getAvailableDevices();
   for (int i = 0; i < available_devices.size(); i++)
   {
      std::cout << i << ". " << available_devices.at(i) << std::endl;
   }

   int selected_device;
   std::cin >> selected_device;

   if (not m_audio_processor.setDevice(available_devices.at(selected_device)))
   {
      std::cout << "Failed to set recording device\n";
      return;
   }

   m_audio_processor.start();
}

void Main::onNoteGuessed(Note note)
{
   m_staff.guessNote(note);
}

Main::Main()
   :m_window{}
   ,m_staff{200}
   ,m_audio_processor{std::bind(&Main::onNoteGuessed, this, std::placeholders::_1)}
{
   setupAudio();
   m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(270, 280), "Staff flashcard");
}

void Main::loop()
{
   while (m_window->isOpen())
   {
      pollEvents();

      m_window->clear();
      m_window->draw(m_staff);
      m_window->display();
   }
}

int main()
{
   Main m;
   m.loop();

   return 0;
}

