#ifndef TURRET_H
#define TURRET_H
#include <list>
#include <vector>
#include "GameObject.h"
#include "Bullet.h"
#include "Zombie.h"
class Turret : public GameObject
{
public:
	Turret(std::list<Bullet>*, std::vector<Zombie>);
	void update(const sf::Time&);
private:

};
#endif