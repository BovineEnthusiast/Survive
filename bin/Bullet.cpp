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
	bullet_.setSize(sf::Vector2f(15.0f, 3.3f));
	bullet_.setRotation(atan2(velocity_.y, velocity_.x) / 3.14159265358 * 180);
	bullet_.setFillColor(sf::Color(255, 204, 49, 255));
	bullet_.setOrigin(bullet_.getLocalBounds().width / 2.0f, bullet_.getLocalBounds().height / 2.0f);
	//Light
	light_.setRadius(100.0f);
	light_.setOrigin(light_.getLocalBounds().width / 2.0f, light_.getLocalBounds().height / 2.0f);
	//Critical
	if (std::rand() % 100 == 0)
	{
		damage_ *= 5;
		bullet_.setFillColor(sf::Color(255, 50, 50, 255));
	}
}

void Bullet::update(const sf::Time& dT)
{
	lastPosition_ = positionGlobal_;
	positionGlobal_ += velocity_ * dT.asSeconds();
	bullet_.setPosition(positionGlobal_);
}

//Setters
void Bullet::setHit(bool hit) { hit_ = hit; }
//Getters
int Bullet::getDamage() const { return damage_; }
sf::RectangleShape Bullet::getSprite() const { return bullet_; }
sf::CircleShape Bullet::getLight() const { return light_; }
bool Bullet::isDead() { return deathClock_.getElapsedTime().asMilliseconds() > deathTime_; }
bool Bullet::isHit() { return hit_; }
sf::Vector2f Bullet::getLastPosition() { return lastPosition_; }
