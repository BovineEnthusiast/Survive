#include "GUIManager.h"
#include <iostream>
#include "Humanoid.h"
#include "Player.h"

GUIManager::GUIManager(sf::RenderWindow* pWindow, Player* pPlayer) 
:pWindow_(pWindow), pPlayer_(pPlayer)
{
    if(!font_.loadFromFile("arial.ttf"))
        std::cout << "Failed to load font." << std::endl;
        
    //Health
    healthOutOf_.setFillColor(sf::Color(214, 34, 34));
    healthCurrent_.setFillColor(sf::Color(78, 213, 116));
    healthOutOf_.setSize(sf::Vector2f(3.0f,3.0f));
    healthCurrent_.setSize(sf::Vector2f(3.0f, 3.0f));
    healthOutOf_.setOrigin(0.0f, 3.0f);
    healthCurrent_.setOrigin(0.0f, 3.0f);
    
    //Ammo
    //currentAmmo_.setOrigin()
}
void GUIManager::update(const sf::Time& dT)
{
    sf::Vector2f windowSize = pWindow_->getView().getSize();
    sf::Vector2f currentWindowPos(pWindow_->getView().getCenter().x - windowSize.x / 2.0f, pWindow_->getView().getCenter().y - windowSize.y / 2.);
    //Health
    healthOutOf_.setScale(windowSize.x * healthSize_.x, windowSize.y * healthSize_.y);
    healthCurrent_.setScale(windowSize.x * healthSize_.x * (pPlayer_->getHealth() / 100.0f), windowSize.y * healthSize_.y);
    healthOutOf_.setPosition(currentWindowPos.x + windowSize.x * healthOffset_.x, currentWindowPos.y + windowSize.y - windowSize.y * healthOffset_.y);
    healthCurrent_.setPosition(currentWindowPos.x + windowSize.x * healthOffset_.x, currentWindowPos.y + windowSize.y - windowSize.y * healthOffset_.y);
    
    //Ammo 
    //currentAmmo_.setString(std::to_string(topPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex()).getCurrentBullets()));
   // totalAmmo_.setString(" / " + pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex()).getTotalAmmo());
   
}

//Getters
sf::RectangleShape GUIManager::getHealthOutOf() const {return healthOutOf_;}
sf::RectangleShape GUIManager::getHealthCurrent() const {return healthCurrent_;}
