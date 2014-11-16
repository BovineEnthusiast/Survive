#ifndef BULLET_H
#define	BULLET_H
#include "GameObject.h"
#include <SFML/Graphics.hpp>
class Bullet : GameObject
{
public:
    Bullet(const sf::Vector2f&, const sf::Vector2f&, const int);
    void update(const sf::Time&);
    
    //Getters
    sf::RectangleShape getSprite();
    
    //For later
    //std::vector<Zombie>* pZombies;
private:
    sf::RectangleShape bullet_;
    sf::Vector2f velocity_;
    int damage_ = 25;
};

#endif

