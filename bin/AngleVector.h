#ifndef ANGLEVECTOR_H
#define ANGLEVECTOR_H
#include <SFML/Graphics.hpp>

class AngleVector
{
 public:
  AngleVector();
  AngleVector(const sf::Vector2f&);
 private:
  sf::Vector2f position_;
  sf::Vector2f vector_;
  float rotation
}
#endif
