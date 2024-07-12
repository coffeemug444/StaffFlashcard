#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <deque>
#include "staff.hpp"
#include <complex>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>

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




std::vector<complex> fft(const std::vector<complex>& x) {
   const int N = x.size();

   if (N <= 1) return x;

   std::vector<complex> even(x.size()/2);
   std::vector<complex> odd(x.size()/2);
   for (unsigned i = 0; i < N; i++)
   {
      (i%2==0 ? even : odd).at(i/2) = x.at(i);
   }

   even = fft(even);
   odd = fft(odd);

   std::vector<complex> result(N);
   for (unsigned k = 0; k < N/2; k++)
   {
      complex t = odd.at(k) * std::exp(complex(0,-2*M_PI) * complex(static_cast<double>(k)/N,0));
      result.at(k    ) = even.at(k) + t;
      result.at(k+N/2) = even.at(k) - t;
   }

   return result;
}

double getFrequency(unsigned index, unsigned sample_rate, unsigned number_of_samples)
{
   return static_cast<double>(index * sample_rate)/number_of_samples;
}

int getNoteIndex(double frequency)
{
   return std::round(12*std::log2(frequency/440.0));
}

void doSoundStuff()
{
   sf::SoundBuffer buffer;
   if (!buffer.loadFromFile("0.wav")) {
      // Error handling if file loading fails
      std::cout << "Couldn't load\n";
      return;
   }
   const sf::Int16* samples = buffer.getSamples();
   std::size_t sample_count = buffer.getSampleCount();
   std::vector<complex> audio_data(sample_count);

   auto sample_rate = buffer.getSampleRate();

   std::transform(samples, samples + sample_count, 
      audio_data.begin(),
      [](sf::Int16 i) { return complex(i, 0); }
   );

   int nearest_pow_2 = 1;
   while (sample_count > nearest_pow_2)
   {
      nearest_pow_2 *= 2;
   }
   audio_data.resize(nearest_pow_2);

   std::vector<complex> frequency_data = fft(audio_data);

   int i = 0;
   double highest = 0;
   for (int j = 0; j < frequency_data.size(); j++)
   {
      double val = std::abs(frequency_data.at(j));
      if (val > highest)
      {
         highest = val;
         i = j;
      }
   }

   int note_index = getNoteIndex(getFrequency(i, sample_rate, frequency_data.size()));
   while (note_index < 0)
   {
      note_index += 12;
   }
   note_index = note_index % 12;

   switch (note_index)
   {
   case 0:  std::cout << "A\n"; break;
   case 1:  std::cout << "A#\n"; break;
   case 2:  std::cout << "B\n"; break;
   case 3:  std::cout << "C\n"; break;
   case 4:  std::cout << "C#\n"; break;
   case 5:  std::cout << "D\n"; break;
   case 6:  std::cout << "D#\n"; break;
   case 7:  std::cout << "E\n"; break;
   case 8:  std::cout << "F\n"; break;
   case 9:  std::cout << "F#\n"; break;
   case 10: std::cout << "G\n"; break;
   case 11: std::cout << "G#\n"; break;
   default:
      break;
   }
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

