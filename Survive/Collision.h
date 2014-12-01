#ifndef COLLISION_H
#define	COLLISION_H
#include <SFML/Graphics.hpp>
bool isColliding(const sf::Sprite&, const sf::Sprite&);
bool isColliding(const sf::Sprite& , const sf::RectangleShape& bulletNew, const sf::Vector2f& bulletOld);
#endif	

 