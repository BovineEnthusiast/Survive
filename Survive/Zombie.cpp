#include "Zombie.h"
#include <iostream>
#include "Collision.h"
Zombie::Zombie(Player* player, sf::Texture* texture)
: Humanoid(texture), pPlayer_(player)
{
    finalSpeed_ = 3 + std::rand() % 20;
}
void Zombie::update(const sf::Time& dT)
{
    if(sqrt(pow(positionGlobal_.y - pPlayer_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pPlayer_->getPositionGlobal().x, 2)) > 50)
    {
        targetVector_ = pPlayer_->getPositionGlobal() - positionGlobal_;
        targetVector_ /= (float)sqrt(pow(targetVector_.x, 2) + pow(targetVector_.y, 2)); // Normalize
        velocity_ = targetVector_ * speed_;  
    }
   
    else
        velocity_ = sf::Vector2f(0,0);

    if(sqrt(pow(positionGlobal_.y - pPlayer_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pPlayer_->getPositionGlobal().x, 2)) <= 65.0f && !attacking_)
    {
            attackClock_.restart();
            attacking_ = true;
    }
    animate(dT);
    sf::Vector2f perpVector = sf::Vector2f(cos((rotationGlobal_ + 90.0f) * 3.14159265358f / 180.0f), sin((rotationGlobal_ + 90.0f)  * 3.14159265358 / 180.0f));
    sf::Vector2f forwardVector = sf::Vector2f(cos((rotationGlobal_) * 3.14159265358f / 180.0f), sin((rotationGlobal_)  * 3.14159265358 / 180.0f));
    
    headSprite_.setRotation(atan2(pPlayer_->getPositionGlobal().y - positionGlobal_.y, pPlayer_->getPositionGlobal().x - positionGlobal_.x) * 180.0f / 3.14159265358f);
    
    armLeftSprite_.setPosition(positionGlobal_ + forwardVector * 9.0f + forwardVector * (float)sin(armClock_.getElapsedTime().asSeconds() * 6 + armLeftVerticalOffset_) * 1.5f - perpVector * 12.0f);
    armRightSprite_.setPosition(positionGlobal_ + forwardVector * 11.0f + forwardVector * (float)sin(armClock_.getElapsedTime().asSeconds() * 6 + armRightVerticalOffset_) * 1.5f + perpVector * 12.0f);
    armLeftSprite_.setRotation(headSprite_.getRotation() + sin(armClock_.getElapsedTime().asSeconds() * 6 + armLeftHorizontalOffset_) * 3);
    armRightSprite_.setRotation(headSprite_.getRotation() + sin(armClock_.getElapsedTime().asSeconds() * 6 + armRightHorizontalOffset_) * 3);

    //Attack animation
    if(attacking_)
    {
        sf::Time sinTime = attackClock_.getElapsedTime();
        if(sinTime.asSeconds() < 0.35f)
        {
            armLeftSprite_.setPosition(armLeftSprite_.getPosition() + forwardVector * (float)sin(sinTime.asSeconds() * 8.164f) * 6.0f);
            armRightSprite_.setPosition(armRightSprite_.getPosition() + forwardVector * (float)sin(sinTime.asSeconds() * 8.164f) * 6.0f);
            armLeftSprite_.setRotation(armLeftSprite_.getRotation() + (float)sin(sinTime.asSeconds() * 8.164f) * 12.0f);
            armRightSprite_.setRotation(armRightSprite_.getRotation() - (float)sin(sinTime.asSeconds() * 8.164f) * 12.0f);
            
            if(!hit_ && sinTime.asSeconds() > 0.175f)
            {
                if(sqrt(pow(positionGlobal_.y - pPlayer_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pPlayer_->getPositionGlobal().x, 2)) <= 65.0f)
                {
                    pPlayer_->setHealth(pPlayer_->getHealth() - 10.0f);
                    pPlayer_->injure();
                }
                    hit_ = true;
            }
        }
        else
        {
            attacking_ = false;
            hit_ = false;
        }
    }
}

//Setters
void Zombie::setPosition(const sf::Vector2f& position) {positionGlobal_ = position;}

