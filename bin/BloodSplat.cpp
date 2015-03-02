#include "BloodSplat.h"
#include <iostream>

BloodSplat::BloodSplat(sf::Texture* texture) 
{
    pBloodSplatTexture_ = texture;
    bloodSplatSprite_.setTexture(*pBloodSplatTexture_);
    bloodSplatSprite_.setColor(color_);
    bloodSplatSprite_.setOrigin(25.0f,25.0f);
    bloodSplatSprite_.setRotation(std::rand() % 361);
	bloodSplatSprite_.setScale(0.67f, 0.67f);
}
void BloodSplat::update(const sf::Time&)
{
    bloodSplatSprite_.setPosition(positionGlobal_);
    if(lifeClock_.getElapsedTime().asMilliseconds() > fadeAfter_)
    {
        color_ = sf::Color(color_.r,0,0, 150 - ((lifeClock_.getElapsedTime().asMilliseconds() - fadeAfter_) / fadeFor_ * 150));
        bloodSplatSprite_.setColor(color_);
        if(lifeClock_.getElapsedTime().asMilliseconds() > fadeAfter_ + fadeFor_)
            faded_ = true;
    }
}

//Getters
bool BloodSplat::isFaded() const {return faded_;}
sf::Sprite BloodSplat::getSprite() const {return bloodSplatSprite_;}
