#pragma once

#include "audioProcessor.hpp"
#include "staff.hpp"
#include "staffSetup.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector3.hpp>
#include <string>

class Main
{
public:
   Main();

   void loop();
private:
   enum class Stage {
      AUDIO_SETUP,
      NOTES_SETUP,
      RUNNING
   };

   void pollEvents();
   void pickAudioDevice(int idx);
   void setWindowSize(const sf::Vector2f& size);
   
   void gotoNotesSetup();
   void gotoRunning(const std::vector<NoteOctave>& notes);

   sf::RenderWindow m_window;
   Staff m_staff;
   StaffSetup m_staff_setup;
   AudioProcessor m_audio_processor;
   Stage m_stage;
   
   sf::Font m_font;
   std::vector<std::string> m_audio_devices;
   sf::Text m_audio_devices_text;
};