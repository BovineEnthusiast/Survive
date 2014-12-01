#include "Bullet.h"
#include <math.h>
#include <iostream>
#include "Collision.h"
#include "Humanoid.h"
#include "Zombie.h"
Bullet::Bullet(const sf::Vector2f& startPos, const sf::Vector2f& velocity, const int damage)
:velocity_(velocity), damage_(damage)
{
    //Bullet visual description
    positionGlobal_ = startPos;
    lastPosition_ = positionGlobal_;
    bullet_.setSize(sf::Vector2f(5, 5));
    bullet_.setRotation(atan2(velocity_.y, velocity_.x) / 3.14159265358 * 180);
    bullet_.setFillColor(sf::Color(255, 204, 49, 255));
}

void Bullet::update(const sf::Time& dT)
{
    lastPosition_ = positionGlobal_;
    positionGlobal_ += velocity_ * dT.asSeconds();
    bullet_.setPosition(positionGlobal_);

}

//Setters
void Bullet::setHit(bool hit) {hit_ = hit;}
//Getters
sf::RectangleShape Bullet::getSprite() {return bullet_;}
bool Bullet::isDead() {return deathClock_.getElapsedTime().asMilliseconds() > deathTime_;}
bool Bullet::isHit() {return hit_;}
sf::Vector2f Bullet::getLastPosition() {return lastPosition_;}