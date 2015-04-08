#ifndef PARTICLE_H
#define PARTICLE_H
#include "GameObject.h"

class Particle : public GameObject
{
public:
	Particle(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, const float, const float, const float, const float, const sf::Color&, const sf::Color&);
	void update(const sf::Time&);

	//Getters
	bool isDead() const;
	sf::RectangleShape getParticle() const;

	//Setters
	void setEmitterOffset(const sf::Vector2f&);
private:
	bool dead_ = false;

	sf::RectangleShape particle_;

	//Particle sizes
	sf::Vector2f startingSize_;
	sf::Vector2f endingSize_;
	sf::Vector2f sizeDifference_;

	//Rotation in degrees
	float direction_;
	sf::Vector2f directionVector_; //In rectangular

	//Speed in units/sec
	float speed_;

	//At what rate the speed accelerated/decelerates in units/sec
	float speedAcceleration_;

	//Length of life in seconds
	float life_;

	//Velocity
	sf::Vector2f velocity_;

	//Starting and ending color
	sf::Color startingColor_;
	sf::Color endingColor_;
	sf::Vector3f colorDifference_;

	//Used to manage life
	sf::Clock lifeClock_;

	//Last emitter position used to move the particles relative to the emitter
	sf::Vector2f emitterOffset_ = sf::Vector2f(0.0f, 0.0f);
};
#endif 
