#include "SpatialPartition.h"
#include "Collision.h"
#include <iostream>

SpatialPartition::SpatialPartition(const sf::FloatRect& partitionSpace, Player* player, std::vector<std::vector<SpatialPartition>>* partitions, SoundManager* pSoundManager, int* pZombiesAlive, int* pZombiesToSpawn, int* pWave)
	:partitionSpace_(partitionSpace), player_(player), partitions_(partitions), pSoundManager_(pSoundManager), pZombiesAlive_(pZombiesAlive), pZombiesToSpawn_(pZombiesToSpawn), pWave_(pWave)
{
	xPos_ = partitionSpace.left / 320;
	yPos_ = partitionSpace.top / 320;

}
void SpatialPartition::update(const sf::Time& dT)
{
	if (firstUpdate_)
	{
		if (xPos_ - 1 >= 0 && yPos_ - 1 >= 0)
			pSpatialPartitions_[0] = &partitions_->at(yPos_ - 1).at(xPos_ - 1);
		else
			pSpatialPartitions_[0] = nullptr;
		if (yPos_ - 1 >= 0)
			pSpatialPartitions_[1] = &partitions_->at(yPos_ - 1).at(xPos_);
		else
			pSpatialPartitions_[1] = nullptr;
		if (xPos_ + 1 < partitions_->size() && yPos_ - 1 >= 0)
			pSpatialPartitions_[2] = &partitions_->at(yPos_ - 1).at(xPos_ + 1);
		else
			pSpatialPartitions_[2] = nullptr;
		if (xPos_ + 1 < partitions_->size())
			pSpatialPartitions_[3] = &partitions_->at(yPos_).at(xPos_ + 1);
		else
			pSpatialPartitions_[3] = nullptr;
		if (xPos_ + 1 < partitions_->size() && yPos_ + 1 < partitions_->size())
			pSpatialPartitions_[4] = &partitions_->at(yPos_ + 1).at(xPos_ + 1);
		else
			pSpatialPartitions_[4] = nullptr;
		if (yPos_ + 1 < partitions_->size())
			pSpatialPartitions_[5] = &partitions_->at(yPos_ + 1).at(xPos_);
		else
			pSpatialPartitions_[5] = nullptr;
		if (xPos_ - 1 >= 0 && yPos_ + 1 < partitions_->size())
			pSpatialPartitions_[6] = &partitions_->at(yPos_ + 1).at(xPos_ - 1);
		else
			pSpatialPartitions_[6] = nullptr;
		if (xPos_ - 1 >= 0)
			pSpatialPartitions_[7] = &partitions_->at(yPos_).at(xPos_ - 1);
		else
			pSpatialPartitions_[7] = nullptr;

		firstUpdate_ = false;
	}
	//Checks if the player is within this partition
	if (partitionSpace_.contains(player_->getPositionGlobal()))
		hasPlayer_ = true;
	else
		hasPlayer_ = false;

	if (hasPlayer_)
	{
		player_->update(dT);
		player_->setGunBulletPointers(&lBullets_);
	}

	//--------------------------------------------UPDATE--------------------------------
	//Blood
	for (auto iBloodSplat = dBloodSplats_.begin(); iBloodSplat != dBloodSplats_.end();)
	{
		iBloodSplat->update(dT);

		//Removes the blood if it is old enough, increments iterator
		if (iBloodSplat->isFaded())
			iBloodSplat = dBloodSplats_.erase(iBloodSplat);
		else
			iBloodSplat++;
	}

	//Dens
	for (auto& den : vDens_)
	{
		den.update(dT);

		//Spawn a zombie if time is ready
		if (den.isReadyToSpawn() && *pZombiesToSpawn_ > 0)
		{
			--(*pZombiesToSpawn_);
			++(*pZombiesAlive_);
			Zombie zombie = Zombie(player_, &imageManager_->humanoidZombieTexture, &imageManager_->zombieCorpseTexture, (*pWave_ - 1) * 10, (*pWave_ + 10));
			zombie.pTiles = pVTiles_;
			zombie.setPositionGlobal(den.getPositionGlobal());
			vZombies_.push_back(zombie);
		}
	}

	//Zombies
	for (auto iZombie = vZombies_.begin(); iZombie != vZombies_.end();)
	{
		float closestDistance = 9999.9f;
		Turret* pClosestTurret = nullptr;

		for (auto& turret : vTurrets_)
		{
			float distance = sqrt(pow(turret.getPositionGlobal().x - iZombie->getPositionGlobal().x, 2) + pow(turret.getPositionGlobal().y - iZombie->getPositionGlobal().y, 2));
			if (distance < closestDistance)
			{
				closestDistance = distance;
				pClosestTurret = &turret;
			}
		}
		for (auto& partition : pSpatialPartitions_)
			for (auto& turret : vTurrets_)
			{
				float distance = sqrt(pow(turret.getPositionGlobal().x - iZombie->getPositionGlobal().x, 2) + pow(turret.getPositionGlobal().y - iZombie->getPositionGlobal().y, 2));
				if (distance < closestDistance)
				{
					closestDistance = distance;
					pClosestTurret = &turret;
				}
			}
			
		if (pClosestTurret != nullptr)
			iZombie->setTurretPtr(pClosestTurret);

		iZombie->update(dT);

		//Adds blood splats if it is sliding on the floor
		if (iZombie->bled())
		{
			dBloodSplats_.push_back(BloodSplat(&imageManager_->vBloodSplatTextures[0]));
			dBloodSplats_.back().setPositionGlobal(iZombie->getPositionGlobal());

		}
		if (iZombie->countedDead())
			--(*pZombiesAlive_);

		//Removes the zombie if it is dead, increments iterator
		if (iZombie->isDeletable())
			iZombie = vZombies_.erase(iZombie);
		
		else
		{
			//If outside this partition, move it
			sf::Vector2f zomPos = iZombie->getPositionGlobal();
			if (!partitionSpace_.contains(zomPos))
			{
				bool found = false;
				for (auto& partition : pSpatialPartitions_)
					if (partition != nullptr && partition->getPartitionSpace().contains(zomPos))
					{
						partition->pushZombie(*iZombie);
						iZombie = vZombies_.erase(iZombie);
						found = true;
						break;
					}
				if (!found)
					iZombie = vZombies_.erase(iZombie);
			}
			else
				iZombie++;
		}
	}

	//Tree
	for (auto& tree : vTrees_)
	{
		tree.update(dT);
	}

	for (auto iBullet = lBullets_.begin(); iBullet != lBullets_.end();)
	{
		iBullet->update(dT);

		//If hit or dead, remove, else increments iterater
		if (iBullet->isHit() || iBullet->isHit())
			iBullet = lBullets_.erase(iBullet);
		else
		{
			//If outside partition, move it
			sf::Vector2f bulletPos = iBullet->getPositionGlobal();
			if (!partitionSpace_.contains(bulletPos))
			{
				bool found = false;
				for (auto& partition : pSpatialPartitions_)
					if (partition != nullptr && partition->getPartitionSpace().contains(bulletPos))
					{
						partition->pushBullet(*iBullet);
						iBullet = lBullets_.erase(iBullet);
						found = true;
						break;
					}
				if (!found)
					iBullet = lBullets_.erase(iBullet);
			}
			else
				++iBullet;
		}
	}

	//Turrets
	if (hasPlayer_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && turretClock_.getElapsedTime().asSeconds() > 1.0f)
	{
		turretClock_.restart();
		vTurrets_.push_back(Turret(player_->getPositionGlobal(), &lBullets_, imageManager_));
	}

	std::vector<sf::Vector2f> zomPositions;
	for (auto& zombie : vZombies_)
		if (!zombie.isDead())
			zomPositions.push_back(zombie.getPositionGlobal());

	for (auto& partition : pSpatialPartitions_)
		if (partition != nullptr)
			for (auto& zombie : partition->getZombies())
				if (!zombie.isDead())
					zomPositions.push_back(zombie.getPositionGlobal());

	for (auto& turret : vTurrets_)
	{
		turret.preUpdate(zomPositions);
		turret.update(dT);
	}

	//-----------------COLLISION--------------------------------
	for (auto& bullet : lBullets_)
	{
		//This partition's zombies
		for (auto& zombie : vZombies_)
			if (isColliding(zombie.getHeadSprite(), bullet.getSprite(), bullet.getLastPosition()) == true && zombie.getHealth() > 0)
			{
				zombie.injure();
				zombie.setHealth(zombie.getHealth() - bullet.getDamage());
				dBloodSplats_.push_back(BloodSplat(&imageManager_->vBloodSplatTextures.at(std::rand() % imageManager_->vBloodSplatTextures.size())));
				dBloodSplats_.at(dBloodSplats_.size() - 1).setPositionGlobal(bullet.getSprite().getPosition());
				bullet.setHit(true);
				pSoundManager_->playSound("hit");
			}

		//Neighboring partition's zombies
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& zombie : partition->vZombies_)
					if (isColliding(zombie.getHeadSprite(), bullet.getSprite(), bullet.getLastPosition()) == true && zombie.getHealth() > 0)
					{
						zombie.injure();
						zombie.setHealth(zombie.getHealth() - bullet.getDamage());
						dBloodSplats_.push_back(BloodSplat(&imageManager_->vBloodSplatTextures.at(std::rand() % imageManager_->vBloodSplatTextures.size())));
						dBloodSplats_.at(dBloodSplats_.size() - 1).setPositionGlobal(bullet.getSprite().getPosition());
						bullet.setHit(true);
						pSoundManager_->playSound("hit");
					}

		//This partition's trees
		for (auto& tree : vTrees_)
			if ((isColliding(tree.getTrunk(), bullet.getSprite(), bullet.getLastPosition())) == true)
				bullet.setHit(true);

		//Neighboring partition's trees
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& tree : partition->vTrees_)
					if ((isColliding(tree.getTrunk(), bullet.getSprite(), bullet.getLastPosition())) == true)
						bullet.setHit(true);
	}
	for (auto& zombie : vZombies_)
	{
		if (!zombie.isDead())
		{
			//This partition's zombies
			for (auto& zombie2 : vZombies_)
				if (!zombie2.isDead() && &zombie != &zombie2 && isColliding(zombie.getHeadSprite(), zombie2.getHeadSprite()) != sf::Vector2f(0.0f, 0.0f))
					zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombie.getHeadSprite(), zombie2.getHeadSprite()));

			//Neigboring partition's zombies
			for (auto& partition : pSpatialPartitions_)
				for (auto& zombie2 : partition->vZombies_)
					if (!zombie2.isDead() && isColliding(zombie.getHeadSprite(), zombie2.getHeadSprite()) != sf::Vector2f(0.0f, 0.0f))
						zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombie.getHeadSprite(), zombie2.getHeadSprite()));

			//This partition's trees
			for (auto& tree : vTrees_)
				if (isColliding(zombie.getHeadSprite(), tree.getTrunk()) != sf::Vector2f(0.0f, 0.0f))
					zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombie.getHeadSprite(), tree.getTrunk()));

			//Neighboring partitions's tree
			for (auto& partition : pSpatialPartitions_)
				for (auto& tree : partition->vTrees_)
					if (isColliding(zombie.getHeadSprite(), tree.getTrunk()) != sf::Vector2f(0.0f, 0.0f))
						zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombie.getHeadSprite(), tree.getTrunk()));
		}
		
	}

	if (hasPlayer_)
	{
		//This partition's zombies
		for (auto& zombie : vZombies_)
			if (!zombie.isDead() && isColliding(zombie.getHeadSprite(), player_->getHeadSprite()) != sf::Vector2f(0.0f, 0.0f))
				player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(zombie.getHeadSprite(), player_->getHeadSprite()));

		//Neigboring partition's zombies
		for (auto& partition : pSpatialPartitions_)
			for (auto& zombie : partition->vZombies_)
				if (!zombie.isDead() && isColliding(zombie.getHeadSprite(), player_->getHeadSprite()) != sf::Vector2f(0.0f, 0.0f))
					player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(zombie.getHeadSprite(), player_->getHeadSprite()));

		//This partition's trees
		for (auto& tree : vTrees_)
			if (isColliding(player_->getHeadSprite(), tree.getTrunk()) != sf::Vector2f(0.0f, 0.0f))
				player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(player_->getHeadSprite(), tree.getTrunk()));

		//Neighboring partitions's tree
		for (auto& partition : pSpatialPartitions_)
			for (auto& tree : partition->vTrees_)
				if (isColliding(player_->getHeadSprite(), tree.getTrunk()) != sf::Vector2f(0.0f, 0.0f))
					player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(player_->getHeadSprite(), tree.getTrunk()));
	}
}

