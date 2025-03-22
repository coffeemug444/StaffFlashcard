#include "staffSetup.hpp"
#include "types.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <ranges>
#include <algorithm>
#include "constants.hpp"

constexpr std::array ALL_OCTAVES = std::to_array({0,1,2,3});

StaffSetup::StaffSetup(std::function<void(const std::vector<NoteOctave>&)> pick_notes)
   :m_pick_notes{pick_notes}
   ,m_major_buttons{
      {"C major", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MAJOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"D major", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MAJOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"E major", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MAJOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"F major", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MAJOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"G major", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MAJOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"A major", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MAJOR), ALL_OCTAVES)), {100.f, 40.f}}
   }
   ,m_minor_buttons{
      {"c minor", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MINOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"d minor", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MINOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"e minor", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MINOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"f minor", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MINOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"g minor", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MINOR), ALL_OCTAVES)), {100.f, 40.f}},
      {"a minor", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MINOR), ALL_OCTAVES)), {100.f, 40.f}}
   }
   ,m_major_pentatonic_buttons{
      {"C major pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MAJOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"D major pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MAJOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"E major pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MAJOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"F major pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MAJOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"G major pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MAJOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"A major pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MAJOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}}
   }
   ,m_minor_pentatonic_buttons{
      {"c minor pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MINOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"d minor pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MINOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"e minor pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MINOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"f minor pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MINOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"g minor pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MINOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}},
      {"a minor pentatonic", std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MINOR_PENTATONIC), ALL_OCTAVES)), {250.f, 40.f}}
   }
   ,m_first_position_button{"First position", [this](){ m_pick_notes(filterNotes(noteOctavesForFirstPosition())); }, {180.f, 40.f}}
   ,m_string_label{FONT, "String: "}
   ,m_E_string_button{"E", [this](){ m_pick_notes(filterNotes(noteOctavesForEString())); }, {40.f, 40.f}}
   ,m_A_string_button{"A", [this](){ m_pick_notes(filterNotes(noteOctavesForAString())); }, {40.f, 40.f}}
   ,m_D_string_button{"D", [this](){ m_pick_notes(filterNotes(noteOctavesForDString())); }, {40.f, 40.f}}
   ,m_G_string_button{"G", [this](){ m_pick_notes(filterNotes(noteOctavesForGString())); }, {40.f, 40.f}}
   ,m_B_string_button{"B", [this](){ m_pick_notes(filterNotes(noteOctavesForBString())); }, {40.f, 40.f}}
   ,m_e_string_button{"e", [this](){ m_pick_notes(filterNotes(noteOctavesForeString())); }, {40.f, 40.f}}
   ,m_sharps_checkbox{"Sharps"}
   ,m_flats_checkbox{"Flats"}
{
   for (auto [idx, button] : std::ranges::views::enumerate(m_major_buttons))
   {
      button.move({0.f,idx*50.f});
   }
   for (auto [idx, button] : std::ranges::views::enumerate(m_minor_buttons))
   {
      button.move({120.f,idx*50.f});
   }
   for (auto [idx, button] : std::ranges::views::enumerate(m_major_pentatonic_buttons))
   {
      button.move({240.f,idx*50.f});
   }
   for (auto [idx, button] : std::ranges::views::enumerate(m_minor_pentatonic_buttons))
   {
      button.move({510.f,idx*50.f});
   }

   m_first_position_button.move({0.f,m_major_buttons.size()*50.f});

   m_string_label.move({0.f,m_major_buttons.size()*50.f + 50.f});
   m_E_string_button.move({120.f + 0*50.f,m_major_buttons.size()*50 + 50.f});
   m_A_string_button.move({120.f + 1*50.f,m_major_buttons.size()*50 + 50.f});
   m_D_string_button.move({120.f + 2*50.f,m_major_buttons.size()*50 + 50.f});
   m_G_string_button.move({120.f + 3*50.f,m_major_buttons.size()*50 + 50.f});
   m_B_string_button.move({120.f + 4*50.f,m_major_buttons.size()*50 + 50.f});
   m_e_string_button.move({120.f + 5*50.f,m_major_buttons.size()*50 + 50.f});

   m_sharps_checkbox.move({450.f,m_major_buttons.size()*50.f});
   m_flats_checkbox.move({450.f,(m_major_buttons.size()+1)*50.f});
}

std::vector<NoteOctave> StaffSetup::filterNotes(const std::vector<NoteOctave>& notes)
{
   return notes 
      | std::ranges::views::filter([this](NoteOctave note) 
      { 
         switch (getModifier(note.first))
         {
         case NoteModifier::FLAT:
            return m_flats_checkbox.checked();
         case NoteModifier::NATURAL:
            return true;
         case NoteModifier::SHARP:
            return m_sharps_checkbox.checked();
         }
         return false;
      })
      | std::ranges::to<std::vector>();
}

void StaffSetup::mouseMoved(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseMoved), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_first_position_button);
   fn(m_E_string_button);
   fn(m_A_string_button);
   fn(m_D_string_button);
   fn(m_G_string_button);
   fn(m_B_string_button);
   fn(m_e_string_button);

   m_sharps_checkbox.mouseMoved(pos);
   m_flats_checkbox.mouseMoved(pos);
}

void StaffSetup::mouseDown(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseDown), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_first_position_button);
   fn(m_E_string_button);
   fn(m_A_string_button);
   fn(m_D_string_button);
   fn(m_G_string_button);
   fn(m_B_string_button);
   fn(m_e_string_button);

   m_sharps_checkbox.mouseDown(pos);
   m_flats_checkbox.mouseDown(pos);
}

void StaffSetup::mouseUp(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseUp), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_first_position_button);
   fn(m_E_string_button);
   fn(m_A_string_button);
   fn(m_D_string_button);
   fn(m_G_string_button);
   fn(m_B_string_button);
   fn(m_e_string_button);

   m_sharps_checkbox.mouseUp(pos);
   m_flats_checkbox.mouseUp(pos);
}


void StaffSetup::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{

   for (const Button& button : m_major_buttons)
   {
      target.draw(button);
   }
   for (const Button& button : m_minor_buttons)
   {
      target.draw(button);
   }
   for (const Button& button : m_major_pentatonic_buttons)
   {
      target.draw(button);
   }
   for (const Button& button : m_minor_pentatonic_buttons)
   {
      target.draw(button);
   }

   target.draw(m_first_position_button);
   target.draw(m_string_label);
   target.draw(m_E_string_button);
   target.draw(m_A_string_button);
   target.draw(m_D_string_button);
   target.draw(m_G_string_button);
   target.draw(m_B_string_button);
   target.draw(m_e_string_button);

   target.draw(m_sharps_checkbox);
   target.draw(m_flats_checkbox);
}