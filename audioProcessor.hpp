#include <SFML/Audio.hpp>
#include <vector>
#include <span>
#include <complex>

using complex = std::complex<double>;

class AudioProcessor : public sf::SoundRecorder
{
public:
   AudioProcessor();
   bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;

private:



   double getFrequency(unsigned index, unsigned number_of_samples);
   int getNoteIndex(double frequency);

   std::vector<complex> fft(std::span<const complex> x);
   std::vector<double> amplitudeToPower(std::span<const complex> amplitudes);
   double highestFrequency(std::span<const sf::Int16> samples);


   static constexpr unsigned SAMPLE_RATE = 44100;
   
   sf::SoundBuffer m_buffer;
};
