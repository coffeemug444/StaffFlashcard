#pragma once

#include "audioProcessor.hpp"
#include "staff.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

class Main
{
public:
   Main();

   void loop();
private:
   void onNoteGuessed(Note note);
   void pollEvents();
   void setupAudio();

   std::unique_ptr<sf::RenderWindow> m_window;
   Staff m_staff;
   AudioProcessor m_audio_processor;
};