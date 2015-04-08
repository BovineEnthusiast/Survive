#ifndef LIGHTPOLY_H
#define LIGHTPOLY_H
#include <SFML/Graphics.hpp>

class LightPoly
{
 public:
  LightPoly();
  
 private:

  //The light's origin
  sf::Vector2f lightOrigin_;

  //The four local points
  sf::Vector2f localPointOne_, localPointTwo_, localPointThree_, localPointFour_;

  //A vector of all sorrounding points. 4pnts/rect. 2pnts/line
  std::vector<sf::Vector2f> vOtherPoints_;
  //The light polygon will always be concave, and must be constructed from convex shapes
  std::vector<sf::ConvexShape> vConvexShapes_;

  //What the collection of convex shapes is drawn to
  sf::RenderTexture convexTexture_;

  //Sprite that uses the convexTexture_'s texture
  sf::Sprite convexSprite_;
  
  //Helper functions
  void packageHull()
};

#endif
