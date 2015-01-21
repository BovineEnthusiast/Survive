#include "Zombie.h"
#include <iostream>
#include "Collision.h"
Zombie::Zombie(Player* player, sf::Texture* texture, sf::Texture* pCorpseTexture, const int health, const int speed)
: Humanoid(texture), pCorpseTexture_(pCorpseTexture), pPlayer_(player)
{
	corpseSprite_.setTexture(*pCorpseTexture_);
	corpseSprite_.setOrigin(17.0f, 14.0f);
    finalSpeed_ = 3 + std::rand() % (int)(speed * 1.5);
	health_ = 100 + health;

}
void Zombie::update(const sf::Time& dT)
{
	if (!dead_)
	{
		float playerDistance = sqrt(pow(positionGlobal_.y - pPlayer_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pPlayer_->getPositionGlobal().x, 2));
		float turretDistance = -1.0f;
		float closerDistance;

		if (pTurret_ != nullptr)
			turretDistance = sqrt(pow(positionGlobal_.y - pTurret_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pTurret_->getPositionGlobal().x, 2));
		if (pTurret_ != nullptr && turretDistance < playerDistance)
			closerDistance = turretDistance;
		else
			closerDistance = playerDistance;


		if (closerDistance > 33.5f)
		{
			if (closerDistance == playerDistance)
				targetVector_ = pPlayer_->getPositionGlobal() - positionGlobal_;
			else
				targetVector_ = pTurret_->getPositionGlobal() - positionGlobal_;

			targetVector_ /= (float)sqrt(pow(targetVector_.x, 2) + pow(targetVector_.y, 2)); // Normalize
			velocity_ = targetVector_ * speed_;
		}

		else
			velocity_ = sf::Vector2f(0, 0);

		if (closerDistance <= 43.5f && !attacking_)
		{
			attackClock_.restart();
			attacking_ = true;
		}
		animate(dT);
		sf::Vector2f perpVector = sf::Vector2f(cos((rotationGlobal_ + 90.0f) * 3.14159265358f / 180.0f), sin((rotationGlobal_ + 90.0f)  * 3.14159265358 / 180.0f));
		sf::Vector2f forwardVector = sf::Vector2f(cos((rotationGlobal_)* 3.14159265358f / 180.0f), sin((rotationGlobal_)* 3.14159265358 / 180.0f));

		headSprite_.setRotation(atan2(pPlayer_->getPositionGlobal().y - positionGlobal_.y, pPlayer_->getPositionGlobal().x - positionGlobal_.x) * 180.0f / 3.14159265358f);

		armLeftSprite_.setPosition(positionGlobal_ + forwardVector * 6.0f + forwardVector * (float)sin(armClock_.getElapsedTime().asSeconds() * 4 + armLeftVerticalOffset_) - perpVector * 8.0f);
		armRightSprite_.setPosition(positionGlobal_ + forwardVector * 7.4f + forwardVector * (float)sin(armClock_.getElapsedTime().asSeconds() * 4 + armRightVerticalOffset_) + perpVector * 8.0f);
		armLeftSprite_.setRotation(headSprite_.getRotation() + sin(armClock_.getElapsedTime().asSeconds() * 6 + armLeftHorizontalOffset_) * 3);
		armRightSprite_.setRotation(headSprite_.getRotation() + sin(armClock_.getElapsedTime().asSeconds() * 6 + armRightHorizontalOffset_) * 3);

		//Attack animation
		if (attacking_)
		{
			sf::Time sinTime = attackClock_.getElapsedTime();
			if (sinTime.asSeconds() < 0.35f)
			{
				armLeftSprite_.setPosition(armLeftSprite_.getPosition() + forwardVector * (float)sin(sinTime.asSeconds() * 8.164f) * 4.0f);
				armRightSprite_.setPosition(armRightSprite_.getPosition() + forwardVector * (float)sin(sinTime.asSeconds() * 8.164f) * 4.0f);
				armLeftSprite_.setRotation(armLeftSprite_.getRotation() + (float)sin(sinTime.asSeconds() * 8.164f) * 8.0f);
				armRightSprite_.setRotation(armRightSprite_.getRotation() - (float)sin(sinTime.asSeconds() * 8.164f) * 8.0f);

				if (!hit_ && sinTime.asSeconds() > 0.175f)
				{
					if (closerDistance <= 43.5f)
					{
						if (closerDistance == playerDistance)
						{
							pPlayer_->setHealth(pPlayer_->getHealth() - 10.0f);
							pPlayer_->injure();
						}
						else
							pTurret_->setHealth(pTurret_->getHealth() - 10.0f);
						
					}
					hit_ = true;
				}
			}
			else
			{
				attacking_ = false;
				hit_ = false;
			}
		}

		//Sets it to corpse mode if health <= 0
		if (health_ <= 0)
		{
			dead_ = true;
			corpseSpeed_ = finalSpeed_;
		}
	}
	else
	{
		if (!still_)
		{
			sf::Vector2f forwardVector = sf::Vector2f(cos((rotationGlobal_)* 3.14159265358f / 180.0f), sin((rotationGlobal_)* 3.14159265358 / 180.0f));
			positionGlobal_ += forwardVector * corpseSpeed_ * 13.5f * dT.asSeconds();
			corpseSprite_.setPosition(positionGlobal_);
			corpseSprite_.setRotation(rotationGlobal_);
			corpseSpeed_ -= 7.5f * dT.asSeconds();
		}
		if (!still_ && corpseSpeed_ <= 0.0f)
		{
			still_ = true;
			deathClock_.restart();
		}
		else if (still_ && deathClock_.getElapsedTime().asSeconds() >= 3.0f)
			delete_ = true;
		//else if (still_)
			//corpseSprite_.setColor
		
		
	}
}

//Getters
bool Zombie::bled()
{
	if (dead_ && !still_ && bleedClock_.getElapsedTime().asSeconds() > 0.1f)
	{
		bleedClock_.restart();
		return true;
	}
	else
		return false;
}
bool Zombie::countedDead()
{
	if (!countedDead_ && dead_)
	{
		countedDead_ = true;
		return true;
	}
	else
		return false;
}
bool Zombie::isStill() const { return still_; }
bool Zombie::isDead() const { return dead_; }
bool Zombie::isDeletable() const { return delete_; }
sf::Sprite Zombie::getCorpseSprite() const { return corpseSprite_; }

//Setters
void Zombie::setTurretPtr(Turret* pTurret) { pTurret_ = pTurret; }