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

using complex = std::complex<double>;


void pollEvents(sf::RenderWindow& window, Staff& staff) {
   sf::Event event;
   while (window.pollEvent(event))
   {
      switch (event.type)
      {
      case sf::Event::Closed:
         window.close();
         break;
      case sf::Event::KeyPressed:
         if (event.key.code == sf::Keyboard::Escape)
         {
            window.close();
         }
         break;
      default:
         break;
      }
   }
}


void setupAudio(AudioProcessor& processor)
{
   std::vector<std::string> available_devices = AudioProcessor::getAvailableDevices();
   for (int i = 0; i < available_devices.size(); i++)
   {
      std::cout << i << ". " << available_devices.at(i) << std::endl;
   }

   int selected_device;
   std::cin >> selected_device;

   if (not processor.setDevice(available_devices.at(selected_device)))
   {
      std::cout << "Failed to set recording device\n";
      return;
   }

   processor.start();
}

int main()
{
   Staff staff{100};
   staff.move(0,10);

   AudioProcessor processor{staff};
   setupAudio(processor);
   
   sf::RenderWindow window(sf::VideoMode(150, 140), "Staff flashcard");


   while (window.isOpen())
   {
      pollEvents(window, staff);

      window.clear();
      window.draw(staff);
      window.display();
   }

   return 0;
}

