#include "Player.h"
#include "Humanoid.h"
#include <iostream>
Player::Player() 
{
    velocity_ = sf::Vector2f(1,0);
}

void Player::update(const sf::Time& dT)
{ 
    animate(dT);
    //velocity_ = sf::Vector2f((float)cos(rotation.getElapsedTime().asSeconds()) * 10, (float)sin(rotation.getElapsedTime().asSeconds()) * 10);
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
    
                
}
//Getters
sf::Sprite Player::getLegLeftSprite() {return legLeftSprite_;}
sf::Sprite Player::getLegRightSprite() {return legRightSprite_;}
sf::Sprite Player::getArmLeftSprite() {return armLeftSprite_;}
sf::Sprite Player::getArmRightSprite() {return armRightSprite_;}
sf::Sprite Player::getHeadSprite() {return headSprite_;}
sf::Vector2f Player::getVelocity() {return velocity_;}

//Setters 
void Player::setVelocity(const sf::Vector2f& velocity) {velocity_ = velocity;}
