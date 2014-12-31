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
	selectionRect_.setFillColor(sf::Color::Transparent);
	setUpText(storeText_, "Store");
	
	setUpText(pistolText_, "Pistol");
	setUpText(magnumText_, "Magnum");
	setUpText(shotgunText_, "Shotgun");
	setUpText(rifleText_, "Rifle");
	setUpText(rocketText_, "Rocket");
	setUpText(barricadeText_, "Barricade");
	setUpText(turretText_, "Turret");
	setUpText(rocketTurretText_, "Rocket Turret");
	setUpText(mineText_, "Mine");
	setUpText(buyMagnumText_, "Buy");
	setUpText(buyShotgunText_, "Buy");
	setUpText(buyRifleText_, "Buy");
	setUpText(buyRocketText_, "Buy");
	setUpText(buyBarricadeText_, "Buy");
	setUpText(buyTurretText_, "Buy");
	setUpText(buyRocketTurretText_, "Buy");
	setUpText(buyMineText_, "Buy");
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
		if (time > storeOpenTime_)
		{
			buyMenuBackground_.setScale(sf::Vector2f(storeSize_.x * windowSize.x, storeSize_.y * windowSize.y));
			buyMenuBackground_.setFillColor(sf::Color(226, 232, 235, 235));
			buyMenuBackground_.setPosition(pWindow_->getView().getCenter());

			sf::Vector2f size(buyMenuBackground_.getGlobalBounds().width, buyMenuBackground_.getGlobalBounds().height);
			sf::Vector2f pos = buyMenuBackground_.getPosition();
			showInStore(pistolText_, 1.0f, 4.0f, itemTextSize_);
			showInStore(magnumText_, 2.0f, 4.0f, itemTextSize_);
			showInStore(shotgunText_, 3.0f, 4.0f, itemTextSize_);
			showInStore(rifleText_, 4.0f, 4.0f, itemTextSize_);
			showInStore(rocketText_, 5.0f, 4.0f, itemTextSize_);
			showInStore(barricadeText_, 6.0f, 4.0f, itemTextSize_);
			showInStore(turretText_, 7.0f, 4.0f, itemTextSize_);
			showInStore(rocketTurretText_, 8.0f, 4.0f, itemTextSize_);
			showInStore(mineText_, 9.0f, 4.0f, itemTextSize_);
			showInStore(buyMagnumText_, 2.0f, 8.0f, itemTextSize_);
			showInStore(buyShotgunText_, 3.0f, 8.0f, itemTextSize_);
			showInStore(buyRifleText_, 4.0f, 8.0f, itemTextSize_);
			showInStore(buyRocketText_, 5.0f, 8.0f, itemTextSize_);
			showInStore(buyBarricadeText_, 6.0f, 8.0f, itemTextSize_);
			showInStore(buyTurretText_, 7.0f, 8.0f, itemTextSize_);
			showInStore(buyRocketTurretText_, 8.0f, 8.0f, itemTextSize_);
			showInStore(buyMineText_, 9.0f, 8.0f, itemTextSize_);

			if (hover(buyMagnumText_))
			{

			}
			else if (hover(buyShotgunText_))
			{

			}
			else if (hover(buyRifleText_))
			{

			}
			else if (hover(buyRocketText_))
			{

			}
			else if (hover(buyBarricadeText_))
			{

			}
			else if (hover(buyTurretText_))
			{

			}
			else if (hover(buyRocketTurretText_))
			{

			}
			else if (hover(buyMineText_))
			{

			}
			else
				selectionRect_.setFillColor(sf::Color::Transparent);

		}
		else
		{
			
			buyMenuBackground_.setScale(sf::Vector2f(storeSize_.x * windowSize.x * (time / storeOpenTime_), storeSize_.y * windowSize.y * (time / storeOpenTime_)));
			buyMenuBackground_.setFillColor(sf::Color(226, 232, 235, 235 * (time / storeOpenTime_)));
			buyMenuBackground_.setPosition(pWindow_->getView().getCenter());
		}
	}
	else
	{
		buyMenuBackground_.setFillColor(sf::Color::Transparent);
		pistolText_.setColor(sf::Color::Transparent);
		magnumText_.setColor(sf::Color::Transparent);
		shotgunText_.setColor(sf::Color::Transparent);
		rifleText_.setColor(sf::Color::Transparent);
		rocketText_.setColor(sf::Color::Transparent);
		barricadeText_.setColor(sf::Color::Transparent);
		turretText_.setColor(sf::Color::Transparent);
		rocketTurretText_.setColor(sf::Color::Transparent);
		mineText_.setColor(sf::Color::Transparent);
		buyMagnumText_.setColor(sf::Color::Transparent);
		buyShotgunText_.setColor(sf::Color::Transparent);
		buyRifleText_.setColor(sf::Color::Transparent);
		buyRocketText_.setColor(sf::Color::Transparent);
		buyBarricadeText_.setColor(sf::Color::Transparent);
		buyTurretText_.setColor(sf::Color::Transparent);
		buyRocketTurretText_.setColor(sf::Color::Transparent);
		buyMineText_.setColor(sf::Color::Transparent);
		selectionRect_.setFillColor(sf::Color::Transparent);
	}
}

