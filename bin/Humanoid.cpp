#include "Humanoid.h"
#include <math.h>
#include <iostream>
Humanoid::Humanoid(sf::Texture* texture) 
{
    bodySpriteSheet_ = texture;
    
    //Assigns the texture and the texture rects
	legLeftSprite_.setTexture(*bodySpriteSheet_);
	legRightSprite_.setTexture(*bodySpriteSheet_);
	armLeftSprite_.setTexture(*bodySpriteSheet_);
	armRightSprite_.setTexture(*bodySpriteSheet_);
	headSprite_.setTexture(*bodySpriteSheet_);

	//Normal sized zombies
	if (bodySpriteSheet_->getSize().x <= 39)
	{
		legLeftSprite_.setTextureRect(sf::IntRect(22, 9, 10, 7));
		legRightSprite_.setTextureRect(sf::IntRect(22, 9, 10, 7));
		armLeftSprite_.setTextureRect(sf::IntRect(22, 16, 17, 6));
		armRightSprite_.setTextureRect(sf::IntRect(22, 16, 17, 6));
		headSprite_.setTextureRect(sf::IntRect(0, 0, 22, 22));

		//Assigns their origins
		legLeftSprite_.setOrigin(5.0f, 3.5f);
		legRightSprite_.setOrigin(5.0f, 3.5f);
		armLeftSprite_.setOrigin(0.0f, 3.0f);
		armRightSprite_.setOrigin(0.0f, 3.0f);
		headSprite_.setOrigin(11.0f, 11.0f);
	}
	else
	{
		legLeftSprite_.setTextureRect(sf::IntRect(35, 15, 16, 11));
		legRightSprite_.setTextureRect(sf::IntRect(35, 15, 16, 11));
		armLeftSprite_.setTextureRect(sf::IntRect(35, 26, 26, 9));
		armRightSprite_.setTextureRect(sf::IntRect(35, 26, 26, 9));
		headSprite_.setTextureRect(sf::IntRect(0, 0, 35, 35));

		//Assigns their origins
		legLeftSprite_.setOrigin(8.0f, 5.5f);
		legRightSprite_.setOrigin(8.0f, 5.5f);
		armLeftSprite_.setOrigin(0.0f, 4.5f);
		armRightSprite_.setOrigin(0.0f, 4.5f);
		headSprite_.setOrigin(17.5f, 17.5f);

	}
}
Humanoid::Humanoid(){};
void Humanoid::animate(const sf::Time& dT)
{
	float multiplier = (bodySpriteSheet_->getSize().x <= 39) ? 1.0f : 1.6f;

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
    sf::Vector2f normalizedVelocity(0.0f, 0.0f);
    
    if(velocity_ != sf::Vector2f(0.0f, 0.0f))
      {
	//Sets global variables
	normalizedVelocity = velocity_ / (float)sqrt( velocity_.x * velocity_.x + velocity_.y * velocity_.y );
	
	//Moves the humanoid
	positionGlobal_ += velocity_ * dT.asSeconds() * 13.5f;
      }
    if(velocity_ != sf::Vector2f(0.0f,0.0f))
        rotationGlobal_ = atan2(velocity_.y, velocity_.x) * 180 / 3.14159265358;
    
    //REMEMBER: normalized -1/(y/x)
    sf::Vector2f perpVec;
    if(velocity_.x != 0)
        perpVec = sf::Vector2f((velocity_.y / velocity_.x), -1);
    else
        perpVec = sf::Vector2f(1, 0);
    
    perpVec /= (float)sqrt(1 + perpVec.x * perpVec.x);
    
	legLeftSprite_.setPosition(positionGlobal_ - perpVec * 6.7f * multiplier + (normalizedVelocity * (float)(sin(sinClock_.getElapsedTime().asSeconds() * 10) * 10)) * multiplier);
	legRightSprite_.setPosition(positionGlobal_ + perpVec * 6.7f * multiplier + (normalizedVelocity * (float)(sin(sinClock_.getElapsedTime().asSeconds() * 10) * -10) * multiplier));
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
bool Humanoid::isInjured() { return injured_; }

//Setters
void Humanoid::setVelocity(const sf::Vector2f& velocity) {velocity_ = velocity;}
void Humanoid::setHealth(const int health) {health_ = health;}
void Humanoid::setRotationGlobal(const float rotation) { rotationGlobal_ = rotation; }
void Humanoid::setPositionGlobal(const sf::Vector2f& position) { positionGlobal_ = position; }