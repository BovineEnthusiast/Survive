#ifndef Ray_H
#define Ray_H
#include <SFML/Graphics.hpp>
class Ray
{
public:
	Ray();
	Ray(const sf::Vector2f&, const float);
	Ray(const sf::Vector2f&, const sf::Vector2f&);

	//Getters
	sf::Vector2f getDirection() const;
	float getAngle() const;

	//Operator overloading
	bool operator< (const Ray&) const;

private:
	sf::Vector2f direction_ = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f start_ = sf::Vector2f(0.0f, 0.0f);
	float angle_ = 0; //Radians
};
#endif