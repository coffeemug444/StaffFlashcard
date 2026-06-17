#include "audioProcessor.hpp"

#include <algorithm>
#include <utility>
#include <ranges>
#include <map>

// #define DEBUG

#ifdef DEBUG
#include <fmt/core.h>
#include <fmt/color.h>
#include "types.hpp"
#endif


template <typename T>
struct Caster_t {
   template <typename U>
   auto operator()(U&& x) const
   {
      return static_cast<T>(std::forward<U>(x));
   }
};

template <typename T>
static constexpr Caster_t<T> caster{};

AudioProcessor::AudioProcessor(std::function<void(int)> on_tone_index_guessed)
   :m_on_tone_index_guessed{std::move(on_tone_index_guessed)}
{
}

bool AudioProcessor::onProcessSamples(const int16_t* samples, std::size_t sample_count)
{
   m_buffer.insert(end(m_buffer), samples, std::next(samples, sample_count));

   // process samples in 250ms chunks
   if (m_buffer.size() < PROCESSING_SIZE) return true;

   std::vector<double> samples_double;
   samples_double.reserve(sample_count);

   std::ranges::transform(m_buffer | std::views::take(PROCESSING_SIZE), std::back_inserter(samples_double), caster<double>);

   m_buffer.erase(begin(m_buffer), next(begin(m_buffer), PROCESSING_SIZE));
   auto total_power = std::ranges::fold_left(samples_double, 0.0, [](double accumulator, double x) { return accumulator += x*x; }) / PROCESSING_SIZE;
   if (total_power < 1000)
   {
      #ifdef DEBUG
      fmt::println("Skipping sample batch, power {:.1f} is too low", total_power);
      #endif
      return true;
   }


   #ifdef DEBUG
   sf::Clock clock{};
   auto start = clock.getElapsedTime().asMicroseconds();
   #endif

   double lowest_frequency = 55.0; // equivalent of A1
   std::array<double, 12*8> all_note_powers{};
   for (int note = 0; note < ssize(all_note_powers); note++)
   {
      double frequency = lowest_frequency*std::pow(2.0, note/12.0);
      all_note_powers.at(note) = goertzelMag(samples_double, frequency);
   }

   auto getNotePower = [&](int index){
      if (index < 0) return 0.0;
      return all_note_powers.at(index);
   };

   std::array<double, 12> bins{};

   for (int note = 0; note < 12*5; note++)
   {
      // 5 octaves of notes
      double sub_harm2{getNotePower(note - 24 - 4)}; // (1/5.0)*frequency
      double sub_harm1{getNotePower(note - 12 - 7)}; // (1/3.0)*frequency
      double base = getNotePower(note); // 1*frequency
      double octave{getNotePower(note + 12)}; // 2*frequency
      double harm1{getNotePower(note + 12 + 7)}; // 3*frequency
      double double_octave{getNotePower(note + 24)}; // 4*frequency
      double harm2{getNotePower(note + 24 + 4)}; // 5*frequency

      double total = base + octave + double_octave + harm1 + harm2 - sub_harm1 - sub_harm2;

      int bin_index = note % 12;
      bins.at(bin_index) += total;
   }

   #ifdef DEBUG
   auto clock_end = clock.getElapsedTime().asMicroseconds();
   fmt::println("{}us", clock_end - start);
   #endif

   auto best_note = std::ranges::max_element(bins);
   int best_power = *best_note;
   int tone_index = std::ranges::distance(begin(bins), best_note);
   std::ranges::nth_element(bins, std::next(end(bins), -2), {});
   double second_best_power = *(std::next(end(bins), -2));
   double ratio = best_power / second_best_power;

   
   #ifdef DEBUG
   bool note_is_checkable = best_power > 1000.0 and ratio > 1.1;
   fmt::color print_color = note_is_checkable ? fmt::color::green : fmt::color::red;
   fmt::print(fmt::fg(print_color), "Best note: {}, power: {}, ratio: {:.2f}\n", mapIndexToNote(tone_index), best_power, ratio);
   #endif

   if (best_power < 1000.0 or ratio < 1.1) return true;

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
   Q q = std::ranges::fold_left(samples, Q{0,0}, [&](const Q& q, double sample) {
      return Q { 
         (coeff * q.first) - q.second + sample, 
         q.first 
      };
   });

   // calculate the real and imaginary results
   // scaling appropriately
   double real = q.first - (q.second * cosine);
   double imag = q.second * sine;
   double x = 8.0/(samples.size()*samples.size());

   double magnitude = std::sqrt(x*(real*real + imag*imag));
   return magnitude;
}