//Helpers
void GUIManager::setUpText(sf::Text& text, const std::string& string)
{
	text.setFont(font_);
	text.setColor(sf::Color(40, 42, 43));
	text.setString(string);
	text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2);
}
void GUIManager::showInStore(sf::Text& text, const int x, const int y, const float charSize)
{
	sf::Vector2f size(buyMenuBackground_.getGlobalBounds().width, buyMenuBackground_.getGlobalBounds().height);
	sf::Vector2f pos = buyMenuBackground_.getPosition();
	text.setPosition(sf::Vector2f(pos.x - (size.x / 2.0f) + (size.x / 10.0f) * x, pos.y - size.y / y));
	text.setCharacterSize(charSize * size.y);
	text.setColor(sf::Color(40, 42, 43));
}
bool GUIManager::hover(const sf::Text& text)
{
	sf::View view = pWindow_->getView();
	sf::Vector2f windowSize = view.getSize();
	sf::Vector2f currentWindowPos(view.getCenter().x - windowSize.x / 2.0f, view.getCenter().y - windowSize.y / 2.0f);
	float ratio = view.getSize().y / pWindow_->getSize().y;

	if (text.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_) * ratio + currentWindowPos))
	{
		sf::FloatRect bounds = text.getGlobalBounds();
		selectionRect_.setFillColor(sf::Color(255, 0, 55, 200));
		selectionRect_.setSize(sf::Vector2f(bounds.width + bounds.width * 0.75f, bounds.height + bounds.height * 0.25f));
		selectionRect_.setPosition(text.getGlobalBounds().left - text.getGlobalBounds().width * 0.375f, text.getGlobalBounds().top - text.getGlobalBounds().height * 0.175f);
		return true;
	}
	else
		return false;
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
sf::RectangleShape GUIManager::getSelectionRect() const { return selectionRect_; }
sf::Text GUIManager::getPistolText() const { return pistolText_; }
sf::Text GUIManager::getMagnumText() const { return magnumText_; }
sf::Text GUIManager::getShotgunText() const { return shotgunText_; }
sf::Text GUIManager::getRifleText() const { return rifleText_; }
sf::Text GUIManager::getRocketText() const { return rocketText_; }
sf::Text GUIManager::getBarricadeText() const { return barricadeText_; }
sf::Text GUIManager::getTurretText() const { return turretText_; }
sf::Text GUIManager::getRocketTurretText() const { return rocketTurretText_; }
sf::Text GUIManager::getMineText() const { return mineText_; }
sf::Text GUIManager::getBuyMagnumText() const { return buyMagnumText_; }
sf::Text GUIManager::getBuyShotgunText() const { return buyShotgunText_; }
sf::Text GUIManager::getBuyRifleText() const { return buyRifleText_; }
sf::Text GUIManager::getBuyRocketText() const { return buyRocketText_; }
sf::Text GUIManager::getBuyBarricadeText() const { return buyBarricadeText_; }
sf::Text GUIManager::getBuyTurretText() const {	return buyTurretText_;}
sf::Text GUIManager::getBuyRocketTurretText() const { return buyRocketTurretText_; }
sf::Text GUIManager::getBuyMineText() const { return buyMineText_; }