#include "main.hpp"

#include "audioProcessor.hpp"
#include "staff.hpp"
#include "types.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include <complex>
#include <functional>
#include <ranges>

using complex = std::complex<double>;


void Main::pollEvents() {
   sf::Event event;
   while (m_window.pollEvent(event))
   {
      switch (event.type)
      {
      case sf::Event::Closed:
         m_window.close();
         break;
      case sf::Event::KeyPressed:
         if (event.key.code == sf::Keyboard::Escape)
         {
            m_window.close();
         }
         break;
      case sf::Event::TextEntered:
      {
         if (m_stage != Stage::AUDIO_SETUP) break;
         if (event.text.unicode >= 128) break;
         char c = event.text.unicode;
         if (c < '0' or c > '9') break;
         pickAudioDevice(c - '0');
      }
      case sf::Event::MouseMoved:
      {
         if (m_stage != Stage::NOTES_SETUP) break;
         sf::Vector2f pos = {static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)};
         m_staff_setup.mouseMoved(pos);
         break;
      }
      case sf::Event::MouseButtonPressed:
      {
         if (m_stage != Stage::NOTES_SETUP) break;
         if (event.mouseButton.button != sf::Mouse::Button::Left) break;
         sf::Vector2f pos = {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};
         m_staff_setup.mouseDown(pos);
         break;
      }
      case sf::Event::MouseButtonReleased:
      {
         if (m_stage != Stage::NOTES_SETUP) break;
         if (event.mouseButton.button != sf::Mouse::Button::Left) break;
         sf::Vector2f pos = {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};
         m_staff_setup.mouseUp(pos);
         break;
      }

      default:
         break;
      }
   }
}


void Main::pickAudioDevice(int idx)
{
   if (idx >= m_audio_devices.size()) return;
   if (not m_audio_processor.setDevice(m_audio_devices.at(idx))) return;
   m_audio_processor.start();

   gotoNotesSetup();
}

void Main::gotoNotesSetup()
{
   setWindowSize({760, 340});
   m_stage = Stage::NOTES_SETUP;
}

void Main::gotoRunning(const std::vector<BetterNote>& notes)
{
   setWindowSize({270, 360});
   m_staff.setNotes(notes);
   m_stage = Stage::RUNNING;
}

void Main::setWindowSize(const sf::Vector2f& size)
{
   auto float_to_unsigned = [](const sf::Vector2f v) { return sf::Vector2u{ static_cast<unsigned>(v.x), static_cast<unsigned>(v.y) }; };
   m_window.setSize(float_to_unsigned(size));
   m_window.setView(sf::View{sf::FloatRect{0.f, 0.f, size.x, size.y}});
}

Main::Main()
   :m_window{sf::VideoMode(1, 1), "Staff flashcard", sf::Style::Default ^ sf::Style::Resize}
   ,m_staff{200}
   ,m_staff_setup{std::bind(&Main::gotoRunning, this, std::placeholders::_1)}
   ,m_audio_processor{std::bind(&Staff::guessNote, &m_staff, std::placeholders::_1)}
   ,m_stage{Stage::AUDIO_SETUP}
   ,m_font{[](){ sf::Font font; font.loadFromFile("font.ttf"); return font; }()}
   ,m_audio_devices{AudioProcessor::getAvailableDevices()}
   ,m_audio_devices_text(
      std::ranges::views::join(
         std::ranges::views::enumerate(m_audio_devices) 
       | std::ranges::views::transform([](std::tuple<long, const std::string &>&& str_enumerated) {
            auto [idx, str] = str_enumerated; 
            return std::to_string(idx) + ". " + str + '\n';
         })) 
       | std::ranges::to<std::string>()
      , m_font)
{
   m_window.setFramerateLimit(60);

   auto text_bounds = m_audio_devices_text.getGlobalBounds();
   text_bounds.width += 5;
   setWindowSize(text_bounds.getSize());
}

void Main::loop()
{
   while (m_window.isOpen())
   {
      pollEvents();

      m_window.clear();
      switch (m_stage)
      {
      case Stage::AUDIO_SETUP:
         m_window.draw(m_audio_devices_text);
         break;
      case Stage::NOTES_SETUP:
         m_window.draw(m_staff_setup);
         break;
      case Stage::RUNNING:
         m_window.draw(m_staff);
         break;
      }
      m_window.display();
   }
}

int main()
{
   Main m{};
   m.loop();

   return 0;
}

