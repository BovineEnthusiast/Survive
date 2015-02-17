#ifndef BULLET_H
#define	BULLET_H
#include "GameObject.h"
#include <SFML/Graphics.hpp>
class Bullet : public GameObject
{
public:
    Bullet(const sf::Vector2f&, const sf::Vector2f&, const int);
    void update(const sf::Time&);
    
    //Setters
    void setHit(bool);
    
    //Getters
    sf::RectangleShape getSprite();
    int getDamage() const;
    bool isDead();
    bool isHit();
    sf::Vector2f getLastPosition();
 
private:
    sf::RectangleShape bullet_;
    sf::Vector2f velocity_;
    sf::Vector2f lastPosition_;
    int damage_ = 25;
    //Used to determine when it should die if it hits nothing
    int deathTime_ = 1250;
    sf::Clock deathClock_;
    bool hit_ = false;
};

#endif

