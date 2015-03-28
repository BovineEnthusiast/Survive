#include "Bullet.h"
#include <math.h>
#include <iostream>
#include "Collision.h"
#include "Humanoid.h"
#include "Zombie.h"
Bullet::Bullet(const bool rocket, const sf::Vector2f& startPos, const sf::Vector2f& velocity, const int damage)
	:rocket_(rocket), velocity_(velocity), damage_(damage)
{
	//Bullet visual description
	positionGlobal_ = startPos;
	lastPosition_ = positionGlobal_;
	bullet_.setRotation(atan2(velocity_.y, velocity_.x) / 3.14159265358 * 180);
	
	if (!rocket_)
	{
		bullet_.setSize(sf::Vector2f(15.0f, 3.3f));
		bullet_.setFillColor(sf::Color(255, 204, 49, 255));
		bullet_.setOrigin(bullet_.getLocalBounds().width / 2.0f, bullet_.getLocalBounds().height / 2.0f);

		//Critical
		if (std::rand() % 100 == 0)
		{
			damage_ *= 5;
			bullet_.setFillColor(sf::Color(255, 50, 50, 255));
		}
	}
	else
	{
		shake_ = 30.0f;
		bullet_.setSize(sf::Vector2f(20.0f, 6.0f));
		bullet_.setOrigin(10.0f, 3.0f);

		bullet_.setFillColor(sf::Color(50, 125, 50, 255));
		rocketEmitter_ = Emitter(false,
			positionGlobal_,
			true,
			1000,
			100,
			sf::Vector2f(5.0f, 5.0f),
			sf::Vector2f(25.0f, 25.0f),
			-10.0f,
			10.0f,
			0.0f,
			50.0f,
			-25.0f,
			1.0f,
			3.0f,
			sf::Color(200, 200, 155, 255),
			sf::Color(155, 155, 155, 255));
	}

	
}

void Bullet::update(const sf::Time& dT)
{
	lastPosition_ = positionGlobal_;
	positionGlobal_ += velocity_ * dT.asSeconds();
	bullet_.setPosition(positionGlobal_);
	if (rocket_)
	{
		float magnitude = (float)sqrt(velocity_.x * velocity_.x + velocity_.y * velocity_.y);
		sf::Vector2f normalizedVelocity(velocity_.x / magnitude, velocity_.y / magnitude);
		rocketEmitter_.update(dT);
		rocketEmitter_.setPositionGlobal(positionGlobal_ - normalizedVelocity * 10.0f);

		if (hit_)
		{
			positionGlobal_ = lastPosition_;
			bullet_.setPosition(positionGlobal_);
			explosionEmitter_.update(dT);
			
			
		}
	}

}

//Setters
void Bullet::setHit(bool hit) 
{
	hit_ = hit;
	if (rocket_)
	{
		rocketEmitter_.setParticlesToEmit(0);
		bullet_.setFillColor(sf::Color::Transparent);
		explosionEmitter_ = Emitter(true,
			positionGlobal_,
			true,
			300,
			3000,
			sf::Vector2f(5.0f, 5.0f),
			sf::Vector2f(50.0f, 50.0f),
			0.0f,
			360.0f,
			0.0f,
			850.0f,
			-3000.0f,
			0.75f,
			1.25f,
			sf::Color(255, 225, 50, 255),
			sf::Color(250, 250, 250, 255));

	}
}
void Bullet::setFromTurret(const bool isFromTurret) { fromTurret_ = isFromTurret; }
void Bullet::setColor(const sf::Color& color) { bullet_.setFillColor(color); }
void Bullet::setSize(const sf::Vector2f& size) { bullet_.setSize(size); }
void Bullet::setSpeed(const int speed) { speed_ = speed; }

//Getters
bool Bullet::isDead() const { return deathClock_.getElapsedTime().asMilliseconds() > deathTime_; }
bool Bullet::isFromTurret() const { return fromTurret_; }
bool Bullet::isHit() const { return hit_; }
bool Bullet::isRocket() const { return rocket_; }
Emitter Bullet::getExplosionEmitter() const { return explosionEmitter_; }
Emitter Bullet::getRocketEmitter() const { return rocketEmitter_; }

int Bullet::getDamage() const { return damage_; }


sf::RectangleShape Bullet::getSprite() const { return bullet_; }
sf::Vector2f Bullet::getLastPosition() { return lastPosition_; }
float Bullet::getShake()
{
	float shake = shake_;
	shake_ = 0;
	return shake;
}

