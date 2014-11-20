#include "Player.h"
#include "Humanoid.h"
#include <math.h>
#include <iostream>
Player::Player() 
{
    positionGlobal_ = sf::Vector2f(1000.0f, 1000.0f);
    
    //Default gun
    vGuns_.push_back(Gun("pistol"));
    vGuns_[0].setLocalPosition(sf::Vector2f(30, 10));
    
    armLeftSprite_.setOrigin(armLeftSprite_.getLocalBounds().width / 2, 0);
    armRightSprite_.setOrigin(armLeftSprite_.getLocalBounds().width / 2, 0);
}

void Player::update(const sf::Time& dT)
{ 
    vGuns_[0].window = window;
    animate(dT);
    
    velocity_ = sf::Vector2f(0,0);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        velocity_ += sf::Vector2f(0, -1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        velocity_ += sf::Vector2f(-1, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        velocity_ += sf::Vector2f(0, 1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        velocity_ += sf::Vector2f(1, 0);
    if(velocity_ != sf::Vector2f(0,0))
        velocity_ /= (float)sqrt( velocity_.x * velocity_.x + velocity_.y * velocity_.y);
    
    velocity_ *= (float)10;
    
    headSprite_.setRotation(atan2((sf::Mouse::getPosition(*window).y + (((float)window->getView().getCenter().y) - ((float)window->getSize().y / 2.0f))) - positionGlobal_.y, (sf::Mouse::getPosition(*window).x + (((float)window->getView().getCenter().x) - ((float)window->getSize().x / 2.0f))) - positionGlobal_.x) * 180 / 3.14159265358);

   
    
    //Updates current gun
    vGuns_[0].setPlayerHeadRotation(atan2(sf::Mouse::getPosition(*window).y - (float)window->getSize().y / 2.0f, sf::Mouse::getPosition(*window).x - (float)window->getSize().x / 2.0f) * 180 / 3.14159265358);
    vGuns_[0].setPlayerPosition(positionGlobal_);
    vGuns_[0].setPlayerVelocity(velocity_);
    vGuns_[0].update(dT);
    
    float rotationRadians = (headSprite_.getRotation() + 90) * 3.14159265358 / 180;
    sf::Vector2f perpVec = sf::Vector2f(cos(rotationRadians), sin(rotationRadians));
    
    sf::Vector2f armRotationVecLeft = (positionGlobal_ + sf::Vector2f(perpVec * 11.5f)) - (vGuns_[0].getArmLeftPos() + vGuns_[0].getPositionGlobal());
    sf::Vector2f armRotationVecRight = (positionGlobal_ - sf::Vector2f(perpVec * 11.5f)) - (vGuns_[0].getArmRightPos() + vGuns_[0].getPositionGlobal());
    armLeftSprite_.setPosition(vGuns_[0].getArmLeftPos() + vGuns_[0].getPositionGlobal());
    armRightSprite_.setPosition(vGuns_[0].getArmRightPos() + vGuns_[0].getPositionGlobal());
    armLeftSprite_.setRotation(atan2(armRotationVecLeft.y, armRotationVecLeft.x) / 3.14159265358 * 180 -90);
    armRightSprite_.setRotation(atan2(armRotationVecRight.y, armRotationVecRight.x) / 3.14159265358 * 180 -90);
}
//Getters
sf::Sprite Player::getLegLeftSprite() {return legLeftSprite_;}
sf::Sprite Player::getLegRightSprite() {return legRightSprite_;}
sf::Sprite Player::getArmLeftSprite() {return armLeftSprite_;}
sf::Sprite Player::getArmRightSprite() {return armRightSprite_;}
sf::Sprite Player::getHeadSprite() {return headSprite_;}
sf::Vector2f Player::getVelocity() {return velocity_;}
std::vector<Gun> Player::getGuns() {return vGuns_;}

//Setters 
void Player::setVelocity(const sf::Vector2f& velocity) {velocity_ = velocity;}
void Player::setPosition(const sf::Vector2f& position) {positionGlobal_ = position;}