#include "Zombie.h"

Zombie::Zombie(Player* player)
: pPlayer_(player)
{
    
}
void Zombie::update(const sf::Time& dT)
{
    targetVector_ = pPlayer_->getPositionGlobal() - positionGlobal_;
    targetVector_ /= (float)sqrt(pow(targetVector_.x, 2) + pow(targetVector_.y, 2)); // Normalize
    
    velocity_ = targetVector_;
}

//Setters
void Zombie::setPosition(const sf::Vector2f& position) {positionGlobal_ = position;}

