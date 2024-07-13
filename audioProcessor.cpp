#include "audioProcessor.hpp"
#include <iostream>
#include "staff.hpp"


AudioProcessor::AudioProcessor(Staff& staff)
   :m_staff{staff}
{
   setProcessingInterval(sf::milliseconds(100));
}

bool AudioProcessor::onProcessSamples(const sf::Int16* samples, std::size_t sample_count)
{
   // Check if avg power level is high enough first
   std::span<const sf::Int16> samples_span{samples, sample_count};

   double total_power = 0;
   for (auto sample : samples_span)
   {
      total_power += sample*sample;
   }

   total_power /= sample_count;
   if (total_power < 1'000'000)
   {
      // get this value from a calibration step
      return true;
   }

   std::optional<int> note_index = getNoteIndex(highestFrequency(samples_span));
   if (not note_index.has_value()) return true;

   m_staff.guessNote(getNoteFromIndex(note_index.value()));

   return true;
}

std::vector<complex> AudioProcessor::fft(std::span<const complex> x) {
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

double AudioProcessor::getFrequency(unsigned index, unsigned number_of_samples)
{
   return static_cast<double>(index * SAMPLE_RATE)/number_of_samples;
}

std::optional<int> AudioProcessor::getNoteIndex(double frequency)
{

   double index = 12*std::log2(frequency/440.0);
   int rounded_index = std::round(index);
   double remainder = index - rounded_index;

   if (0.1 <= remainder and remainder <= 0.9)
   {
      // this is unacceptable!!
      return std::nullopt;
   }

   while (rounded_index < 0)
   {
      rounded_index += 12;
   }
   return rounded_index % 12;
}

std::vector<double> AudioProcessor::amplitudeToPower(std::span<const complex> amplitudes)
{
   std::vector<double> power;
   power.reserve(amplitudes.size());
   std::transform(amplitudes.begin(), amplitudes.end(), std::back_inserter(power), [](const complex& x) { return std::abs(x); });
   return power;
}

double AudioProcessor::highestFrequency(std::span<const sf::Int16> samples)
{
   std::vector<complex> audio_data;
   audio_data.reserve(samples.size());
   std::transform(samples.begin(), samples.end(), 
      std::back_inserter(audio_data),
      [](sf::Int16 i) { return complex(i, 0); }
   );

   audio_data.resize(std::pow(2.0, std::ceil(std::log2(samples.size()))));
   const std::vector<double>& frequency_data = amplitudeToPower(fft(audio_data));

   // int i = std::distance(frequency_data.begin(), std::max_element(frequency_data.begin(), frequency_data.end()));

   int i = std::distance(frequency_data.begin(), std::find_if(frequency_data.begin(), frequency_data.end(), [](double x) { return x > 1'000'000; }));
   if (i == frequency_data.size())
   {
      i = std::distance(frequency_data.begin(), std::max_element(frequency_data.begin(), frequency_data.end()));
   }

   return getFrequency(i, frequency_data.size());
}
