#include "Gun.h"
#include "GameObject.h"
#include <math.h>
Gun::Gun(const std::string& type, ImageManager* pImageManager, SoundManager* pSoundManager)
	:pImageManager_(pImageManager), pSoundManager_(pSoundManager), gunType_(type)
{
	if (type == "pistol")
	{
		pTexture_ = &pImageManager_->gunPistolTexture;
		gun_.setTextureRect(sf::IntRect(0, 0, 26, 10));
		gun_.setOrigin(0.0f, 5.0f);
		gunPosition_ = sf::Vector2f(30.0f, 10.0f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(4.0f, 0.0f);
		armRightPosLocal_ = armLeftPosLocal_;
	}
	else if (type == "rifle")
	{
		pTexture_ = &pImageManager_->gunRifleTexture;
		gun_.setTextureRect(sf::IntRect(0, 0, 74, 14));
		gun_.setOrigin(15.0f, 7.0f);
		gunPosition_ = sf::Vector2f(32.0f, 12.0f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(15.0f, 0.0f);
		armRightPosLocal_ = sf::Vector2f(0.0f, 0.0f);
		reloadTime_ = 2.5f;
		bulletsPerMag_ = 30;
		currentBullets_ = 30;
		totalBullets_ = 9999;
		recoilAmount_ = 5;
		fireRate_ = 0.08f; // ~12rps 
		auto_ = true;
		bulletDamage_ = 35;
		bulletSpawnPos_ = sf::Vector2f(59.0f, 0.0f);

	}

	gun_.setTexture(*pTexture_);

}
void Gun::update(const sf::Time& dT)
{
	if (clicked_ && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
		clicked_ = false;

	//Animated the local position if moving
	if (playerVelocity_ != sf::Vector2f(0, 0))
	{
		swayOffset_ = sf::Vector2f(sin(animationClock_.getElapsedTime().asSeconds() * 5) * 1.0f, cos(animationClock_.getElapsedTime().asSeconds() * 10) * 5.00f);
	}
	else
	{
		if (abs(swayOffset_.x) > 0.5f)
		{
			swayOffset_ = sf::Vector2f(sin(animationClock_.getElapsedTime().asSeconds() * 5) * 1.0f, cos(animationClock_.getElapsedTime().asSeconds() * 10) * 5.00f);
		}
		else
		{
			animationClock_.restart();
			swayOffset_ = sf::Vector2f(0, 0);
		}
	}

	if (fireRateClock_.getElapsedTime().asSeconds() <= fireRate_ / 3.0f)
		recoilOffset_ = sf::Vector2f(dT.asSeconds() / (fireRate_ / 3.0f) * recoilAmount_, 0);
	else if (fireRateClock_.getElapsedTime().asSeconds() <= fireRate_)
		recoilOffset_ = sf::Vector2f((dT.asSeconds() - fireRate_ / 3.0f) / (fireRate_ * 2.0f / 3.0f) * recoilAmount_, 0);
	else
		recoilOffset_ = sf::Vector2f(0, 0);

	//Value used to rotate sprites to player's rotation
	float rotateBy = playerRotation_ * 3.14159265358 / 180;


	//rotates the offsets based on player rotation
	swayOffset_ = sf::Vector2f(swayOffset_.x * cos(rotateBy) - swayOffset_.y * sin(rotateBy), swayOffset_.x * sin(rotateBy) + swayOffset_.y * cos(rotateBy));
	recoilOffset_ = sf::Vector2f(recoilOffset_.x * cos(rotateBy) - recoilOffset_.y * sin(rotateBy), recoilOffset_.x * sin(rotateBy) + recoilOffset_.y * cos(rotateBy));

	//Updates position + rotation
	positionGlobal_ = playerPos_ + sf::Vector2f(positionLocal_.x * cos(rotateBy) - positionLocal_.y * sin(rotateBy), positionLocal_.x * sin(rotateBy) + positionLocal_.y * cos(rotateBy));
	armLeftPosGlobal_ = sf::Vector2f(recoilOffset_.x + swayOffset_.x + armLeftPosLocal_.x * cos(rotateBy) - armLeftPosLocal_.y * sin(rotateBy), recoilOffset_.y + swayOffset_.y + armLeftPosLocal_.x * sin(rotateBy) + armLeftPosLocal_.y * cos(rotateBy));
	armRightPosGlobal_ = sf::Vector2f(recoilOffset_.x + swayOffset_.x + armRightPosLocal_.x * cos(rotateBy) - armRightPosLocal_.y * sin(rotateBy), recoilOffset_.y + swayOffset_.y + armRightPosLocal_.x * sin(rotateBy) + armRightPosLocal_.y * cos(rotateBy));
	rotationGlobal_ = atan2((sf::Mouse::getPosition(*window).y + (((float)window->getView().getCenter().y) - ((float)window->getSize().y / 2.0f))) - positionGlobal_.y, (sf::Mouse::getPosition(*window).x + (((float)window->getView().getCenter().x) - ((float)window->getSize().x / 2.0f))) - positionGlobal_.x) * 180 / 3.14159265358;
	gun_.setPosition(positionGlobal_ + swayOffset_ + recoilOffset_);
	gun_.setRotation(rotationGlobal_);

	//If not reloading and 'R' is pressed, reload
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && currentBullets_ < bulletsPerMag_ && totalBullets_ > 0 && !reloading_)
	{
		reloadClock_.restart();
		reloading_ = true;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (fireRateClock_.getElapsedTime().asSeconds() >= fireRate_ && currentBullets_ > 0 && !reloading_ && (auto_ || !clicked_) && ptrSet_)
		{
			//Converts the rotation to a vector, times it by bulletSpeed_, and creates a bullet
			sf::Vector2f spawnPos = positionGlobal_ + sf::Vector2f(bulletSpawnPos_.x * cos(rotationGlobal_ * 3.14159265358 / 180) - bulletSpawnPos_.y * sin(rotationGlobal_ * 3.14159265358 / 180), bulletSpawnPos_.x * sin(rotationGlobal_ * 3.14159265358 / 180) + bulletSpawnPos_.y * cos(rotationGlobal_ * 3.14159265358 / 180));
			sf::Vector2f bulletVelocity = sf::Vector2f(cos(rotationGlobal_ * 3.14159265358 / 180), sin(rotationGlobal_ * 3.14159265358 / 180)) * bulletSpeed_;
			Bullet billy(Bullet(spawnPos, bulletVelocity, bulletDamage_));
			pLBullets_->push_back(Bullet(spawnPos, bulletVelocity, bulletDamage_));
			currentBullets_ -= 1;
			fireRateClock_.restart();
			pSoundManager_->playSound(gunType_);
		}
		else if (currentBullets_ == 0 && totalBullets_ > 0 && !reloading_)
		{
			reloadClock_.restart();
			reloading_ = true;
		}
		clicked_ = true;

	}

	//Reloaded if reloading clock finished
	if (reloading_ && reloadClock_.getElapsedTime().asSeconds() > reloadTime_)
	{
		reloading_ = false;
		if (totalBullets_ >= bulletsPerMag_)
		{
			currentBullets_ = bulletsPerMag_;
			totalBullets_ -= bulletsPerMag_;
		}
		else
		{
			currentBullets_ = totalBullets_;
			totalBullets_ = 0;
		}
	}

}


//Getters
sf::Vector2f Gun::getArmLeftPos() { return armLeftPosGlobal_; }
sf::Vector2f Gun::getArmRightPos() { return armRightPosGlobal_; }
sf::Sprite Gun::getSprite() { return gun_; }
int Gun::getDamage() const { return bulletDamage_; }
int Gun::getCurrentBullets() const { return currentBullets_; }
int Gun::getBulletsPerMag() const { return bulletsPerMag_; }
int Gun::getTotalAmmo() const { return totalBullets_; }
sf::Time Gun::getCurrentReloadTime() const { return reloadClock_.getElapsedTime(); }
float Gun::getReloadTime() const { return reloadTime_; }
bool Gun::isReloading() const { return reloading_; }

//Setters 
void Gun::setLocalPosition(const sf::Vector2f& position) { positionLocal_ = position; }
void Gun::setPlayerPosition(const sf::Vector2f& position) { playerPos_ = position; }
void Gun::setPlayerVelocity(const sf::Vector2f& velocity) { playerVelocity_ = velocity; }
void Gun::setPlayerHeadRotation(const float rotation) { playerRotation_ = rotation; }
void Gun::setBulletsPtr(std::list<Bullet>* pointer) { pLBullets_ = pointer; ptrSet_ = true; }