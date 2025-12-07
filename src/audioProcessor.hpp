#pragma once

#include <SFML/Audio.hpp>
#include <complex>
#include <functional>
#include <span>

using complex = std::complex<double>;

class Staff;

class AudioProcessor : public sf::SoundRecorder
{
public:
   explicit AudioProcessor(std::function<void(int)> on_tone_index_guessed);
   bool onProcessSamples(const int16_t* samples, std::size_t sample_count) override;

   ~AudioProcessor() override { stop(); }
private:

   static double goertzelMag(std::span<const double> samples, double frequency);
   static constexpr unsigned SAMPLE_RATE = 44100;
   static constexpr unsigned PROCESSES_PER_SECOND = 8;
   static constexpr unsigned PROCESSING_SIZE = SAMPLE_RATE / PROCESSES_PER_SECOND;

   std::function<void(int)> m_on_tone_index_guessed;
   std::vector<int16_t> m_buffer;
};
