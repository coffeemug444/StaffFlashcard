#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <span>
#include <complex>
#include <optional>

using complex = std::complex<double>;

class Staff;

class AudioProcessor : public sf::SoundRecorder
{
public:
   AudioProcessor(Staff& staff);
   bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;

   ~AudioProcessor() { stop(); }
private:

   double getFrequency(unsigned index, unsigned number_of_samples);
   std::optional<int> getNoteIndex(double frequency);

   std::vector<complex> fft(std::span<const complex> x);
   std::vector<double> amplitudeToPower(std::span<const complex> amplitudes);
   double highestFrequency(std::span<const sf::Int16> samples);
   double goertzel_mag(std::span<const double> samples, double frequency);


   static constexpr unsigned SAMPLE_RATE = 44100;

   Staff& m_staff;
   sf::SoundBuffer m_buffer;
};