#pragma once

#include "audioProcessor.hpp"
#include "staff.hpp"
#include "staffSetup.hpp"
#include "audioSetup.hpp"

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
   void pickAudioDevice(const std::string& device_name);
   void setWindowSize(const sf::Vector2f& size);
   
   void gotoAudioSetup();
   void gotoNotesSetup();
   void gotoRunning(const std::vector<NoteSet>& notes);

   sf::RenderWindow m_window;
   Staff m_staff;
   StaffSetup m_staff_setup;
   AudioProcessor m_audio_processor;
   Stage m_stage;
   
   AudioSetup m_audio_setup;
};