#include "Player.h"
#include "Humanoid.h"
#include <math.h>
#include <iostream>
Player::Player(sf::Texture* texture, ImageManager* pImageManager, SoundManager* pSoundManager)
:Humanoid(texture), pSoundManager_(pSoundManager)
{
    finalSpeed_ = 20;
    
    //Pushes the guns
    vGuns_.push_back(Gun("pistol", pImageManager, pSoundManager));
    vGuns_.push_back(Gun("rifle", pImageManager, pSoundManager));
    
    armLeftSprite_.setOrigin(armLeftSprite_.getLocalBounds().width, 4.5f);
    armRightSprite_.setOrigin(armLeftSprite_.getLocalBounds().width, 4.5f);
}

void Player::update(const sf::Time& dT)
{ 
    
    
    if(health_ > 0)
    {
        //Switches out the gun if number is pressed
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            currentGun_ = 0;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            currentGun_ = 1;
        
        vGuns_[currentGun_].window = window;

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

        velocity_ *= speed_;
        animate(dT);
        
        //Footsteps
        if(footstepClock_.getElapsedTime().asSeconds() > 0.15f && velocity_ != sf::Vector2f(0.0f, 0.0f))
        {
            footstepClock_.restart();
            pSoundManager_->playSound("grass");
        }

        headSprite_.setRotation(atan2((sf::Mouse::getPosition(*window).y + (((float)window->getView().getCenter().y) - ((float)window->getSize().y / 2.0f))) - positionGlobal_.y, (sf::Mouse::getPosition(*window).x + (((float)window->getView().getCenter().x) - ((float)window->getSize().x / 2.0f))) - positionGlobal_.x) * 180 / 3.14159265358);



        //Updates current gun
        vGuns_[currentGun_].setPlayerHeadRotation(atan2(sf::Mouse::getPosition(*window).y - (float)window->getSize().y / 2.0f, sf::Mouse::getPosition(*window).x - (float)window->getSize().x / 2.0f) * 180 / 3.14159265358);
        vGuns_[currentGun_].setPlayerPosition(positionGlobal_);
        vGuns_[currentGun_].setPlayerVelocity(velocity_);
        vGuns_[currentGun_].update(dT);

        float rotationRadians = (headSprite_.getRotation() + 90) * 3.14159265358 / 180;
        sf::Vector2f perpVec = sf::Vector2f(cos(rotationRadians), sin(rotationRadians));

        sf::Vector2f armRotationVecLeft = (positionGlobal_ - sf::Vector2f(perpVec * 11.5f)) - armLeftSprite_.getPosition();
        sf::Vector2f armRotationVecRight = (positionGlobal_ + sf::Vector2f(perpVec * 11.5f)) - armRightSprite_.getPosition(); 
        armLeftSprite_.setPosition(vGuns_[currentGun_].getArmLeftPos() + vGuns_[currentGun_].getPositionGlobal());
        armRightSprite_.setPosition(vGuns_[currentGun_].getArmRightPos() + vGuns_[currentGun_].getPositionGlobal());
        armLeftSprite_.setRotation(atan2(armRotationVecLeft.y, armRotationVecLeft.x) / 3.14159265358 * 180 + 180.0f);
        armRightSprite_.setRotation(atan2(armRotationVecRight.y, armRotationVecRight.x) / 3.14159265358 * 180 + 180.0f);
    }
}

//Getters
std::vector<Gun> Player::getGuns() {return vGuns_;}
int Player::getCurrentGunIndex() const {return currentGun_;}

//Setters 
void Player::setPosition(const sf::Vector2f& position) {positionGlobal_ = position;}
void Player::setGunBulletPointers(std::list<Bullet>* pointer)
{
    for(auto iGun = vGuns_.begin(); iGun != vGuns_.end(); ++iGun)
        iGun->setBulletsPtr(pointer);
}
