#include "audioProcessor.hpp"
#include <iostream>
#include "staff.hpp"
#include <utility>
#include <tuple>
#include <numeric>


AudioProcessor::AudioProcessor(std::function<void(Note)> on_note_guessed)
   :m_on_note_guessed{on_note_guessed}
{
   setProcessingInterval(sf::milliseconds(200));
}

bool AudioProcessor::onProcessSamples(const sf::Int16* samples, std::size_t sample_count)
{
   std::vector<double> samples_double;
   samples_double.reserve(sample_count);

   std::transform(samples, samples + sample_count, std::back_inserter(samples_double), [](sf::Int16 sample) { return static_cast<double>(sample); });

   double total_power = 0;
   for (auto sample : samples_double)
   {
      total_power += sample*sample;
   }

   total_power /= sample_count;

   double lowest_frequency = 55.0; // equivalent of A1
   std::vector<double> bins {};

   for (int note = 0; note < 12*7; note++)
   {
      // 7 octaves of notes
      bins.push_back(goertzelMag(samples_double, lowest_frequency*std::pow(2.0, note/12.0)));
   }

   std::vector<std::pair<int, double>> harmonics = {
      {12      , 2.0/4},  // octave (12th fret harmonic)
      {12+7    , 3.0/8},  // octave + perfect 5th (7th fret harmonic)
      {2*12    , 2.0/8},  // 2 octaves (5th fret harmonic)
      {2*12 + 4, 3.0/16}, // 2 octaves + major 3rd (9th fret harmonic)
      {2*12 + 7, 3.0/16}  // 2 octaves + perfect 5th (idk but I'm sure it's present somehow)
   };

   for (int note = 0; note < bins.size(); note++)
   {
      for (auto [harmonic, strength] : harmonics)
      {
         if ((note + harmonic) < bins.size())
         {
            bins.at(note) += strength*bins.at(note + harmonic);
         }
      }
   }

   auto best_note = std::max_element(bins.begin(), bins.end());

   if (*best_note < 500) return true;

   Note note = static_cast<Note>(std::distance(bins.begin(), best_note) % 12);

   m_on_note_guessed(note);

   return true;
}

double AudioProcessor::goertzelMag(std::span<const double> samples, double frequency)
{
   int k = std::round((samples.size() * frequency) / SAMPLE_RATE);
   double omega = (2.0 * M_PI * k) / samples.size();
   double sine = sin(omega);
   double cosine = cos(omega);
   double coeff = 2.0 * cosine;

   using Q = std::pair<double,double>;
   Q q = std::accumulate(samples.begin(), samples.end(), Q{0,0}, [&](const Q& q, double sample) {
      return Q { 
         coeff * q.first - q.second + sample, 
         q.first 
      };
   });

   // calculate the real and imaginary results
   // scaling appropriately
   double real = q.first - q.second * cosine;
   double imag = q.second * sine;
   double x = 8.0/(samples.size()*samples.size());

   double magnitude = std::sqrt(x*(real*real + imag*imag));
   return magnitude;
}
