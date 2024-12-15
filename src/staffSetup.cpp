#include "staffSetup.hpp"
#include "types.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <ranges>


StaffSetup::StaffSetup(std::function<void(const std::vector<BetterNote>&)> pick_notes)
   :m_pick_notes{pick_notes}
   ,m_major_buttons{
      {"C major", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::C, Key::MAJOR));}},
      {"D major", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::D, Key::MAJOR));}},
      {"E major", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::E, Key::MAJOR));}},
      {"F major", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::F, Key::MAJOR));}},
      {"G major", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::G, Key::MAJOR));}},
      {"A major", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::A, Key::MAJOR));}}
   }
   ,m_minor_buttons{
      {"c minor", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::C, Key::MINOR));}},
      {"d minor", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::D, Key::MINOR));}},
      {"e minor", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::E, Key::MINOR));}},
      {"f minor", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::F, Key::MINOR));}},
      {"g minor", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::G, Key::MINOR));}},
      {"a minor", {100.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::A, Key::MINOR));}}
   }
   ,m_major_pentatonic_buttons{
      {"C major pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::C, Key::MAJOR_PENTATONIC));}},
      {"D major pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::D, Key::MAJOR_PENTATONIC));}},
      {"E major pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::E, Key::MAJOR_PENTATONIC));}},
      {"F major pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::F, Key::MAJOR_PENTATONIC));}},
      {"G major pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::G, Key::MAJOR_PENTATONIC));}},
      {"A major pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::A, Key::MAJOR_PENTATONIC));}}
   }
   ,m_minor_pentatonic_buttons{
      {"c minor pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::C, Key::MINOR_PENTATONIC));}},
      {"d minor pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::D, Key::MINOR_PENTATONIC));}},
      {"e minor pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::E, Key::MINOR_PENTATONIC));}},
      {"f minor pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::F, Key::MINOR_PENTATONIC));}},
      {"g minor pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::G, Key::MINOR_PENTATONIC));}},
      {"a minor pentatonic", {250.f, 40.f}, [this](){m_pick_notes(getNotesForKey(BetterNote::A, Key::MINOR_PENTATONIC));}}
   }
   ,m_all_button{"all", {100.f, 40.f}, [this](){m_pick_notes(getAllNotes());}}
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

   m_all_button.move({0.f,m_major_buttons.size()*50.f});
}


void StaffSetup::mouseMoved(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseMoved), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_all_button);
}

void StaffSetup::mouseDown(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseDown), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_all_button);
}

void StaffSetup::mouseUp(const sf::Vector2f& pos)
{
   auto fn = std::bind(std::mem_fn(&Button::mouseUp), std::placeholders::_1, pos);

   std::ranges::for_each(m_major_buttons, fn);
   std::ranges::for_each(m_minor_buttons, fn);
   std::ranges::for_each(m_major_pentatonic_buttons, fn);
   std::ranges::for_each(m_minor_pentatonic_buttons, fn);
   fn(m_all_button);
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

   target.draw(m_all_button);
}