//Setters
void SpatialPartition::setImageManagerPointer(ImageManager* imageManager) { imageManager_ = imageManager; }
void SpatialPartition::setTilesPointer(std::vector<std::vector<Tile> >* pTiles) { pVTiles_ = pTiles; }


//Getters
sf::FloatRect SpatialPartition::getPartitionSpace() const { return partitionSpace_; }
std::vector<Zombie> SpatialPartition::getZombies() const { return vZombies_; }
std::list<Bullet> SpatialPartition::getBullets() const { return lBullets_; }
std::vector<Tree> SpatialPartition::getTrees() const { return vTrees_; }
std::vector<Den> SpatialPartition::getDens() const { return vDens_; }
std::deque<BloodSplat> SpatialPartition::getBloodSplats() const { return dBloodSplats_; }
std::vector<Turret> SpatialPartition::getTurrets() const { return vTurrets_; }
std::array<SpatialPartition*, 8> SpatialPartition::getNeigborPartitions() const { return pSpatialPartitions_; }
//Pushers
void SpatialPartition::pushZombie(const Zombie& zombie) { vZombies_.push_back(zombie); }
void SpatialPartition::pushBullet(const Bullet& bullet) { lBullets_.push_back(bullet); }
void SpatialPartition::pushTree(const Tree& tree) { vTrees_.push_back(tree); }
void SpatialPartition::pushDen(const Den& den) { vDens_.push_back(den); }
void SpatialPartition::pushBloodSplat(const BloodSplat& splat) { dBloodSplats_.push_back(splat); }