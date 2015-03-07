#ifndef MINE_H
#define MINE_H
#include "GameObject.h"
#include "Emitter.h"
#include "ImageManager.h"
#include "LightingPolygon.h"

class Mine : public GameObject
{
 public:
  Mine(sf::Texture*);
  void update(const sf::Time&);
  
  //Getters
  int getDamage() const;
  int getRadius() const;
  LightingPolygon getLightingPolygon() const;
  sf::Sprite getMine() const;
  bool exploded() const;
  Emitter getEmitter() const;
  float getExplosionTime() const;
  
  //Pushers
  void pushSprite(const sf::Sprite&);
  //Helpers
  void explode();
  
 private:
  sf::Texture* pTexture_;
  sf::Sprite mineSprite_;
  Emitter emitter_;
  LightingPolygon light_;
  bool exploded_ = false;
  bool needLight_ = false;
  int damage_ = 500;
  int radius_  = 500;

  sf::Clock explosionLightClock_;
};
#endif
