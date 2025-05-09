#include "staffSetup.hpp"
#include "pressable.hpp"
#include "types.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <ranges>
#include <algorithm>
#include "constants.hpp"

constexpr std::array ALL_OCTAVES = std::to_array({0,1,2,3});

static std::vector<NoteSet> getNoteSet(Note note, Key key, std::span<const int> octaves, std::string_view name)
{
   return {NoteSet{notesInOctaves(getNotesForKey(note, key), octaves), std::string{name}}};
}

constexpr sf::Vector2f POS_SIZE = {55.f, 40.f};

StaffSetup::StaffSetup(std::function<void(const std::vector<NoteSet>&)> pick_notes)
   :m_pick_notes{pick_notes}
   ,m_major_buttons{
      {"C major", std::bind(m_pick_notes, getNoteSet(Note::C, Key::MAJOR, ALL_OCTAVES, "C major")), {100.f, 40.f}},
      {"D major", std::bind(m_pick_notes, getNoteSet(Note::D, Key::MAJOR, ALL_OCTAVES, "D major")), {100.f, 40.f}},
      {"E major", std::bind(m_pick_notes, getNoteSet(Note::E, Key::MAJOR, ALL_OCTAVES, "E major")), {100.f, 40.f}},
      {"F major", std::bind(m_pick_notes, getNoteSet(Note::F, Key::MAJOR, ALL_OCTAVES, "F major")), {100.f, 40.f}},
      {"G major", std::bind(m_pick_notes, getNoteSet(Note::G, Key::MAJOR, ALL_OCTAVES, "G major")), {100.f, 40.f}},
      {"A major", std::bind(m_pick_notes, getNoteSet(Note::A, Key::MAJOR, ALL_OCTAVES, "A major")), {100.f, 40.f}}
   }
   ,m_minor_buttons{
      {"c minor", std::bind(m_pick_notes, getNoteSet(Note::C, Key::MINOR, ALL_OCTAVES, "c minor")), {100.f, 40.f}},
      {"d minor", std::bind(m_pick_notes, getNoteSet(Note::D, Key::MINOR, ALL_OCTAVES, "d minor")), {100.f, 40.f}},
      {"e minor", std::bind(m_pick_notes, getNoteSet(Note::E, Key::MINOR, ALL_OCTAVES, "e minor")), {100.f, 40.f}},
      {"f minor", std::bind(m_pick_notes, getNoteSet(Note::F, Key::MINOR, ALL_OCTAVES, "f minor")), {100.f, 40.f}},
      {"g minor", std::bind(m_pick_notes, getNoteSet(Note::G, Key::MINOR, ALL_OCTAVES, "g minor")), {100.f, 40.f}},
      {"a minor", std::bind(m_pick_notes, getNoteSet(Note::A, Key::MINOR, ALL_OCTAVES, "a minor")), {100.f, 40.f}}
   }
   ,m_major_pentatonic_buttons{
      {"C major pentatonic", std::bind(m_pick_notes, getNoteSet(Note::C, Key::MAJOR_PENTATONIC, ALL_OCTAVES, "C major pentatonic")), {250.f, 40.f}},
      {"D major pentatonic", std::bind(m_pick_notes, getNoteSet(Note::D, Key::MAJOR_PENTATONIC, ALL_OCTAVES, "D major pentatonic")), {250.f, 40.f}},
      {"E major pentatonic", std::bind(m_pick_notes, getNoteSet(Note::E, Key::MAJOR_PENTATONIC, ALL_OCTAVES, "E major pentatonic")), {250.f, 40.f}},
      {"F major pentatonic", std::bind(m_pick_notes, getNoteSet(Note::F, Key::MAJOR_PENTATONIC, ALL_OCTAVES, "F major pentatonic")), {250.f, 40.f}},
      {"G major pentatonic", std::bind(m_pick_notes, getNoteSet(Note::G, Key::MAJOR_PENTATONIC, ALL_OCTAVES, "G major pentatonic")), {250.f, 40.f}},
      {"A major pentatonic", std::bind(m_pick_notes, getNoteSet(Note::A, Key::MAJOR_PENTATONIC, ALL_OCTAVES, "A major pentatonic")), {250.f, 40.f}}
   }
   ,m_minor_pentatonic_buttons{
      {"c minor pentatonic", std::bind(m_pick_notes, getNoteSet(Note::C, Key::MINOR_PENTATONIC, ALL_OCTAVES, "c minor pentatonic")), {250.f, 40.f}},
      {"d minor pentatonic", std::bind(m_pick_notes, getNoteSet(Note::D, Key::MINOR_PENTATONIC, ALL_OCTAVES, "d minor pentatonic")), {250.f, 40.f}},
      {"e minor pentatonic", std::bind(m_pick_notes, getNoteSet(Note::E, Key::MINOR_PENTATONIC, ALL_OCTAVES, "e minor pentatonic")), {250.f, 40.f}},
      {"f minor pentatonic", std::bind(m_pick_notes, getNoteSet(Note::F, Key::MINOR_PENTATONIC, ALL_OCTAVES, "f minor pentatonic")), {250.f, 40.f}},
      {"g minor pentatonic", std::bind(m_pick_notes, getNoteSet(Note::G, Key::MINOR_PENTATONIC, ALL_OCTAVES, "g minor pentatonic")), {250.f, 40.f}},
      {"a minor pentatonic", std::bind(m_pick_notes, getNoteSet(Note::A, Key::MINOR_PENTATONIC, ALL_OCTAVES, "a minor pentatonic")), {250.f, 40.f}}
   }
   ,m_position_label{FONT, "Position: "}
   ,m_I_position_button{"I", POS_SIZE}
   ,m_II_position_button{"II", POS_SIZE}
   ,m_III_position_button{"III", POS_SIZE}
   ,m_IV_position_button{"IV", POS_SIZE}
   ,m_V_position_button{"V", POS_SIZE}
   ,m_VI_position_button{"VI", POS_SIZE}
   ,m_VII_position_button{"VII", POS_SIZE}
   ,m_VIII_position_button{"VIII", POS_SIZE}
   ,m_IX_position_button{"IX", POS_SIZE}
   ,m_position_go_button{"->", std::bind(&StaffSetup::useSelectedPositions, this), {40.f, 40.f}}
   ,m_string_label{FONT, "String: "}
   ,m_E_string_button{"E", {40.f, 40.f}}
   ,m_A_string_button{"A", {40.f, 40.f}}
   ,m_D_string_button{"D", {40.f, 40.f}}
   ,m_G_string_button{"G", {40.f, 40.f}}
   ,m_B_string_button{"B", {40.f, 40.f}}
   ,m_e_string_button{"e", {40.f, 40.f}}
   ,m_string_go_button{"->", std::bind(&StaffSetup::useSelectedStrings, this), {40.f, 40.f}}
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

   m_position_label.move({0.f,m_major_buttons.size()*50.f});
   m_I_position_button.move({   120.f + 0*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 0*(POS_SIZE.y + 10.f)});
   m_II_position_button.move({  120.f + 1*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 0*(POS_SIZE.y + 10.f)});
   m_III_position_button.move({ 120.f + 2*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 0*(POS_SIZE.y + 10.f)});
   
   m_IV_position_button.move({  120.f + 0*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 1*(POS_SIZE.y + 10.f)});
   m_V_position_button.move({   120.f + 1*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 1*(POS_SIZE.y + 10.f)});
   m_VI_position_button.move({  120.f + 2*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 1*(POS_SIZE.y + 10.f)});
   
   m_VII_position_button.move({ 120.f + 0*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 2*(POS_SIZE.y + 10.f)});
   m_VIII_position_button.move({120.f + 1*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 2*(POS_SIZE.y + 10.f)});
   m_IX_position_button.move({  120.f + 2*(POS_SIZE.x + 10.f),m_major_buttons.size()*50.f + 2*(POS_SIZE.y + 10.f)});

   m_position_go_button.move({120.f + 6*50.f,m_major_buttons.size()*50.f + 1*(POS_SIZE.y + 10.f)});

   m_string_label.move({0.f,m_major_buttons.size()*50.f + 3*(POS_SIZE.y + 10.f)});
   m_E_string_button.move({120.f + 0*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});
   m_A_string_button.move({120.f + 1*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});
   m_D_string_button.move({120.f + 2*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});
   m_G_string_button.move({120.f + 3*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});
   m_B_string_button.move({120.f + 4*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});
   m_e_string_button.move({120.f + 5*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});
   m_string_go_button.move({120.f + 6*50.f,m_major_buttons.size()*50 + 3*(POS_SIZE.y + 10.f)});

   m_sharps_checkbox.move({550.f,m_major_buttons.size()*50.f});
   m_flats_checkbox.move({550.f,(m_major_buttons.size()+1)*50.f});
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

