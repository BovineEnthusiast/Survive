#include "GUIManager.h"
#include <iostream>
#include "Humanoid.h"
#include "Player.h"

GUIManager::GUIManager(ImageManager* pImageManager, sf::RenderWindow* pWindow, Player* pPlayer, int* pWave, int* pZombiesAlive)
	:pImageManager_(pImageManager), pWindow_(pWindow), pPlayer_(pPlayer), pWave_(pWave), pZombiesAlive_(pZombiesAlive)
{
	if (!font_.loadFromFile("assets/fonts/font.otf"))
		std::cout << "Failed to load font." << std::endl;

	reset();

}
void GUIManager::update(const sf::Time& dT)
{
	if (clickDown_ && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
		clickDown_ = false;

	if (toggled_ && !sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		toggled_ = false;
	sf::Vector2f viewSize(pWindow_->getView().getSize());
	sf::Vector2u windowSize(pWindow_->getSize());
	sf::Vector2f currentWindowPos(pWindow_->getView().getCenter().x - viewSize.x / 2.0f, pWindow_->getView().getCenter().y - viewSize.y / 2.0f);

	//Wave info
	waveBackground_.setPosition(currentWindowPos.x + viewSize.x, currentWindowPos.y);
	waveBackground_.setScale(viewSize.x * waveBackgroundSize_.x, viewSize.y * waveBackgroundSize_.y);
	waveText_.setString("Wave: " + std::to_string(*pWave_));
	waveText_.setOrigin(waveText_.getLocalBounds().width, 0.0f);
	waveText_.setPosition(currentWindowPos.x + viewSize.x - (waveTextOffset_.x * viewSize.x), currentWindowPos.y + (waveTextOffset_.y * viewSize.y));
	waveText_.setCharacterSize((int)(windowSize.y * waveTextSize_));
	waveText_.setScale(0.5f, 0.5f);
	zombiesText_.setString("Zombies: " + std::to_string(*pZombiesAlive_));
	zombiesText_.setOrigin(zombiesText_.getLocalBounds().width, 0.0f);
	zombiesText_.setPosition(currentWindowPos.x + viewSize.x - (zombiesTextOffset_.x * viewSize.x), currentWindowPos.y + (zombiesTextOffset_.y * viewSize.y));
	zombiesText_.setCharacterSize((int)(windowSize.y * zombiesTextSize_));
	zombiesText_.setScale(0.5f, 0.5f);
	pointsText_.setString("Points: " + std::to_string(pPlayer_->getPoints()));
	pointsText_.setOrigin(pointsText_.getLocalBounds().width, 0.0f);
	pointsText_.setPosition(currentWindowPos.x + viewSize.x - (pointsTextOffset_.x * viewSize.x), currentWindowPos.y + (pointsTextOffset_.y * viewSize.y));
	pointsText_.setCharacterSize((int)(windowSize.y * pointsTextSize_));
	pointsText_.setScale(0.5f, 0.5f);

	//Health  
	healthOutOf_.setScale(viewSize.x * healthSize_.x, viewSize.y * healthSize_.y);
	healthCurrent_.setScale(viewSize.x * healthSize_.x * (pPlayer_->getHealth() / 100.0f), viewSize.y * healthSize_.y);
	healthOutOf_.setPosition(currentWindowPos.x + viewSize.x * healthOffset_.x, currentWindowPos.y + viewSize.y - viewSize.y * healthOffset_.y);
	healthCurrent_.setPosition(currentWindowPos.x + viewSize.x * healthOffset_.x, currentWindowPos.y + viewSize.y - viewSize.y * healthOffset_.y);

	//Sprint
	sprintOutOf_.setScale(viewSize.x * healthSize_.x, viewSize.y * healthSize_.y);
	sprintCurrent_.setScale(viewSize.x * healthSize_.x * (pPlayer_->getSprint() / 3.0f), viewSize.y * healthSize_.y);
	sprintOutOf_.setPosition(currentWindowPos.x + viewSize.x * sprintOffset_.x, currentWindowPos.y + viewSize.y - viewSize.y * sprintOffset_.y);
	sprintCurrent_.setPosition(currentWindowPos.x + viewSize.x * sprintOffset_.x, currentWindowPos.y + viewSize.y - viewSize.y * sprintOffset_.y);

	//Reload
	Gun gun = pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex());
	if (gun.isReloading())
		reloadCurrent_.setScale(viewSize.x * reloadSize_.x * (gun.getCurrentReloadTime().asSeconds() / gun.getReloadTime()), viewSize.y * reloadSize_.y);
	else
		reloadCurrent_.setScale(viewSize.x * reloadSize_.x * (gun.getCurrentBullets() / (float)gun.getBulletsPerMag()), viewSize.y * reloadSize_.y);

	reloadOutOf_.setScale(viewSize.x * reloadSize_.x, viewSize.y * reloadSize_.y);
	reloadCurrent_.setPosition(currentWindowPos.x + viewSize.x - viewSize.x * reloadOffset_.x, currentWindowPos.y + viewSize.y - viewSize.y * reloadOffset_.y);
	reloadOutOf_.setPosition(currentWindowPos.x + viewSize.x - viewSize.x * reloadOffset_.x, currentWindowPos.y + viewSize.y - viewSize.y * reloadOffset_.y);

	//Ammo
	ammo_.setString(std::to_string(pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex()).getCurrentBullets()) + " / " + std::to_string(pPlayer_->getGuns().at(pPlayer_->getCurrentGunIndex()).getTotalAmmo()));
	ammo_.setOrigin(ammo_.getLocalBounds().width / 2, ammo_.getLocalBounds().height / 2.0f);
	ammo_.setCharacterSize((int)(0.75f * reloadOutOf_.getGlobalBounds().height * 2));
	ammo_.setScale(0.5f, 0.5f);
	ammo_.setPosition(reloadOutOf_.getPosition().x - reloadOutOf_.getGlobalBounds().width / 2.0f, reloadOutOf_.getPosition().y - reloadOutOf_.getGlobalBounds().height / 2.0f);


	//Store
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B) || (storeOpen_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) && !toggled_)
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
			buyMenuBackground_.setScale(sf::Vector2f(storeSize_.x * viewSize.x, storeSize_.y * viewSize.y));
			buyMenuBackground_.setFillColor(sf::Color(226, 232, 235, 235));
			buyMenuBackground_.setPosition(pWindow_->getView().getCenter());

			sf::Vector2f size(buyMenuBackground_.getGlobalBounds().width, buyMenuBackground_.getGlobalBounds().height);
			sf::Vector2f pos = buyMenuBackground_.getPosition();
			showInStore(storeText_, 5.0f, 2.2f, itemTextSize_ * 2);

			showInStore(pistolSprite_, 1.5f, 10.0f);
			showInStore(magnumSprite_, 2.5f, 10.0f);
			showInStore(shotgunSprite_, 3.5f, 10.0f);
			showInStore(rifleSprite_, 4.5f, 10.0f);
			showInStore(rocketSprite_, 5.5f, 10.0f);
			showInStore(barricadeSprite_, 6.5f, 10.0f);
			showInStore(turretSprite_, 7.5f, 10.0f);
			showInStore(mineSprite_, 8.5f, 10.0f);

			showInStore(pistolText_, 1.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(magnumText_, 2.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(shotgunText_, 3.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(rifleText_, 4.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(rocketText_, 5.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(barricadeText_, 6.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(turretText_, 7.5f, 3.0f, itemTextSize_ * 1.25f);
			showInStore(mineText_, 8.5f, 3.0f, itemTextSize_ * 1.25f);

			showInStore(buyMagnumText_, 2.5f, -8.0f, itemTextSize_);
			showInStore(buyShotgunText_, 3.5f, -8.0f, itemTextSize_);
			showInStore(buyRifleText_, 4.5f, -8.0f, itemTextSize_);
			showInStore(buyRocketText_, 5.5f, -8.0f, itemTextSize_);
			showInStore(buyBarricadeText_, 6.5f, -8.0f, itemTextSize_);
			showInStore(buyTurretText_, 7.5f, -8.0f, itemTextSize_);
			showInStore(buyMineText_, 8.5f, -8.0f, itemTextSize_);

			showInStore(buyPistolAmmoText_, 1.5f, -4.0f, itemTextSize_ / 1.5f);
			showInStore(buyMagnumAmmoText_, 2.5f, -4.0f, itemTextSize_ / 1.5f);
			showInStore(buyShotgunAmmoText_, 3.5f, -4.0f, itemTextSize_ / 1.5f);
			showInStore(buyRifleAmmoText_, 4.5f, -4.0f, itemTextSize_ / 1.5f);
			showInStore(buyRocketAmmoText_, 5.5f, -4.0f, itemTextSize_ / 1.5f);
			if (hover(buyMagnumText_))
			{
			    if( !pPlayer_->hasMagnum() && pPlayer_->getPoints() >= priceMagnum_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setHasMagnum(true);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceMagnum_);
				buyMagnumText_.setString("Bought");
				buyMagnumText_.setOrigin(buyMagnumText_.getLocalBounds().width / 2.0f, buyMagnumText_.getLocalBounds().height / 2.0f);
			    }
			}
			else if (hover(buyShotgunText_))
			{
			    if( pPlayer_->getPoints() >= priceShotgun_ && !pPlayer_->hasShotgun() && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setHasShotgun(true);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceShotgun_);
				buyShotgunText_.setString("Bought");
				buyShotgunText_.setOrigin(buyShotgunText_.getLocalBounds().width / 2.0f, buyShotgunText_.getLocalBounds().height / 2.0f);
			    }
			}
			else if (hover(buyRifleText_))
			{
			    if( !pPlayer_->hasRifle() && pPlayer_->getPoints() >= priceRifle_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setHasRifle(true);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceRifle_);
				buyRifleText_.setString("Bought");
				buyRifleText_.setOrigin(buyRifleText_.getLocalBounds().width / 2.0f, buyRifleText_.getLocalBounds().height / 2.0f);
			    }
			}
			else if (hover(buyRocketText_))
			{
			    if(!pPlayer_->hasRocket() && pPlayer_->getPoints() >= priceRocket_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setHasRocket(true);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceRocket_);
				buyRocketText_.setString("Bought");
				buyRocketText_.setOrigin(buyRocketText_.getLocalBounds().width / 2.0f, buyRocketText_.getLocalBounds().height / 2.0f);
			    }
			}
			else if (hover(buyBarricadeText_))
			{
			    if( pPlayer_->getPoints() >= priceBarricade_ && !clickDown_ &&  sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setBarricades(pPlayer_->getBarricades() + 1);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceBarricade_);
			    }
			}
			else if (hover(buyTurretText_))
			{
			    if( pPlayer_->getPoints() >= priceTurret_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setTurrets(pPlayer_->getTurrets() + 1);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceTurret_);
			    }
			}
			else if (hover(buyMineText_))
			{
			    if(pPlayer_->getPoints() >= priceMine_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->setMines(pPlayer_->getMines() + 1);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceMine_);
			    }
			}
			else if(hover(buyPistolAmmoText_))
			{
			    if(pPlayer_->getPoints() >= priceAmmoPistol_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->increaseAmmo(0, pistolAmmoIncrease_);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceAmmoPistol_);
			    }
			}
			else if(hover(buyMagnumAmmoText_))
			{
			    if(pPlayer_->getPoints() >= priceAmmoMagnum_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->increaseAmmo(1, magnumAmmoIncrease_);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceAmmoMagnum_);
			    }
			}
			else if(hover(buyShotgunAmmoText_))
			{
			    if(pPlayer_->getPoints() >= priceAmmoShotgun_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->increaseAmmo(2, shotgunAmmoIncrease_);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceAmmoShotgun_);
			    }
			}
			else if(hover(buyRifleAmmoText_))
			{
			    if(pPlayer_->getPoints() >= priceAmmoRifle_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->increaseAmmo(3, rifleAmmoIncrease_);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceAmmoRifle_);
			    }
			}
			else if(hover(buyRocketAmmoText_))
			{
			    if(pPlayer_->getPoints() >= priceAmmoRocket_ && !clickDown_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			    {
				pPlayer_->increaseAmmo(4, rocketAmmoIncrease_);
				pPlayer_->setPoints(pPlayer_->getPoints() - priceAmmoRocket_);
			    }
			}
			else
				selectionRect_.setFillColor(sf::Color::Transparent);

		}
		else
		{

			buyMenuBackground_.setScale(sf::Vector2f(storeSize_.x * viewSize.x * (time / storeOpenTime_), storeSize_.y * viewSize.y * (time / storeOpenTime_)));
			buyMenuBackground_.setFillColor(sf::Color(226, 232, 235, (int)(235 * (time / storeOpenTime_))));
			buyMenuBackground_.setPosition(pWindow_->getView().getCenter());
		}
	}
	else
	{
		storeText_.setColor(sf::Color::Transparent);
		pistolSprite_.setColor(sf::Color::Transparent);
		magnumSprite_.setColor(sf::Color::Transparent);
		shotgunSprite_.setColor(sf::Color::Transparent);
		rifleSprite_.setColor(sf::Color::Transparent);
		rocketSprite_.setColor(sf::Color::Transparent);
		barricadeSprite_.setColor(sf::Color::Transparent);
		turretSprite_.setColor(sf::Color::Transparent);
		mineSprite_.setColor(sf::Color::Transparent);
		buyMenuBackground_.setFillColor(sf::Color::Transparent);
		pistolText_.setColor(sf::Color::Transparent);
		magnumText_.setColor(sf::Color::Transparent);
		shotgunText_.setColor(sf::Color::Transparent);
		rifleText_.setColor(sf::Color::Transparent);
		rocketText_.setColor(sf::Color::Transparent);
		barricadeText_.setColor(sf::Color::Transparent);
		turretText_.setColor(sf::Color::Transparent);
		mineText_.setColor(sf::Color::Transparent);
		buyMagnumText_.setColor(sf::Color::Transparent);
		buyShotgunText_.setColor(sf::Color::Transparent);
		buyRifleText_.setColor(sf::Color::Transparent);
		buyRocketText_.setColor(sf::Color::Transparent);
		buyBarricadeText_.setColor(sf::Color::Transparent);
		buyTurretText_.setColor(sf::Color::Transparent);
		buyMineText_.setColor(sf::Color::Transparent);
		buyPistolAmmoText_.setColor(sf::Color::Transparent);
		buyMagnumAmmoText_.setColor(sf::Color::Transparent);
		buyShotgunAmmoText_.setColor(sf::Color::Transparent);
		buyRifleAmmoText_.setColor(sf::Color::Transparent);
		buyRocketAmmoText_.setColor(sf::Color::Transparent);
		selectionRect_.setFillColor(sf::Color::Transparent);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		clickDown_ = true;
}

