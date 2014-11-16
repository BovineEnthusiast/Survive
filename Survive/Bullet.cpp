#include "Bullet.h"
#include <math.h>
#include <iostream>
Bullet::Bullet(const sf::Vector2f& startPos, const sf::Vector2f& velocity, const int damage)
:velocity_(velocity), damage_(damage)
{
    //Bullet visual description
    positionGlobal_ = startPos;
    bullet_.setSize(sf::Vector2f(5, 5));
    bullet_.setFillColor(sf::Color(255, 204, 49, 255));
    std::cout << "bullet created!" << std::endl;
}

void Bullet::update(const sf::Time& dT)
{
    positionGlobal_ += velocity_ * dT.asSeconds();
    bullet_.setPosition(positionGlobal_);
    bullet_.setRotation(atan2(velocity_.y, velocity_.x));
}

//Getters
sf::RectangleShape Bullet::getSprite() {return bullet_;}