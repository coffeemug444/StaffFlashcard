#include <SFML/Graphics.hpp>

class Staff : public sf::Drawable
{
public:
   Staff(float height);

   void setPos(float x, float y);
   void move(float x, float y);

   void clearNote();
   void drawNote(int position);

   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
   float noteHeight() { return m_height*0.135; };

   sf::Vector2f m_position;

   float m_height;

   float m_y1;

   float m_note_height;

   sf::Font m_font;
   sf::Text m_cleff;
   sf::Text m_lines;
   sf::Text m_note;

   bool m_display_note;

   static constexpr int NUM_LINES = 5;
};