#include "Ray.h"

//Constructors
Ray::Ray(){}
Ray::Ray(const sf::Vector2f& start, const float angle)
	:angle_(angle)
{
	direction_ = sf::Vector2f(cos(angle_), sin(angle_));
}
Ray::Ray(const sf::Vector2f& direction, const sf::Vector2f& start)
	:direction_(direction), start_(start)
{
	//Normalize direction
	direction_ /= sqrt(direction.x * direction.x + direction.y * direction.y);

	angle_ = atan2(direction.y, direction.x);
}

//Getters
sf::Vector2f Ray::getDirection() const { return direction_; }
float Ray::getAngle() const { return angle_; }

//Operator overloading
bool Ray::operator< (const Ray& otherPoint) const { return (angle_ < otherPoint.angle_); }