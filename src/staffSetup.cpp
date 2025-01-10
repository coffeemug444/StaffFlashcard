#include "staffSetup.hpp"
#include "types.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <ranges>

constexpr std::array ALL_OCTAVES = std::to_array({0,1,2,3});

static std::vector<NoteOctave> firstPositionNotes()
{
   auto first_position_notes = notesInOctaves(getAllNotes(), std::to_array({0,1}));

   // there are a few more in octave 2
   using enum Note;
   auto notes_above_e = notesInOctaves(std::to_array({
      Fb, F, Fs,
      Gb, G, Gs
   }), std::to_array({2}));

   first_position_notes.insert(end(first_position_notes), begin(notes_above_e), end(notes_above_e));
   return first_position_notes;
}


StaffSetup::StaffSetup(std::function<void(const std::vector<NoteOctave>&)> pick_notes)
   :m_pick_notes{pick_notes}
   ,m_major_buttons{
      {"C major", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MAJOR), ALL_OCTAVES))},
      {"D major", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MAJOR), ALL_OCTAVES))},
      {"E major", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MAJOR), ALL_OCTAVES))},
      {"F major", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MAJOR), ALL_OCTAVES))},
      {"G major", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MAJOR), ALL_OCTAVES))},
      {"A major", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MAJOR), ALL_OCTAVES))}
   }
   ,m_minor_buttons{
      {"c minor", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MINOR), ALL_OCTAVES))},
      {"d minor", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MINOR), ALL_OCTAVES))},
      {"e minor", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MINOR), ALL_OCTAVES))},
      {"f minor", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MINOR), ALL_OCTAVES))},
      {"g minor", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MINOR), ALL_OCTAVES))},
      {"a minor", {100.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MINOR), ALL_OCTAVES))}
   }
   ,m_major_pentatonic_buttons{
      {"C major pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MAJOR_PENTATONIC), ALL_OCTAVES))},
      {"D major pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MAJOR_PENTATONIC), ALL_OCTAVES))},
      {"E major pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MAJOR_PENTATONIC), ALL_OCTAVES))},
      {"F major pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MAJOR_PENTATONIC), ALL_OCTAVES))},
      {"G major pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MAJOR_PENTATONIC), ALL_OCTAVES))},
      {"A major pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MAJOR_PENTATONIC), ALL_OCTAVES))}
   }
   ,m_minor_pentatonic_buttons{
      {"c minor pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::C, Key::MINOR_PENTATONIC), ALL_OCTAVES))},
      {"d minor pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::D, Key::MINOR_PENTATONIC), ALL_OCTAVES))},
      {"e minor pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::E, Key::MINOR_PENTATONIC), ALL_OCTAVES))},
      {"f minor pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::F, Key::MINOR_PENTATONIC), ALL_OCTAVES))},
      {"g minor pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::G, Key::MINOR_PENTATONIC), ALL_OCTAVES))},
      {"a minor pentatonic", {250.f, 40.f}, std::bind(m_pick_notes, notesInOctaves(getNotesForKey(Note::A, Key::MINOR_PENTATONIC), ALL_OCTAVES))}
   }
   ,m_first_position_button{"First position", {180.f, 40.f}, std::bind(m_pick_notes, firstPositionNotes())}
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
}


void StaffSetup::mouseMoved(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseMoved), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_first_position_button);
}

void StaffSetup::mouseDown(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseDown), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_first_position_button);
}

void StaffSetup::mouseUp(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseUp), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_first_position_button);
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
}