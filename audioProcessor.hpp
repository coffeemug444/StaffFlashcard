#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <span>
#include <complex>
#include <optional>
#include <functional>
#include "types.hpp"

using complex = std::complex<double>;

class Staff;

class AudioProcessor : public sf::SoundRecorder
{
public:
   AudioProcessor(std::function<void(Note)> on_note_guessed);
   bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;

   ~AudioProcessor() { stop(); }
private:

   double goertzel_mag(std::span<const double> samples, double frequency);
   static constexpr unsigned SAMPLE_RATE = 44100;

   std::function<void(Note)> m_on_note_guessed;
   sf::SoundBuffer m_buffer;
};
