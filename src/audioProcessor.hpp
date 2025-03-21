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
   AudioProcessor(std::function<void(int)> on_tone_index_guessed);
   bool onProcessSamples(const int16_t* samples, std::size_t sampleCount) override;

   ~AudioProcessor() { stop(); }
private:

   double goertzelMag(std::span<const double> samples, double frequency);
   static constexpr unsigned SAMPLE_RATE = 44100;

   std::function<void(int)> m_on_tone_index_guessed;
   sf::SoundBuffer m_buffer;
};
