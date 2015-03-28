#ifndef BULLET_H
#define	BULLET_H
#include "GameObject.h"
#include "Emitter.h"
#include <SFML/Graphics.hpp>
class Bullet : public GameObject
{
 public:
  Bullet(const bool, const sf::Vector2f&, const sf::Vector2f&, const int);
  void update(const sf::Time&);
    
  //Setters
  void setHit(bool);
  void setFromTurret(const bool);
  void setColor(const sf::Color&);
  void setSize(const sf::Vector2f&);
  void setSpeed(const int);
  
  //Getters
  sf::RectangleShape getSprite() const;
  int getDamage() const;
  bool isDead() const;
  bool isHit() const;
  bool isRocket() const;
  sf::Vector2f getLastPosition();
  Emitter getRocketEmitter() const;
  Emitter getExplosionEmitter() const;
  bool isFromTurret() const;
  float getShake();

 private:   
  sf::RectangleShape bullet_;
  sf::Vector2f velocity_;
  sf::Vector2f lastPosition_;
  int damage_ = 25;
  int speed_ = 250;
  //Used to determine when it should die if it hits nothing
  int deathTime_ = 1250;
  sf::Clock deathClock_;
  bool hit_ = false;
  bool fromTurret_ = false;

  //For rocket launchers
  bool rocket_ = false;
  Emitter rocketEmitter_;
  Emitter explosionEmitter_;

  //Shake from explosion
  float shake_ = 0.0f;

  sf::Clock explosionClock_;
};

#endif














