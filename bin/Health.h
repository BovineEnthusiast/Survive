#ifndef HEALTH_H
#define HEALTH_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
class Health : public GameObject
{
public:
	Health(sf::Texture*);
	void update(const sf::Time&);

	//Getters
	sf::Sprite getSprite() const;
	sf::Clock getDurationClock() const;
	bool isCollected() const;

	//Setters
	void setCollected(const bool);
private:
	sf::Texture* pTexture_;
	sf::Sprite sprite_;
	bool collected_ = false;
	sf::Clock durationClock_;
};
#endif //HEALTH_H