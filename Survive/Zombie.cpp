#include "Zombie.h"
#include <iostream>
#include "Collision.h"
Zombie::Zombie(Player* player)
: pPlayer_(player)
{
    bodySpriteSheet_.loadFromFile("zombie.png");
}
void Zombie::update(const sf::Time& dT)
{
    if(sqrt(pow(positionGlobal_.y - pPlayer_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pPlayer_->getPositionGlobal().x, 2)) > 50)
    {
        targetVector_ = pPlayer_->getPositionGlobal() - positionGlobal_;
        targetVector_ /= (float)sqrt(pow(targetVector_.x, 2) + pow(targetVector_.y, 2)); // Normalize
        velocity_ = targetVector_;  
    }
    else
        velocity_ = sf::Vector2f(0,0);
    
    animate(dT);

    headSprite_.setRotation(atan2(pPlayer_->getPositionGlobal().y - positionGlobal_.y, pPlayer_->getPositionGlobal().x - positionGlobal_.x) * 180 / 3.14159265358);
}

//Setters
void Zombie::setPosition(const sf::Vector2f& position) {positionGlobal_ = position;}

