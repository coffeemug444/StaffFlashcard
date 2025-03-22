#include "main.hpp"

#include "audioProcessor.hpp"
#include "staff.hpp"
#include "types.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <complex>
#include <functional>

using complex = std::complex<double>;

template<typename ... Callable>
struct visitor : Callable... {
   using Callable::operator()...;
};


template <typename B, typename A>
sf::Vector2<B> convertVec(sf::Vector2<A> a)
{
   return sf::Vector2<B> {
      static_cast<B>(a.x),
      static_cast<B>(a.y),
   };
}


void Main::pollEvents() 
{
   while (const std::optional<sf::Event> event = m_window.pollEvent())
   {
      event->visit(visitor{
         [&](sf::Event::Closed)
         {
            m_window.close();
         },
         [&](sf::Event::KeyPressed ev)
         {
            switch (ev.code)
            {
            case sf::Keyboard::Key::Escape:
               m_window.close();
               break;
            case sf::Keyboard::Key::Enter:
               m_staff.cheat();
               break;
            case sf::Keyboard::Key::Backspace:
               if (m_stage == Stage::RUNNING)
               {
                  gotoNotesSetup();
               }
            default: 
               break;
            }
         },
         [&](sf::Event::MouseMoved ev)
         {
            sf::Vector2f pos = convertVec<float>(ev.position);
            switch(m_stage)
            {
               using enum Stage;
               case AUDIO_SETUP:
                  m_audio_setup.mouseMoved(pos);
                  break;
               case NOTES_SETUP:
                  m_staff_setup.mouseMoved(pos);
                  break;
               case RUNNING:
                  break;
            }
         },
         [&](sf::Event::MouseButtonPressed ev)
         {
            if (ev.button != sf::Mouse::Button::Left) return;
            sf::Vector2f pos = convertVec<float>(ev.position);
            switch(m_stage)
            {
               using enum Stage;
               case AUDIO_SETUP:
                  m_audio_setup.mouseDown(pos);
                  break;
               case NOTES_SETUP:
                  m_staff_setup.mouseDown(pos);
                  break;
               case RUNNING:
                  break;
            }
         },
         [&](sf::Event::MouseButtonReleased ev)
         {
            if (ev.button != sf::Mouse::Button::Left) return;
            sf::Vector2f pos = convertVec<float>(ev.position);
            switch(m_stage)
            {
               using enum Stage;
               case AUDIO_SETUP:
                  m_audio_setup.mouseUp(pos);
                  break;
               case NOTES_SETUP:
                  m_staff_setup.mouseUp(pos);
                  break;
               case RUNNING:
                  break;
            }
         },
         [](auto){} // default
      });
   }
}

void Main::pickAudioDevice(const std::string& device_name)
{
   if (not m_audio_processor.setDevice(device_name)) exit(-1);
   assert(m_audio_processor.start());

   gotoNotesSetup();
}

void Main::gotoNotesSetup()
{
   setWindowSize({760, 400});
   m_stage = Stage::NOTES_SETUP;
}

void Main::gotoRunning(const std::vector<NoteOctave>& notes)
{
   setWindowSize({270, 460});
   m_staff.setNotes(notes);
   m_stage = Stage::RUNNING;
}

void Main::setWindowSize(const sf::Vector2f& size)
{
   auto float_to_unsigned = [](const sf::Vector2f v) { return sf::Vector2u{ static_cast<unsigned>(v.x), static_cast<unsigned>(v.y) }; };
   m_window.setSize(float_to_unsigned(size));
   m_window.setView(sf::View{sf::FloatRect{{0.f, 0.f}, size}});
}


Main::Main()
   :m_window{sf::VideoMode({1, 1}), "Staff flashcard", sf::Style::Default ^ sf::Style::Resize}
   ,m_staff{200}
   ,m_staff_setup{std::bind(&Main::gotoRunning, this, std::placeholders::_1)}
   ,m_audio_processor{std::bind(&Staff::guessNote, &m_staff, std::placeholders::_1)}
   ,m_stage{Stage::AUDIO_SETUP}
   ,m_font{[](){ sf::Font font; assert(font.openFromFile("font.ttf")); return font; }()}
   ,m_audio_setup{
      AudioProcessor::getAvailableDevices(), 
      std::bind(&Main::pickAudioDevice, this, std::placeholders::_1),
      std::bind(&Main::setWindowSize, this, std::placeholders::_1)}
{
   m_window.setFramerateLimit(144);
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
         m_window.draw(m_audio_setup);
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

