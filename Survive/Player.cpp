#include "Player.h"
#include "Humanoid.h"
#include <math.h>
#include <iostream>
Player::Player() 
{
    positionGlobal_ = sf::Vector2f(1000.0f, 1000.0f);
    
    //Default gun
    vGuns_.push_back(Gun("pistol"));
    vGuns_[0].setLocalPosition(sf::Vector2f(19,23));
}

void Player::update(const sf::Time& dT)
{ 
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
    
    headSprite_.setRotation(atan2(sf::Mouse::getPosition(*window).y - ((float)window->getSize().y / 2.0f ), sf::Mouse::getPosition(*window).x - (float)window->getSize().x / 2.0f) * 180 / 3.14159265358);

   
    
    //Updates current gun
    vGuns_[0].setPlayerHeadRotation(atan2(sf::Mouse::getPosition(*window).y - (float)window->getSize().y / 2.0f, sf::Mouse::getPosition(*window).x - (float)window->getSize().x / 2.0f) * 180 / 3.14159265358);
    vGuns_[0].setPlayerPosition(positionGlobal_);
    vGuns_[0].update(dT);
    
    float rotationRadians = headSprite_.getRotation() * 3.14159265358 / 180;
    sf::Vector2f rotationVector = sf::Vector2f(cos(rotationRadians), sin(rotationRadians));
    sf::Vector2f perpVec;
    if(rotationVector.x != 0)
        perpVec = sf::Vector2f((rotationVector.y / rotationVector.x), -1);
    else
        perpVec = sf::Vector2f(1, 0);
    
    perpVec /= (float)sqrt(1 + perpVec.x * perpVec.x);
    
    sf::Vector2f positionLeft = sf::Vector2f(perpVec * -23.0f);
    sf::Vector2f positionRight = sf::Vector2f(perpVec * 23.0f);
    armLeftSprite_.setPosition(vGuns_[0].getArmLeftPos());
    armLeftSprite_.setRotation(atan2((positionLeft - vGuns_[0].getArmLeftPos()).y, (positionLeft - vGuns_[0].getArmLeftPos()).x));
    armRightSprite_.setPosition(vGuns_[0].getArmLeftPos());
    armRightSprite_.setRotation(atan2((positionRight - vGuns_[0].getArmLeftPos()).y, (positionRight - vGuns_[0].getArmLeftPos()).x));
    std::cout << "Arm Right: " << armLeftSprite_.getPosition().x << " " << armLeftSprite_.getPosition().y << std::endl;
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