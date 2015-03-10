#ifndef POWERUP_H
#define POWERUP_H
#include "GameObject.h"

class PowerUp : public GameObject
{
 public:
  PowerUp(sf::Texture*, const std::string&);

  //Getters
  bool isEquiped() const;
  sf::Sprite getSprite() const;

 private:
  std::string type_;
  sf::Texture* pTexture_;
  sf::Sprite sprite_;
};
#endif
