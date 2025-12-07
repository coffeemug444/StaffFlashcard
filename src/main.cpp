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
#include <complex>
#include <functional>
#include <fstream>

using complex = std::complex<double>;

template<typename ... Callable>
struct visitor : Callable... {
   using Callable::operator()...;
};

namespace
{

template <typename B, typename A>
sf::Vector2<B> convertVec(const sf::Vector2<A> &vec)
{
   return sf::Vector2<B> {
      static_cast<B>(vec.x),
      static_cast<B>(vec.y),
   };
}

}


void Main::pollEvents() 
{
   while (const std::optional<sf::Event> event = m_window.pollEvent())
   {
      event->visit(visitor{
         std::bind_front(&Main::handleClosed,this),
         std::bind_front(&Main::handleKeyPressed,this),
         std::bind_front(&Main::handleMouseMoved,this),
         std::bind_front(&Main::handleMouseButtonPressed,this),
         std::bind_front(&Main::handleMouseButtonReleased,this),
         [](auto){}  // default
      });
   }
}

void Main::handleClosed(const sf::Event::Closed & /*unused*/)
{
   m_window.close();
}
void Main::handleKeyPressed(const sf::Event::KeyPressed &event)
{
   switch (event.code)
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
         break;
      }
      if (m_stage == Stage::NOTES_SETUP)
      {
         gotoAudioSetup();
         break;
      }
   default: 
      break;
   }
}
void Main::handleMouseMoved(const sf::Event::MouseMoved &event)
{
   sf::Vector2f pos = convertVec<float>(event.position);
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
}
void Main::handleMouseButtonPressed(const sf::Event::MouseButtonPressed &event)
{
   if (event.button != sf::Mouse::Button::Left)
   {
      return;
   }
   sf::Vector2f pos = convertVec<float>(event.position);
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
}
void Main::handleMouseButtonReleased(const sf::Event::MouseButtonReleased &event)
{
   if (event.button != sf::Mouse::Button::Left) return;
   
   sf::Vector2f pos = convertVec<float>(event.position);
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
}

void Main::pickAudioDevice(const std::string& device_name)
{
   assert(m_audio_processor.setDevice(device_name));

   std::ofstream file("saved_device.dat");
   assert(file);
   file << device_name;

   gotoNotesSetup();
}

void Main::gotoAudioSetup()
{
   m_audio_processor.stop();
   m_stage = Stage::AUDIO_SETUP;
   m_audio_setup.setupButtons();
}

void Main::gotoNotesSetup()
{
   setWindowSize({760, 500});
   m_stage = Stage::NOTES_SETUP;
}

void Main::gotoRunning(const std::vector<NoteSet>& notes)
{
   setWindowSize({270, 510});
   assert(m_audio_processor.start());
   m_staff.setNotes(notes);
   m_stage = Stage::RUNNING;
}

void Main::setWindowSize(const sf::Vector2f& size)
{
   m_window.setSize(convertVec<unsigned>(size));
   m_window.setView(sf::View{sf::FloatRect{{0.f, 0.f}, size}});
}


Main::Main()
   :m_window{sf::VideoMode({1, 1}), "Staff flashcard", sf::Style::Default ^ sf::Style::Resize}
   ,m_staff{200}
   ,m_staff_setup{std::bind_front(&Main::gotoRunning, this)}
   ,m_audio_processor{std::bind_front(&Staff::guessNote, &m_staff)}
   ,m_stage{Stage::AUDIO_SETUP}
   ,m_audio_setup{
      std::bind_front(&Main::pickAudioDevice, this),
      std::bind_front(&Main::setWindowSize, this)}
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
   Main main{};
   main.loop();

   return 0;
}

