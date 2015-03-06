#ifndef MINE_H
#define MINE_H
#include "ImageManager.h"
#include "GameObject.h"
#include "Emitter.h"
class Mine : public GameObject
{
 public:
  Mine(sf::Texture*);
  void update(const sf::Time&);
  
  //Getters
  int getDamage() const;
  int getRadius() const;
  sf::Sprite getMine() const;
  bool exploded() const;
  Emitter getEmitter() const;

  //Helpers
  void explode();
  
 private:
  sf::Texture* pTexture_;
  sf::Sprite mineSprite_;
  Emitter emitter_;
  bool exploded_ = false;
  int damage_ = 500;
  int radius_  = 224;
};
#endif
