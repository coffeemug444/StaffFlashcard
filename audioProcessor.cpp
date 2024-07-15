#include "audioProcessor.hpp"
#include <iostream>
#include "staff.hpp"


AudioProcessor::AudioProcessor(std::function<void(Note)> on_note_guessed)
   :m_on_note_guessed{on_note_guessed}
{
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
   if (total_power < 1'000'000)
   {
      // get this value from a calibration step
      return true;
   }

   double lowest_frequency = 55.0; // equivalent of A1
   std::vector<double> bins {};

   for (int note = 0; note < 12*7; note++)
   {
      // 7 octaves of notes
      bins.push_back(goertzel_mag(samples_double, lowest_frequency*std::pow(2.0, note/12.0)));
   }

   for (int note = 0; note < bins.size(); note++)
   {
      if ((note + 12) < bins.size())
      {
         bins.at(note) *= bins.at(note + 12);
      }
      if ((note + 12 + 7) < bins.size())
      {
         bins.at(note) *= bins.at(note + 12 + 7);
      }
      if ((note + 2*12) < bins.size())
      {
         bins.at(note) *= bins.at(note + 2*12);
      }
      if ((note + 2*12 + 4) < bins.size())
      {
         bins.at(note) *= bins.at(note + 2*12 + 4);
      }
      if ((note + 2*12 + 7) < bins.size())
      {
         bins.at(note) *= bins.at(note + 2*12 + 7);
      }

   }

   Note note = static_cast<Note>(std::distance(bins.begin(), std::max_element(bins.begin(), bins.end())) % 12);

   m_on_note_guessed(note);

   return true;
}

double AudioProcessor::goertzel_mag(std::span<const double> samples, double frequency)
{
   int k = std::round((samples.size() * frequency) / SAMPLE_RATE);
   double omega = (2.0 * M_PI * k) / samples.size();
   double sine = sin(omega);
   double cosine = cos(omega);
   double coeff = 2.0 * cosine;
   double q0=0;
   double q1=0;
   double q2=0;

   for (double sample : samples)
   {
      q0 = coeff * q1 - q2 + sample;
      q2 = q1;
      q1 = q0;
   }

   // calculate the real and imaginary results
   // scaling appropriately
   double real = 2*(q1 - q2 * cosine) / samples.size();
   double imag = 2*(q2 * sine) / samples.size();

   double magnitude = std::sqrt(real*real + imag*imag);
   return magnitude;
}
