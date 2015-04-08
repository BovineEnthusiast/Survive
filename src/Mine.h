#ifndef MINE_H
#define MINE_H
#include "GameObject.h"
#include "Emitter.h"
#include "ImageManager.h"

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
  float getExplosionTime() const;
  bool isDead() const;
  
  //Helpers
  void explode();
  
 private:
  sf::Texture* pTexture_;
  sf::Sprite mineSprite_;
  Emitter emitter_;
  bool exploded_ = false;
  bool needLight_ = false;
  bool dead_ = false;
  int damage_ = 300;
  int radius_  = 150;

};
#endif