void StaffSetup::useSelectedStrings()
{
   std::vector<NoteSet> note_sets;
   if (m_E_string_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForString(0)), {"E"}});
   }
   if (m_A_string_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForString(1)), {"A"}});
   }
   if (m_D_string_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForString(2)), {"D"}});
   }
   if (m_G_string_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForString(3)), {"G"}});
   }
   if (m_B_string_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForString(4)), {"B"}});
   }
   if (m_e_string_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForString(5)), {"e"}});
   }

   if (note_sets.empty()) return;

   m_pick_notes(note_sets);
}


void StaffSetup::useSelectedPositions()
{
   std::vector<NoteSet> note_sets;
   if (m_I_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(1)), {"I"}});
   }
   if (m_II_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(2)), {"II"}});
   }
   if (m_III_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(3)), {"III"}});
   }
   if (m_IV_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(4)), {"IV"}});
   }
   if (m_V_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(5)), {"V"}});
   }
   if (m_VI_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(6)), {"VI"}});
   }
   if (m_VII_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(7)), {"VII"}});
   }
   if (m_VIII_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(8)), {"VIII"}});
   }
   if (m_IX_position_button.checked())
   {
      note_sets.push_back({filterNotes(noteOctavesForPosition(9)), {"IX"}});
   }

   if (note_sets.empty()) return;

   m_pick_notes(note_sets);
}

