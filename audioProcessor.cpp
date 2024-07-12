#include "audioProcessor.hpp"
#include <iostream>


AudioProcessor::AudioProcessor()
{
   std::cout << "Audio processor constructed\n";
}

bool AudioProcessor::onProcessSamples(const sf::Int16* samples, std::size_t sample_count)
{
   // Check if avg power level is high enough first

   int note_index = getNoteIndex(highestFrequency({samples, sample_count}));

   std::cout << [&note_index]() {
      switch (note_index)
      {
      case 0:  return "A" ;
      case 1:  return "A#";
      case 2:  return "B" ;
      case 3:  return "C" ;
      case 4:  return "C#";
      case 5:  return "D" ;
      case 6:  return "D#";
      case 7:  return "E" ;
      case 8:  return "F" ;
      case 9:  return "F#";
      case 10: return "G" ;
      case 11: return "G#";
      default: return ""  ;
      }
   }() << std::endl;

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

int AudioProcessor::getNoteIndex(double frequency)
{
   int index = std::round(12*std::log2(frequency/440.0));
   while (index < 0)
   {
      index += 12;
   }
   return index % 12;
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

   int i = std::distance(frequency_data.begin(), std::max_element(frequency_data.begin(), frequency_data.end()));

   return getFrequency(i, frequency_data.size());
}
