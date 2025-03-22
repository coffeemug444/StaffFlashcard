#include "audioProcessor.hpp"

#include <numeric>
#include <utility>



AudioProcessor::AudioProcessor(std::function<void(int)> on_tone_index_guessed)
   :m_on_tone_index_guessed{on_tone_index_guessed}
{
}

bool AudioProcessor::onProcessSamples(const int16_t* samples, std::size_t sample_count)
{
   m_buffer.insert(end(m_buffer), samples, std::next(samples, sample_count));

   // process samples in 250ms chunks
   if (m_buffer.size() < PROCESSING_SIZE) return true;

   std::vector<double> samples_double;
   samples_double.reserve(sample_count);

   std::transform(begin(m_buffer), next(begin(m_buffer), PROCESSING_SIZE), std::back_inserter(samples_double), [](int16_t sample) { return static_cast<double>(sample); });

   m_buffer.erase(begin(m_buffer), next(begin(m_buffer), PROCESSING_SIZE));

   double lowest_frequency = 55.0; // equivalent of A1
   std::vector<double> bins {};

   for (int note = 0; note < 12*5; note++)
   {
      double frequency = lowest_frequency*std::pow(2.0, note/12.0);
      // 5 octaves of notes
      double base        = goertzelMag(samples_double,           frequency);
      double octave      = goertzelMag(samples_double,         2*frequency);
      double fifth       = goertzelMag(samples_double, (3.f/2.f)*frequency);
      double fourth      = goertzelMag(samples_double, (4.f/3.f)*frequency);

      double total = base + octave + fifth/5 + fourth/7;

      double adjustment = std::pow(3.0, note/(12.0*5.0)); // makes higher notes easier to detect

      bins.push_back(total * adjustment);
   }

   auto best_note = std::max_element(bins.begin(), bins.end());
   
   if (*best_note < 400) return true;

   int tone_index = std::distance(bins.begin(), best_note) % 12;

   m_on_tone_index_guessed(tone_index);

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