void StaffSetup::mouseMoved(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Pressable::mouseMoved), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_I_position_button);
   fn(m_II_position_button);
   fn(m_III_position_button);
   fn(m_IV_position_button);
   fn(m_V_position_button);
   fn(m_VI_position_button);
   fn(m_VII_position_button);
   fn(m_VIII_position_button);
   fn(m_IX_position_button);
   fn(m_position_go_button);
   fn(m_E_string_button);
   fn(m_A_string_button);
   fn(m_D_string_button);
   fn(m_G_string_button);
   fn(m_B_string_button);
   fn(m_e_string_button);
   fn(m_string_go_button);
   fn(m_sharps_checkbox);
   fn(m_flats_checkbox);
}

void StaffSetup::mouseDown(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Pressable::mouseDown), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_I_position_button);
   fn(m_II_position_button);
   fn(m_III_position_button);
   fn(m_IV_position_button);
   fn(m_V_position_button);
   fn(m_VI_position_button);
   fn(m_VII_position_button);
   fn(m_VIII_position_button);
   fn(m_IX_position_button);
   fn(m_position_go_button);
   fn(m_E_string_button);
   fn(m_A_string_button);
   fn(m_D_string_button);
   fn(m_G_string_button);
   fn(m_B_string_button);
   fn(m_e_string_button);
   fn(m_string_go_button);
   fn(m_sharps_checkbox);
   fn(m_flats_checkbox);
}

void StaffSetup::mouseUp(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Pressable::mouseUp), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_I_position_button);
   fn(m_II_position_button);
   fn(m_III_position_button);
   fn(m_IV_position_button);
   fn(m_V_position_button);
   fn(m_VI_position_button);
   fn(m_VII_position_button);
   fn(m_VIII_position_button);
   fn(m_IX_position_button);
   fn(m_position_go_button);
   fn(m_E_string_button);
   fn(m_A_string_button);
   fn(m_D_string_button);
   fn(m_G_string_button);
   fn(m_B_string_button);
   fn(m_e_string_button);
   fn(m_string_go_button);
   fn(m_sharps_checkbox);
   fn(m_flats_checkbox);
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

   target.draw(m_position_label);
   target.draw(m_I_position_button);
   target.draw(m_II_position_button);
   target.draw(m_III_position_button);
   target.draw(m_IV_position_button);
   target.draw(m_V_position_button);
   target.draw(m_VI_position_button);
   target.draw(m_VII_position_button);
   target.draw(m_VIII_position_button);
   target.draw(m_IX_position_button);
   target.draw(m_position_go_button);
   target.draw(m_string_label);
   target.draw(m_E_string_button);
   target.draw(m_A_string_button);
   target.draw(m_D_string_button);
   target.draw(m_G_string_button);
   target.draw(m_B_string_button);
   target.draw(m_e_string_button);
   target.draw(m_string_go_button);

   target.draw(m_sharps_checkbox);
   target.draw(m_flats_checkbox);
}