#ifndef PLAYER_H
#define	PLAYER_H
#include "Humanoid.h"
#include <SFML/Graphics.hpp>
class Player : public Humanoid
{
public:
    Player();
    void update(const sf::Time&);
    bool windowFocus;
    //Getters
    sf::Sprite getLegLeftSprite();
    sf::Sprite getLegRightSprite();
    sf::Sprite getArmLeftSprite();
    sf::Sprite getArmRightSprite();
    sf::Sprite getHeadSprite();
    sf::Vector2f getVelocity();
    //Setters
    void setVelocity(const sf::Vector2f&);
private:
    sf::Clock rotation;
    
};

#endif	

