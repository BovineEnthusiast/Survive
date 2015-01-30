#ifndef BARRICADE_H
#define BARRICADE_H
#include <SFML/Graphics.hpp>
#include "GameObject.h"
class Barricade : public GameObject
{
public:
	Barricade(const sf::Vector2f&, sf::Texture*);
	void update(const sf::Time&);

	//Getters
	bool isDead() const;
	bool isReadyToDelete() const;
	sf::Sprite getSprite() const;
private:
	//Visual stuff
	sf::Texture* pTexture_;
	sf::Sprite sprite_;

	bool dead_ = false;
	bool readyToDelete_ = false; //Lets all zombies that point to this clear their pointers

	int health_ = 100;
};


#endif