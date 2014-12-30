#include "GUIManager.h"
#include <iostream>
#include "Humanoid.h"
#include "Player.h"

GUIManager::GUIManager(sf::RenderWindow* pWindow, Player* pPlayer, int* pWave, int* pZombiesAlive) 
	:pWindow_(pWindow), pPlayer_(pPlayer), pWave_(pWave), pZombiesAlive_(pZombiesAlive)
{
    if(!font_.loadFromFile("font.otf"))
        std::cout << "Failed to load font." << std::endl;
        
	//Wave
	waveText_.setFont(font_);
	zombiesText_.setFont(font_);
	waveText_.setColor(sf::Color(226, 232, 235));
	zombiesText_.setColor(sf::Color(226, 232, 235));
	waveBackground_.setFillColor(sf::Color(0, 0, 0, 50));
	waveBackground_.setSize(sf::Vector2f(1.0f, 1.0f));
	waveBackground_.setOrigin(sf::Vector2f(1.0f, 0.0f));

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

	//---------------------Store------------------------
	buyMenuBackground_.setSize(sf::Vector2f(1.0f, 1.0f));
	buyMenuBackground_.setOrigin(0.5f, 0.5f);
	exitButton_.setFillColor(sf::Color(255, 0, 55, 200));
	highlightBackground_.setFillColor(sf::Color::Transparent);
	storeText_.setFont(font_);
	storeText_.setColor(sf::Color(40, 42, 43));
	storeText_.setString("Store");
	pistolText_.setFont(font_);
	pistolText_.setColor(sf::Color(40, 42, 43));
	pistolText_.setString("Pistol");
	magnumText_.setFont(font_);
	magnumText_.setColor(sf::Color(40, 42, 43));
	magnumText_.setString("Magnum");
	shotgunText_.setFont(font_);
	shotgunText_.setColor(sf::Color(40, 42, 43));
	shotgunText_.setString("Shotgun");
	rifleText_.setFont(font_);
	rifleText_.setColor(sf::Color(40, 42, 43));
	rifleText_.setString("Rifle");
	rocketText_.setFont(font_);
	rocketText_.setColor(sf::Color(40, 42, 43));
	rocketText_.setString("Rocket Launcher");
}
void GUIManager::update(const sf::Time& dT)
{
	if (toggled_ && !sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		toggled_ = false;
	sf::Vector2f windowSize = pWindow_->getView().getSize();
	sf::Vector2f currentWindowPos(pWindow_->getView().getCenter().x - windowSize.x / 2.0f, pWindow_->getView().getCenter().y - windowSize.y / 2.);

	//Wave info
	waveBackground_.setPosition(currentWindowPos.x + windowSize.x, currentWindowPos.y);
	waveBackground_.setScale(windowSize.x * waveBackgroundSize_.x, windowSize.y * waveBackgroundSize_.y);
	waveText_.setString("Wave: " + std::to_string(*pWave_));
	waveText_.setOrigin(waveText_.getLocalBounds().width, 0.0f);
	waveText_.setPosition(currentWindowPos.x + windowSize.x - (waveTextOffset_.x * windowSize.x), currentWindowPos.y  + (waveTextOffset_.y * windowSize.y));
	waveText_.setCharacterSize(windowSize.y * waveTextSize_);
	zombiesText_.setString("Zombies: " + std::to_string(*pZombiesAlive_));
	zombiesText_.setOrigin(zombiesText_.getLocalBounds().width, 0.0f);
	zombiesText_.setPosition(currentWindowPos.x + windowSize.x - (zombiesTextOffset_.x * windowSize.x), currentWindowPos.y + (zombiesTextOffset_.y * windowSize.y));
	zombiesText_.setCharacterSize(windowSize.y * zombiesTextSize_);
	
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
   

	//Store
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && !toggled_)
	{
		toggled_ = true;
		if (storeOpen_)
			storeOpen_ = false;
		else
		{
			storeOpen_ = true;
			storeOpenClock_.restart();
		}
	}
	if (storeOpen_)
	{
		float time = storeOpenClock_.getElapsedTime().asSeconds();
		std::cout << time << std::endl;
		if (time > storeOpenTime_)
		{
			buyMenuBackground_.setScale(sf::Vector2f(storeSize_.x * windowSize.x, storeSize_.y * windowSize.y));
			buyMenuBackground_.setFillColor(sf::Color(226, 232, 235, 235));
			buyMenuBackground_.setPosition(pWindow_->getView().getCenter());
		}
		else
		{
			std::cout << "THIS" << std::endl;

			buyMenuBackground_.setScale(sf::Vector2f(storeSize_.x * windowSize.x * (time / storeOpenTime_), storeSize_.y * windowSize.y * (time / storeOpenTime_)));
			buyMenuBackground_.setFillColor(sf::Color(226, 232, 235, 235 * (time / storeOpenTime_)));
			buyMenuBackground_.setPosition(pWindow_->getView().getCenter());
		}
	}
	else
		buyMenuBackground_.setFillColor(sf::Color::Transparent);
}

//Getters
sf::RectangleShape GUIManager::getHealthOutOf() const {return healthOutOf_;}
sf::RectangleShape GUIManager::getHealthCurrent() const {return healthCurrent_;}
sf::RectangleShape GUIManager::getReloadOutOf() const { return reloadOutOf_; }
sf::RectangleShape GUIManager::getReloadCurrent() const { return reloadCurrent_; }
sf::Text GUIManager::getTotalAmmo() const { return ammo_; }
sf::RectangleShape GUIManager::getWaveBackground() const { return waveBackground_; }
sf::Text GUIManager::getWaveText() const { return waveText_; }
sf::Text GUIManager::getZombiesText() const { return zombiesText_; }
sf::RectangleShape GUIManager::getBuyMenuBackground() const { return buyMenuBackground_; }