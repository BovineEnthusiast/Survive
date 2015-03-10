#include "Gun.h"
#include "GameObject.h"
#include <math.h>
Gun::Gun(const std::string& type, ImageManager* pImageManager, SoundManager* pSoundManager)
	:pImageManager_(pImageManager), pSoundManager_(pSoundManager), gunType_(type)
{
	if (type == "pistol")
	{
		pTexture_ = &pImageManager_->gunPistolTexture;
		gun_.setOrigin(3.0f, 3.0f);
		gunPosition_ = sf::Vector2f(20.0f, 6.7f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(0.0f, 0.0f);
		armRightPosLocal_ = armLeftPosLocal_;
	}
	else if (type == "magnum")
	{
		pTexture_ = &pImageManager_->gunMagnumTexture;
		gun_.setOrigin(1.5f, 3.0f);
		gunPosition_ = sf::Vector2f(20.0f, 6.7f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(0.0f, 0.0f);
		armRightPosLocal_ = sf::Vector2f(0.0f, 0.0f);
		reloadTime_ = 2.0f;
		bulletsPerMag_ = 8;
		currentBullets_ = 6;
		totalBullets_ = 32;
		recoilAmount_ = 8.0f;
		fireRate_ = 0.15f; // ~12rps 
		auto_ = false;
		bulletDamage_ = 100;
		bulletSpawnPos_ = sf::Vector2f(15.5f, 0.0f);
	}
	else if (type == "rifle")
	{
		pTexture_ = &pImageManager_->gunRifleTexture;
		gun_.setOrigin(11.0f, 3.5f);
		gunPosition_ = sf::Vector2f(12.0f, 8.0f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(15.0f, 0.0f);
		armRightPosLocal_ = sf::Vector2f(10.0f, 0.0f);
		reloadTime_ = 2.5f;
		bulletsPerMag_ = 30;
		currentBullets_ = 30;
		totalBullets_ = 120;
		recoilAmount_ = 3.3f;
		fireRate_ = 0.08f; // ~12rps 
		auto_ = true;
		bulletDamage_ = 35;
		bulletSpawnPos_ = sf::Vector2f(40.0f, 0.0f);
	}
	else if (type == "shotgun")
	{
		pTexture_ = &pImageManager->gunShotgunTexture;
		gun_.setOrigin(2.5f, 3.5f);
		gunPosition_ = sf::Vector2f(21.5f, 8.0f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(3.0f, 0.0f);
		armRightPosLocal_ = sf::Vector2f(0.0f, 0.0f);
		reloadTime_ = 2.0f;
		bulletsPerMag_ = 2;
		currentBullets_ = 2;
		totalBullets_ = 20;
		recoilAmount_ = 8.0f;
		fireRate_ = 0.15f; // ~12rps 
		auto_ = false;
		bulletDamage_ = 25;
		bulletSpawnPos_ = sf::Vector2f(34.5f, 0.0f);
		shotgun_ = true;
	}
	else if (type == "rocket")
	{
		pTexture_ = &pImageManager->gunRocketTexture;
		gun_.setOrigin(22.0f, 5.0f);
		gunPosition_ = sf::Vector2f(15.0f, 12.0f);
		positionLocal_ = gunPosition_;
		armLeftPosLocal_ = sf::Vector2f(3.0f, 0.0f);
		armRightPosLocal_ = sf::Vector2f(0.0f, 0.0f);
		reloadTime_ = 5.0f;
		bulletsPerMag_ = 1;
		currentBullets_ = 1;
		totalBullets_ = 3;
		recoilAmount_ = 25.0f;
		fireRate_ = 0.1f; 
		auto_ = false;
		bulletDamage_ = 500;
		bulletSpawnPos_ = sf::Vector2f(19.0f, 0.0f);
		rocket_ = true;
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
		swayOffset_ = sf::Vector2f(sin(animationClock_.getElapsedTime().asSeconds() * 5) * 1.0f, cos(animationClock_.getElapsedTime().asSeconds() * 10) * 3.3f);
	}
	else
	{
		if (abs(swayOffset_.x) > 0.5f)
		{
			swayOffset_ = sf::Vector2f(sin(animationClock_.getElapsedTime().asSeconds() * 5) * 1.0f, cos(animationClock_.getElapsedTime().asSeconds() * 10) * 3.3f);
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
			

			if (shotgun_)
			{
			  pSoundManager_->playSound(gunType_);
			  for (int bullet = 0; bullet < 9; ++bullet)
			    {
			      sf::Vector2f bulletVelocity = sf::Vector2f(cos(rotationGlobal_ * 3.14159265358f / 180 + (3.14159265358f / 64.0f) * (bullet - 4)), sin(rotationGlobal_ * 3.14159265358 / 180 + (3.14159265358f / 64.0f) * (bullet - 4))) * bulletSpeed_;
			      pLBullets_->push_back(Bullet(false, spawnPos, bulletVelocity, bulletDamage_));
			    }
			  currentBullets_ -= 1;
			  fireRateClock_.restart();
			  muzzleClock_.restart();

			  shake_ = 2.5f;
			  
			}
			else if (rocket_)
			{
				pSoundManager_->playSound(gunType_);

				sf::Vector2f bulletVelocity = sf::Vector2f(cos(rotationGlobal_ * 3.14159265358 / 180), sin(rotationGlobal_ * 3.14159265358 / 180)) * (bulletSpeed_ / 3.0f);
				pLBullets_->push_back(Bullet(true, spawnPos, bulletVelocity, bulletDamage_));

				currentBullets_ -= 1;
				fireRateClock_.restart();
				muzzleClock_.restart();

				shake_ = 10.0f;
			}
			else
			{
				sf::Vector2f bulletVelocity = sf::Vector2f(cos(rotationGlobal_ * 3.14159265358 / 180), sin(rotationGlobal_ * 3.14159265358 / 180)) * bulletSpeed_;
				pLBullets_->push_back(Bullet(false, spawnPos, bulletVelocity, bulletDamage_));
				fireRateClock_.restart();
				pSoundManager_->playSound(gunType_);
				currentBullets_ -= 1;
				
				muzzleClock_.restart();

				if(gunType_ == "pistol")
				  shake_ = 1.0f;
				else if(gunType_ == "magnum")
				  shake_ = 2.0f;
				else if(gunType_ == "rifle")
				  shake_ = 1.5f;

			}
			vEmitters_.push_back(Emitter(true,
									    spawnPos,
										true,
										10,
										100,
										sf::Vector2f(5.0f, 2.0f),
										sf::Vector2f(2.0f, 2.0f),
										rotationGlobal_ - 12.5f,
										rotationGlobal_ + 12.5f,
										200.0f,
										400.0f,
										0.0f,
										0.05f,
										0.15f,
										sf::Color(255, 204, 49, 255),
										sf::Color(255, 100, 49, 255)));
			vEmitters_.back().setPositionGlobal(spawnPos);

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

	//Update Emitters
	for (auto iEmitter = vEmitters_.begin(); iEmitter != vEmitters_.end();)
	{
		iEmitter->setPositionGlobal(positionGlobal_ + sf::Vector2f(bulletSpawnPos_.x * cos(rotationGlobal_ * 3.14159265358 / 180) - bulletSpawnPos_.y * sin(rotationGlobal_ * 3.14159265358 / 180), bulletSpawnPos_.x * sin(rotationGlobal_ * 3.14159265358 / 180) + bulletSpawnPos_.y * cos(rotationGlobal_ * 3.14159265358 / 180)));
		iEmitter->update(dT);

		if (iEmitter->isDead())
			iEmitter = vEmitters_.erase(iEmitter);
		else
			++iEmitter;
	}

	//Draw lighting if muzzle is flashing
	if (muzzleClock_.getElapsedTime().asSeconds() <= 0.05f)
	{
		muzzleLight_.setPosition((positionGlobal_ + sf::Vector2f(bulletSpawnPos_.x * cos(rotationGlobal_ * 3.14159265358 / 180) - bulletSpawnPos_.y * sin(rotationGlobal_ * 3.14159265358 / 180), bulletSpawnPos_.x * sin(rotationGlobal_ * 3.14159265358 / 180) + bulletSpawnPos_.y * cos(rotationGlobal_ * 3.14159265358 / 180))));
		muzzleLight_.createPolygon();
	}
	muzzleLight_.clearSprites();

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
bool Gun::isMuzzleLight() const { return muzzleClock_.getElapsedTime().asSeconds() <= 0.05f; }
std::vector<sf::ConvexShape> Gun::getMuzzleTriangles() const { return muzzleLight_.getTriangles(); }
std::vector<Emitter> Gun::getEmitters() const { return vEmitters_; }
sf::Vector2f Gun::getBulletSpawnPos() const { return (positionGlobal_ + sf::Vector2f(bulletSpawnPos_.x * cos(rotationGlobal_ * 3.14159265358 / 180) - bulletSpawnPos_.y * sin(rotationGlobal_ * 3.14159265358 / 180), bulletSpawnPos_.x * sin(rotationGlobal_ * 3.14159265358 / 180) + bulletSpawnPos_.y * cos(rotationGlobal_ * 3.14159265358 / 180))); }
float Gun::getShake()
{
  float shake = shake_;
  shake_ = 0.0f;
  return shake;
}
//Setters 
void Gun::setLocalPosition(const sf::Vector2f& position) { positionLocal_ = position; }
void Gun::setPlayerPosition(const sf::Vector2f& position) { playerPos_ = position; }
void Gun::setPlayerVelocity(const sf::Vector2f& velocity) { playerVelocity_ = velocity; }
void Gun::setPlayerHeadRotation(const float rotation) { playerRotation_ = rotation; }
void Gun::setBulletsPtr(std::list<Bullet>* pointer) { pLBullets_ = pointer; ptrSet_ = true; }
void Gun::setAmmoTotal(const int ammo) { totalBullets_ = ammo; }
//Pushers
void Gun::pushMuzzleLightSprite(const sf::Sprite& sprite) { muzzleLight_.pushSprite(sprite); }
