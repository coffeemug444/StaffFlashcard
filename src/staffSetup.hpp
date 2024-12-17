#pragma once

#include "button.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

class StaffSetup : public sf::Drawable
{
public:
   StaffSetup(std::function<void(const std::vector<Note>&)> pick_notes);

    void mouseMoved(const sf::Vector2f& pos);
    void mouseDown(const sf::Vector2f& pos);
    void mouseUp(const sf::Vector2f& pos);

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
   std::function<void(const std::vector<Note>&)> m_pick_notes;

   std::vector<Button> m_major_buttons;
   std::vector<Button> m_minor_buttons;
   std::vector<Button> m_major_pentatonic_buttons;
   std::vector<Button> m_minor_pentatonic_buttons;

   Button m_all_button;

};