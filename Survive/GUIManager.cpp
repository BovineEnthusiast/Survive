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
	ammo_.setFont(font_);
	ammo_.setColor(sf::Color::Black);

	//Reload
	reloadOutOf_.setFillColor(sf::Color(23, 24, 25));
	reloadCurrent_.setFillColor(sf::Color(141, 191, 215));
	reloadCurrent_.setSize(sf::Vector2f(3.0f, 3.0f));
	reloadOutOf_.setSize(sf::Vector2f(3.0f, 3.0f));
	reloadCurrent_.setOrigin(sf::Vector2f(3.0f, 3.0f));
	reloadOutOf_.setOrigin(sf::Vector2f(3.0f, 3.0f));
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

	//Reload
	Gun gun = pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex());
	if (gun.isReloading())
		reloadCurrent_.setScale(windowSize.x * reloadSize_.x * (gun.getCurrentReloadTime().asSeconds() / gun.getReloadTime()), windowSize.y * reloadSize_.y);
	else
		reloadCurrent_.setScale(windowSize.x * reloadSize_.x * (gun.getCurrentBullets() / (float)gun.getBulletsPerMag()), windowSize.y * reloadSize_.y);

	reloadOutOf_.setScale(windowSize.x * reloadSize_.x, windowSize.y * reloadSize_.y);
	reloadCurrent_.setPosition(currentWindowPos.x + windowSize.x - windowSize.x * reloadOffset_.x, currentWindowPos.y + windowSize.y - windowSize.y * reloadOffset_.y);
	reloadOutOf_.setPosition(currentWindowPos.x + windowSize.x - windowSize.x * reloadOffset_.x, currentWindowPos.y + windowSize.y - windowSize.y * reloadOffset_.y);

	//Ammo
	ammo_.setString(std::to_string(pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex()).getCurrentBullets()) + " / " + std::to_string(pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex()).getTotalAmmo()));
	ammo_.setOrigin(ammo_.getLocalBounds().width / 2, ammo_.getLocalBounds().height / 2.0f);
	ammo_.setCharacterSize(0.75f * reloadOutOf_.getGlobalBounds().height);
	ammo_.setPosition(reloadOutOf_.getPosition().x - reloadOutOf_.getGlobalBounds().width / 2.0f, reloadOutOf_.getPosition().y - reloadOutOf_.getGlobalBounds().height / 2.0f);
   
}

//Getters
sf::RectangleShape GUIManager::getHealthOutOf() const {return healthOutOf_;}
sf::RectangleShape GUIManager::getHealthCurrent() const {return healthCurrent_;}
sf::RectangleShape GUIManager::getReloadOutOf() const { return reloadOutOf_; }
sf::RectangleShape GUIManager::getReloadCurrent() const { return reloadCurrent_; }
sf::Text GUIManager::getTotalAmmo() const { return ammo_; }