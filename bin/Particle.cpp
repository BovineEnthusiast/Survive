#include "Particle.h"
#include <iostream>
Particle::Particle(const sf::Vector2f& position,
				   const sf::Vector2f& startingSize,
				   const sf::Vector2f& endingSize,
				   const float direction,
				   const float speed,
				   const float speedAcceleration,
				   const float life,
				   const sf::Color& startingColor,
				   const sf::Color& endingColor)
				   :startingSize_(startingSize),
				   endingSize_(endingSize),
				   direction_(direction),
				   speed_(speed),
				   speedAcceleration_(speedAcceleration),
				   life_(life),
				   startingColor_(startingColor),
				   endingColor_(endingColor)
{
	positionGlobal_ = position;
	particle_.setSize(startingSize_);
	particle_.setRotation(direction);
	particle_.setFillColor(startingColor_);
	sizeDifference_ = endingSize_ - startingSize_;
	colorDifference_ = sf::Vector3f(endingColor_.r - startingColor_.r, endingColor_.g - startingColor_.g, endingColor_.b - startingColor_.b);
	directionVector_ = sf::Vector2f(cos(direction * 3.14f / 180.0f), sin(direction * 3.14f / 180.0f));
}

void Particle::update(const sf::Time& dT)
{
	//Percentage of life
	float timePassed = lifeClock_.getElapsedTime().asSeconds();
	float timePercent = timePassed / life_;

	//Checks if dead
	if (timePercent >= 1.0f)
		dead_ = true;

	//Size
	particle_.setSize(startingSize_ + timePercent * sizeDifference_);

	//Color
	sf::Vector3f currentColor = colorDifference_ * timePercent;
	sf::Color newColor(startingColor_ + sf::Color(currentColor.x, currentColor.y, currentColor.z, 0));
	newColor.a = 1.0f - 255.0f * timePercent;
	particle_.setFillColor(newColor);
	
	//Speed
	speed_ += speedAcceleration_ * dT.asSeconds();

	//Velocity
	velocity_ = speed_ * directionVector_;

	positionGlobal_ += velocity_ * dT.asSeconds() - emitterOffset_;

	particle_.setOrigin(particle_.getLocalBounds().width / 2.0f, particle_.getLocalBounds().height / 2.0f);
	std::cout << emitterOffset_.x << ", " << emitterOffset_.y << std::endl;
	particle_.setPosition(positionGlobal_);
}

//Getters
bool Particle::isDead() const { return dead_; }
sf::RectangleShape Particle::getParticle() const { return particle_; }

//Setters
void Particle::setEmitterOffset(const sf::Vector2f& offset) { emitterOffset_ = offset; }