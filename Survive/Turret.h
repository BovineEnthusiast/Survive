#ifndef TURRET_H
#define TURRET_H
#include <list>
#include <vector>
#include <array>
#include "ImageManager.h"
#include "GameObject.h"
#include "Bullet.h"
class Turret : public GameObject
{
public:
	Turret(const sf::Vector2f&, std::list<Bullet>*, ImageManager*);
	void update(const sf::Time&);
	void preUpdate(const std::vector<sf::Vector2f>&); //Horrible workaround to not being able to throw this into update

	//Getters
	int getHealth() const;
	sf::Sprite getBaseSprite() const;
	sf::Sprite getTurretSprite() const;

	//Setters
	void setHealth(const int);
private:
	//Stores all active bullets
	std::list<Bullet>* pLBullets_;
	sf::Texture* pBaseTexture_;
	sf::Texture* pTurretTexture_;
	sf::Sprite baseSprite_;
	sf::Sprite turretSprite_;
	std::vector<sf::Vector2f> vZomPositions_;
	int damage_ = 10;
	int bullets_ = 100;
	int health_ = 100;
	bool reloading_ = false;
	float firerate_ = 0.1;
	float reloadTime_ = 5.0f;
	//Clocks
	sf::Clock firerateClock_;
	sf::Clock reloadClock_;

};
#endif