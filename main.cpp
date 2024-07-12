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

std::vector<complex> fft(std::span<const complex> x) {
   const int N = x.size();

   if (N <= 1) return std::vector<complex>{x.begin(), x.end()};

   std::vector<complex> even(x.size()/2);
   std::vector<complex> odd(x.size()/2);
   for (unsigned i = 0; i < N; i++)
   {
      (i%2==0 ? even : odd)[i/2] = x[i];
   }

   even = fft(even);
   odd = fft(odd);

   std::vector<complex> result(N);
   for (unsigned k = 0; k < N/2; k++)
   {
      complex t = odd[k] * std::exp(complex(0,-2*M_PI) * complex(static_cast<double>(k)/N,0));
      result[k    ] = even[k] + t;
      result[k+N/2] = even[k] - t;
   }

   return result;
}

double getFrequency(unsigned index, unsigned sample_rate, unsigned number_of_samples)
{
   return static_cast<double>(index * sample_rate)/number_of_samples;
}

int getNoteIndex(double frequency)
{
   int index = std::round(12*std::log2(frequency/440.0));
   while (index < 0)
   {
      index += 12;
   }
   return index % 12;
}

std::vector<double> amplitudeToPower(std::span<const complex> amplitudes)
{
   std::vector<double> power;
   power.reserve(amplitudes.size());
   std::transform(amplitudes.begin(), amplitudes.end(), std::back_inserter(power), [](const complex& x) { return std::abs(x); });
   return power;
}

double highestFrequency(std::span<const sf::Int16> samples, unsigned sample_rate)
{
   std::vector<complex> audio_data;
   audio_data.reserve(samples.size());
   std::transform(samples.begin(), samples.end(), 
      std::back_inserter(audio_data),
      [](sf::Int16 i) { return complex(i, 0); }
   );

   audio_data.resize(std::pow(2.0, std::ceil(std::log2(samples.size()))));
   const std::vector<double>& frequency_data = amplitudeToPower(fft(audio_data));

   int i = std::distance(frequency_data.begin(), std::max_element(frequency_data.begin(), frequency_data.end()));

   return getFrequency(i, sample_rate, frequency_data.size());
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
   
   int note_index = getNoteIndex(highestFrequency({samples, sample_count}, buffer.getSampleRate()));
   const char* note_name;
   switch (note_index)
   {
   case 0:  note_name = "A" ; break;
   case 1:  note_name = "A#"; break;
   case 2:  note_name = "B" ; break;
   case 3:  note_name = "C" ; break;
   case 4:  note_name = "C#"; break;
   case 5:  note_name = "D" ; break;
   case 6:  note_name = "D#"; break;
   case 7:  note_name = "E" ; break;
   case 8:  note_name = "F" ; break;
   case 9:  note_name = "F#"; break;
   case 10: note_name = "G" ; break;
   case 11: note_name = "G#"; break;
   default:
      break;
   }
   std::cout << note_name << std::endl;
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

