#pragma once

#include "button.hpp"
#include "checkbox.hpp"
#include "toggleButton.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

class StaffSetup : public sf::Drawable
{
public:
   StaffSetup(std::function<void(const std::vector<NoteOctave>&)> pick_notes);

   void mouseMoved(const sf::Vector2f& pos);
   void mouseDown(const sf::Vector2f& pos);
   void mouseUp(const sf::Vector2f& pos);

   std::vector<NoteOctave> filterNotes(const std::vector<NoteOctave>&);

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
   std::function<void(const std::vector<NoteOctave>&)> m_pick_notes;

   std::vector<Button> m_major_buttons;
   std::vector<Button> m_minor_buttons;
   std::vector<Button> m_major_pentatonic_buttons;
   std::vector<Button> m_minor_pentatonic_buttons;

   Button m_first_position_button;

   sf::Text m_string_label;

   ToggleButton m_E_string_button;
   ToggleButton m_A_string_button;
   ToggleButton m_D_string_button;
   ToggleButton m_G_string_button;
   ToggleButton m_B_string_button;
   ToggleButton m_e_string_button;

   Checkbox m_sharps_checkbox;
   Checkbox m_flats_checkbox;

};