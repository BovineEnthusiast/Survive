#include "Humanoid.h"
#include <math.h>
#include <iostream>
Humanoid::Humanoid(sf::Texture* texture) 
{
    bodySpriteSheet_ = texture;
    
    //Assigns the texture and the texture rects
    legLeftSprite_.setTexture(*bodySpriteSheet_);
    legLeftSprite_.setTextureRect(sf::IntRect(34, 14, 15, 11));
    legRightSprite_.setTexture(*bodySpriteSheet_);
    legRightSprite_.setTextureRect(sf::IntRect(34, 14, 15, 11));
    armLeftSprite_.setTexture(*bodySpriteSheet_);
    armLeftSprite_.setTextureRect(sf::IntRect(34, 25, 27, 9));
    armRightSprite_.setTexture(*bodySpriteSheet_);
    armRightSprite_.setTextureRect(sf::IntRect(34, 25, 27, 9));
    headSprite_.setTexture(*bodySpriteSheet_);
    headSprite_.setTextureRect(sf::IntRect(0, 0, 34, 34));
    
    //Assigns their origins
    legLeftSprite_.setOrigin(7.5f, 5.5f);
    legRightSprite_.setOrigin(7.5f, 5.5f);
    armLeftSprite_.setOrigin(0.0f, 4.5f);
    armRightSprite_.setOrigin(0.0f, 4.5f);
    headSprite_.setOrigin(17.0f, 16.0f);
}

void Humanoid::animate(const sf::Time& dT)
{
    if(injureClock_.getElapsedTime().asSeconds() < 0.15f)
    {
        headSprite_.setColor(sf::Color(255, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100));
        armLeftSprite_.setColor(sf::Color(255, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100));
        armRightSprite_.setColor(sf::Color(255, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100));
        legLeftSprite_.setColor(sf::Color(255, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100));
        legRightSprite_.setColor(sf::Color(255, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100, 255 - sin(injureClock_.getElapsedTime().asSeconds() * 3.14f / 0.15f) * 100));
        speed_ = finalSpeed_ - 2;
    }
    else
    {
        headSprite_.setColor(sf::Color(255, 255, 255));
        armLeftSprite_.setColor(sf::Color(255, 255, 255));
        armRightSprite_.setColor(sf::Color(255, 255, 255));
        legLeftSprite_.setColor(sf::Color(255, 255, 255));
        legRightSprite_.setColor(sf::Color(255, 255, 255));
        speed_ = finalSpeed_;
    }
    //Sets global variables
    sf::Vector2f normalizedVelocity = velocity_ / (float)sqrt( velocity_.x * velocity_.x + velocity_.y * velocity_.y );
    
    //Moves the humanoid
    positionGlobal_ += velocity_ * dT.asSeconds() * 20.0f;
    
    //Checks to see if it was moved to an illegal tile
    std::string tile = pTiles->at((positionGlobal_.x - fmod(positionGlobal_.x, 50)) / 50).at((positionGlobal_.y - fmod(positionGlobal_.y, 50)) / 50).getType();
    if(tile == "shallowWater" || tile == "deepWater")
        positionGlobal_ -= velocity_ * dT.asSeconds() * 20.0f;
 
    if(velocity_ != sf::Vector2f(0.0f,0.0f))
        rotationGlobal_ = atan2(velocity_.y, velocity_.x) * 180 / 3.14159265358;
    
    //REMEMBER: normalized -1/(y/x)
    sf::Vector2f perpVec;
    if(velocity_.x != 0)
        perpVec = sf::Vector2f((velocity_.y / velocity_.x), -1);
    else
        perpVec = sf::Vector2f(1, 0);
    
    perpVec /= (float)sqrt(1 + perpVec.x * perpVec.x);
    
    legLeftSprite_.setPosition(positionGlobal_ - perpVec * 10.0f + (normalizedVelocity * (float)(sin(sinClock_.getElapsedTime().asSeconds() * 10) * 15)));
    legRightSprite_.setPosition(positionGlobal_ + perpVec * 10.0f + (normalizedVelocity * (float)(sin(sinClock_.getElapsedTime().asSeconds() * 10) * -15)));
    headSprite_.setPosition(positionGlobal_);
    
   
    headSprite_.setRotation(rotationGlobal_);
    legLeftSprite_.setRotation(rotationGlobal_);
    legRightSprite_.setRotation(rotationGlobal_);
}

void Humanoid::injure()
{
    injured_ = true;
    injureClock_.restart();
}

//Getters
sf::Sprite Humanoid::getLegLeftSprite() {return legLeftSprite_;}
sf::Sprite Humanoid::getLegRightSprite() {return legRightSprite_;}
sf::Sprite Humanoid::getArmLeftSprite() {return armLeftSprite_;}
sf::Sprite Humanoid::getArmRightSprite() {return armRightSprite_;}
sf::Sprite Humanoid::getHeadSprite() {return headSprite_;}
sf::Vector2f Humanoid::getVelocity() {return velocity_;}
int Humanoid::getHealth() {return health_;}

//Setters
void Humanoid::setVelocity(const sf::Vector2f& velocity) {velocity_ = velocity;}
void Humanoid::setHealth(const int health) {health_ = health;}