//Helpers
void GUIManager::reset()
{
	pistolSprite_.setTexture(pImageManager_->storePistol);
	magnumSprite_.setTexture(pImageManager_->storeMagnum);
	shotgunSprite_.setTexture(pImageManager_->storeShotgun);
	rifleSprite_.setTexture(pImageManager_->storeRifle);
	rocketSprite_.setTexture(pImageManager_->storeRocket);
	barricadeSprite_.setTexture(pImageManager_->storeBarricade);
	turretSprite_.setTexture(pImageManager_->storeTurret);
	mineSprite_.setTexture(pImageManager_->storeMine);

	pistolSprite_.setOrigin(225.0f, 225.0f);
	magnumSprite_.setOrigin(225.0f, 225.0f);
	shotgunSprite_.setOrigin(225.0f, 225.0f);
	rifleSprite_.setOrigin(225.0f, 225.0f);
	rocketSprite_.setOrigin(225.0f, 225.0f);
	barricadeSprite_.setOrigin(225.0f, 225.0f);
	turretSprite_.setOrigin(225.0f, 225.0f);
	mineSprite_.setOrigin(225.0f, 225.0f);

	//Wave
	waveText_.setFont(font_);
	zombiesText_.setFont(font_);
	waveText_.setColor(sf::Color(226, 232, 235));
	zombiesText_.setColor(sf::Color(226, 232, 235));
	waveBackground_.setFillColor(sf::Color(0, 0, 0, 50));
	waveBackground_.setSize(sf::Vector2f(1.0f, 1.0f));
	waveBackground_.setOrigin(sf::Vector2f(1.0f, 0.0f));
	pointsText_.setFont(font_);

	//Health
	healthOutOf_.setFillColor(sf::Color(214, 34, 34));
	healthCurrent_.setFillColor(sf::Color(78, 213, 116));
	healthOutOf_.setSize(sf::Vector2f(3.0f, 3.0f));
	healthCurrent_.setSize(sf::Vector2f(3.0f, 3.0f));
	healthOutOf_.setOrigin(0.0f, 3.0f);
	healthCurrent_.setOrigin(0.0f, 3.0f);

	//Sprint
	sprintOutOf_.setFillColor(sf::Color(23, 24, 25));
	sprintCurrent_.setFillColor(sf::Color(200, 200, 200));
	sprintOutOf_.setSize(sf::Vector2f(3.0f, 3.0f));
	sprintCurrent_.setSize(sf::Vector2f(3.0f, 3.0f));
	sprintOutOf_.setOrigin(1.5f, 3.0f);
	sprintCurrent_.setOrigin(1.5f, 3.0f);

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
	setUpText(mineText_, "Mine");
	setUpText(buyMagnumText_, "Buy: " + std::to_string(priceMagnum_));
	setUpText(buyShotgunText_, "Buy: " + std::to_string(priceShotgun_));
	setUpText(buyRifleText_, "Buy: " + std::to_string(priceRifle_));
	setUpText(buyRocketText_, "Buy: " + std::to_string(priceRocket_));
	setUpText(buyBarricadeText_, "Buy: " + std::to_string(priceBarricade_));
	setUpText(buyTurretText_, "Buy: " + std::to_string(priceTurret_));
	setUpText(buyMineText_, "Buy: " + std::to_string(priceMine_));
	setUpText(buyPistolAmmoText_, "Ammo: $" + std::to_string(priceAmmoPistol_) + "(+" + std::to_string(pistolAmmoIncrease_) + ")");
	setUpText(buyMagnumAmmoText_, "Ammo: $" + std::to_string(priceAmmoMagnum_) + "(+" + std::to_string(magnumAmmoIncrease_) + ")");
	setUpText(buyShotgunAmmoText_, "Ammo: $" + std::to_string(priceAmmoShotgun_) + "(+" + std::to_string(shotgunAmmoIncrease_) + ")");
	setUpText(buyRifleAmmoText_, "Ammo: $" + std::to_string(priceAmmoRifle_) + "(+" + std::to_string(rifleAmmoIncrease_) + ")");
	setUpText(buyRocketAmmoText_, "Ammo: $" + std::to_string(priceAmmoRocket_) + "(+" + std::to_string(rocketAmmoIncrease_) + ")");
}
void GUIManager::setUpText(sf::Text& text, const std::string& string)
{
	text.setFont(font_);
	text.setColor(sf::Color(40, 42, 43));
	text.setString(string);
	text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2);
}
void GUIManager::showInStore(sf::Text& text, const float x, const float y, const float charSize)
{
	sf::Vector2f size(buyMenuBackground_.getGlobalBounds().width, buyMenuBackground_.getGlobalBounds().height);
	sf::Vector2f pos = buyMenuBackground_.getPosition();
	text.setCharacterSize((int)(charSize * size.y * 2));
	text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
	text.setPosition(sf::Vector2f(pos.x - (size.x / 2.0f) + (size.x / 10.0f) * x, pos.y - size.y / y));
	text.setScale(0.5f, 0.5f);
	text.setColor(sf::Color(40, 42, 43, 220));
}
void GUIManager::showInStore(sf::Sprite& sprite, const float x, const float y)
{
	sf::Vector2f size(buyMenuBackground_.getGlobalBounds().width, buyMenuBackground_.getGlobalBounds().height);
	sf::Vector2f pos = buyMenuBackground_.getPosition();
	sprite.setScale(size.y * 0.0005f, size.y * 0.0005f);
	sprite.setPosition(sf::Vector2f(pos.x - (size.x / 2.0f) + (size.x / 10.0f) * x, pos.y - size.y / y));
	sprite.setColor(sf::Color(255, 255, 255, 175));
}
bool GUIManager::hover(const sf::Text& text)
{
	sf::View view = pWindow_->getView();
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2f currentWindowPos(view.getCenter().x - viewSize.x / 2.0f, view.getCenter().y - viewSize.y / 2.0f);
	float ratio = view.getSize().y / pWindow_->getSize().y;

	if (text.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_) * ratio + currentWindowPos))
	{
		sf::FloatRect bounds = text.getGlobalBounds();
		selectionRect_.setFillColor(sf::Color(255, 0, 55, 200));
		selectionRect_.setSize(sf::Vector2f(bounds.width + bounds.width * 0.75f, bounds.height + bounds.height * 0.25f));
		selectionRect_.setPosition(text.getGlobalBounds().left - text.getGlobalBounds().width * 0.375f, text.getGlobalBounds().top - text.getGlobalBounds().height * 0.175f);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			selectionRect_.setFillColor(sf::Color(150, 0, 0, 225));

		return true;
	}
	else
		return false;

}
//Getters
sf::RectangleShape GUIManager::getHealthOutOf() const { return healthOutOf_; }
sf::RectangleShape GUIManager::getHealthCurrent() const { return healthCurrent_; }
sf::RectangleShape GUIManager::getSprintOutOf() const { return sprintOutOf_; }
sf::RectangleShape GUIManager::getSprintCurrent() const { return sprintCurrent_; }
sf::RectangleShape GUIManager::getReloadOutOf() const { return reloadOutOf_; }
sf::RectangleShape GUIManager::getReloadCurrent() const { return reloadCurrent_; }
sf::Text GUIManager::getTotalAmmo() const { return ammo_; }
sf::RectangleShape GUIManager::getWaveBackground() const { return waveBackground_; }
sf::Text GUIManager::getWaveText() const { return waveText_; }
sf::Text GUIManager::getZombiesText() const { return zombiesText_; }
sf::RectangleShape GUIManager::getBuyMenuBackground() const { return buyMenuBackground_; }
sf::RectangleShape GUIManager::getSelectionRect() const { return selectionRect_; }
sf::Text GUIManager::getStoreText() const { return storeText_; }
sf::Text GUIManager::getPistolText() const { return pistolText_; }
sf::Text GUIManager::getMagnumText() const { return magnumText_; }
sf::Text GUIManager::getShotgunText() const { return shotgunText_; }
sf::Text GUIManager::getRifleText() const { return rifleText_; }
sf::Text GUIManager::getRocketText() const { return rocketText_; }
sf::Text GUIManager::getBarricadeText() const { return barricadeText_; }
sf::Text GUIManager::getTurretText() const { return turretText_; }
sf::Text GUIManager::getMineText() const { return mineText_; }
sf::Text GUIManager::getBuyMagnumText() const { return buyMagnumText_; }
sf::Text GUIManager::getBuyShotgunText() const { return buyShotgunText_; }
sf::Text GUIManager::getBuyRifleText() const { return buyRifleText_; }
sf::Text GUIManager::getBuyRocketText() const { return buyRocketText_; }
sf::Text GUIManager::getBuyBarricadeText() const { return buyBarricadeText_; }
sf::Text GUIManager::getBuyTurretText() const { return buyTurretText_; }
sf::Text GUIManager::getBuyMineText() const { return buyMineText_; }
sf::Text GUIManager::getPointsText() const { return pointsText_; }
sf::Text GUIManager::getBuyPistolAmmoText() const { return buyPistolAmmoText_; }
sf::Text GUIManager::getBuyMagnumAmmoText() const { return buyMagnumAmmoText_; }
sf::Text GUIManager::getBuyShotgunAmmoText() const { return buyShotgunAmmoText_; }
sf::Text GUIManager::getBuyRifleAmmoText() const { return buyRifleAmmoText_; }
sf::Text GUIManager::getBuyRocketAmmoText() const { return buyRocketAmmoText_; }
sf::Sprite GUIManager::getPistolSprite() const { return pistolSprite_; }
sf::Sprite GUIManager::getMagnumSprite() const { return magnumSprite_; }
sf::Sprite GUIManager::getShotgunSprite() const { return shotgunSprite_; }
sf::Sprite GUIManager::getRifleSprite() const { return rifleSprite_; }
sf::Sprite GUIManager::getRocketSprite() const { return rocketSprite_; }
sf::Sprite GUIManager::getBarricadeSprite() const { return barricadeSprite_; }
sf::Sprite GUIManager::getTurretSprite() const { return turretSprite_; }
sf::Sprite GUIManager::getMineSprite() const { return mineSprite_; }
bool GUIManager::isOpen() const { return storeOpen_; }