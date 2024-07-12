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

int getRandomNumber()
{
   static std::random_device rd; // obtain a random number from hardware
   static std::mt19937 gen(rd()); // seed the generator
   static std::uniform_int_distribution<> distr(-2, 3); // define the range

   static std::deque<int> prev_numbers;

   int num = distr(gen);
   while (std::find(prev_numbers.begin(), prev_numbers.end(), num) != prev_numbers.end())
   {
      num = distr(gen);
   }

   prev_numbers.push_back(num);
   while(prev_numbers.size() > 2)
   {
      prev_numbers.pop_front();
   }

   return num;
}

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
         if (event.key.code == sf::Keyboard::Enter)
         {
            staff.drawNote(getRandomNumber());
         }
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


void doSoundStuff()
{
   std::vector<std::string> available_devices = AudioProcessor::getAvailableDevices();
   for (int i = 0; i < available_devices.size(); i++)
   {
      std::cout << i << ". " << available_devices.at(i) << std::endl;
   }

   int selected_device;
   std::cin >> selected_device;

   AudioProcessor processor;
   if (not processor.setDevice(available_devices.at(selected_device)))
   {
      std::cout << "Failed to set recording device\n";
      return;
   }

   processor.start();

   while (true) {}
}

int main()
{
   doSoundStuff();
   return 1;
   sf::RenderWindow window(sf::VideoMode(150, 140), "Staff flashcard");

   Staff staff{100};
   staff.move(0,10);

   while (window.isOpen())
   {
      pollEvents(window, staff);

      window.clear();
      window.draw(staff);
      window.display();
   }

   return 0;
}

