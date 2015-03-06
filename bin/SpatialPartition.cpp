#include <queue>
#include <cmath>
#include <thread>
#include "SpatialPartition.h"
#include "Collision.h"
#include <iostream>

sf::RectangleShape SpatialPartition::selectionRect_;
bool SpatialPartition::selecting_;
bool SpatialPartition::clickTurretDown_;
bool SpatialPartition::clickBarricadeDown_;
bool SpatialPartition::clickMineDown_;

SpatialPartition::SpatialPartition(const sf::FloatRect& partitionSpace, Player* player, std::vector<std::vector<SpatialPartition> >* partitions, SoundManager* pSoundManager, int* pZombiesAlive, int* pZombiesToSpawn, int* pWave)
	:partitionSpace_(partitionSpace), player_(player), partitions_(partitions), pSoundManager_(pSoundManager), pZombiesAlive_(pZombiesAlive), pZombiesToSpawn_(pZombiesToSpawn), pWave_(pWave)
{
	xPos_ = partitionSpace.left / 320;
	yPos_ = partitionSpace.top / 320;

	//Static variable initialization      
	selecting_ = false;
	clickTurretDown_ = false;
	clickBarricadeDown_ = false;
	
	selectionRect_.setSize(sf::Vector2f(32.0f, 32.0f));
	selectionRect_.setOrigin(16.0f, 16.0f);
	selectionRect_.setFillColor(sf::Color(0, 255, 0, 255));
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
		if (iZombie->needsPath())
			iZombie->findPath(pVTiles_);

		// std::thread pathThread(&Zombie::findPath, iZombie, pVTiles_);
		if (targetClock_.getElapsedTime().asSeconds() >= 1.0f)
		{
			targetClock_.restart();
			std::deque<SpatialPartition*> openList;
			std::vector<SpatialPartition*> closedList;
			bool foundTurret = false;
			bool foundBarricade = false;
			float closestDistance = 9999.9f;
			Turret* pClosestTurret = nullptr;
			Barricade* pClosestBarricade = nullptr;
			sf::Vector2f zombiePos = iZombie->getPositionGlobal();
			for (auto& turret : vTurrets_)
			{
				foundTurret = true;
				sf::Vector2f turretPos = turret.getPositionGlobal();
				float distance = abs(turretPos.x - zombiePos.x) + abs(turretPos.y - zombiePos.y);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					pClosestTurret = &turret;
				}
			}
			for (auto& barricade : vBarricades_)
			{
				foundBarricade = true;
				sf::Vector2f barricadePos = barricade.getPositionGlobal();
				float distance = abs(barricadePos.x - zombiePos.x) + (barricadePos.y - zombiePos.y);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					pClosestBarricade = &barricade;
				}
			}
			for (auto& partition : pSpatialPartitions_)
			{
				if (partition != nullptr)
				{
					openList.push_back(partition);
					for (auto& turret : partition->vTurrets_)
					{
						foundTurret = true;
						sf::Vector2f turretPos = turret.getPositionGlobal();
						float distance = abs(turretPos.x - zombiePos.x) + abs(turretPos.y - zombiePos.y);
						if (distance < closestDistance)
						{
							closestDistance = distance;
							pClosestTurret = &turret;
						}
					}

					for (auto& barricade : partition->vBarricades_)
					{
						foundBarricade = true;
						sf::Vector2f barricadePos = barricade.getPositionGlobal();
						float distance = abs(barricadePos.x - zombiePos.x) + (barricadePos.y - zombiePos.y);
						if (distance < closestDistance)
						{
							closestDistance = distance;
							pClosestBarricade = &barricade;
						}
					}
				}
			}
			//No zombies were found in the initial ring of partitions
			const int MIN_NUM_PART_CHECK = 16; //The number of partitions to check at least beyond the initial ring if the initial ring contained no zombie
			int partitionsChecked = 0;
			while ((!foundTurret || !foundBarricade) && closedList.size() != 100)
			{
				++partitionsChecked;
				SpatialPartition* pPartition = openList.front();
				openList.pop_front();
				if (pPartition != nullptr)
				{
					for (auto& turret : pPartition->vTurrets_)
					{
						if (partitionsChecked >= MIN_NUM_PART_CHECK)
							foundTurret = true;
						sf::Vector2f turretPos = turret.getPositionGlobal();
						float distance = abs(turretPos.x - zombiePos.x) + abs(turretPos.y - zombiePos.y);
						if (distance < closestDistance)
						{
							closestDistance = distance;
							pClosestTurret = &turret;
						}

					}
					for (auto& barricade : pPartition->vBarricades_)
					{
						if (partitionsChecked >= MIN_NUM_PART_CHECK)
							foundBarricade = true;
						sf::Vector2f barricadePos = barricade.getPositionGlobal();
						float distance = abs(barricadePos.x - zombiePos.x) + (barricadePos.y - zombiePos.y);
						if (distance < closestDistance)
						{
							closestDistance = distance;
							pClosestBarricade = &barricade;
						}
					}
					for (auto& partition : pPartition->pSpatialPartitions_)
					{
						if (partition != nullptr)
						{
							bool isOnList = false;
							for (auto& listPartition : openList)
								if (listPartition == partition)
									isOnList = true;
							for (auto& listPartition : closedList)
								if (listPartition == partition)
									isOnList = true;
							if (!isOnList)
								openList.push_back(partition);
						}

					}

					closedList.push_back(pPartition);
				}
			}
			if (pClosestBarricade != nullptr)
				iZombie->setBarricadePtr(pClosestBarricade);
			if (pClosestTurret != nullptr)
				iZombie->setTurretPtr(pClosestTurret);
		}
		//pathThread.join();					
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
		tree.update(dT);

	for (auto iBullet = lBullets_.begin(); iBullet != lBullets_.end();)
	{
		iBullet->update(dT);

		sf::Vector2f bulletPos(iBullet->getPositionGlobal());
		sf::Vector2i bulletTilePos((bulletPos.x - fmod(bulletPos.x, 32.0f) + 16) / 32, (bulletPos.y - fmod(bulletPos.y, 32.0f) + 16) / 32);

		//If hit or dead, remove, else increments iterater
		if (iBullet->isHit() || iBullet->isHit() || ((bulletTilePos.x >= 0 && bulletTilePos.y >= 0 && bulletTilePos.x < 255 && bulletTilePos.y < 255) && pVTiles_->at(bulletTilePos.x).at(bulletTilePos.y).getType() == "rock"))
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

	sf::Vector2f playerPosition = player_->getPositionGlobal();
	int xPos = 0;
	int yPos = 0;

	int rotation = (int)player_->getHeadSprite().getRotation() % 360;
	if (rotation <= 23 || rotation > 338)
	{
		xPos = 1;
		yPos = 0;
	}
	else if (rotation <= 68)
	{
		xPos = 1;
		yPos = 1;
	}
	else if (rotation <= 113)
	{
		xPos = 0;
		yPos = 1;
	}
	else if (rotation <= 158)
	{
		xPos = -1;
		yPos = 1;
	}
	else if (rotation <= 203)
	{
		xPos = -1;
		yPos = 0;
	}
	else if (rotation <= 248)
	{
		xPos = -1;
		yPos = -1;
	}
	else if (rotation <= 293)
	{
		xPos = 0;
		yPos = -1;
	}
	else if (rotation <= 338)
	{
		xPos = 1;
		yPos = -1;
	}

	Tile* pAboveTile = &pVTiles_->at((playerPosition.x - fmod(playerPosition.x, 32.0f)) / 32 + xPos).at((playerPosition.y - fmod(playerPosition.y, 32.0f)) / 32 + yPos);


	//Turret Addition
	if (hasPlayer_ && !clickTurretDown_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && player_->getTurrets() > 0)
	{
		clickTurretDown_ = true;
		if (!selecting_)
			selecting_ = true;
		else
		{
			selecting_ = false;
			if (pAboveTile->getType() == "walkable" && !pAboveTile->hasItem())
			{
				pAboveTile->setHasItem(true);
				player_->setTurrets(player_->getTurrets() - 1);
				vTurrets_.push_back(Turret(pAboveTile->getSprite().getPosition(), &lBullets_, imageManager_, pSoundManager_));
			}
		}
	}

	//Barricade Addition
	if (hasPlayer_ && !clickBarricadeDown_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) && player_->getBarricades() > 0)
	{
		clickBarricadeDown_ = true;
		if (!selecting_)
			selecting_ = true;
		else
		{
			selecting_ = false;
			if (pAboveTile->getType() == "walkable" && !pAboveTile->hasItem())
			{
				pAboveTile->setHasItem(true);
				clickBarricadeDown_ = true;
				player_->setBarricades(player_->getBarricades() - 1);
				vBarricades_.push_back(Barricade(pAboveTile->getSprite().getPosition(), &imageManager_->barricadeTexture));
				vBarricades_.back().setHealth(100);
			}
		}
	}
	//Mine Addition
	if(hasPlayer_ && !clickMineDown_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) && player_->getMines() > 0)
	{
	    clickMineDown_ = true;
	    Mine mine(&imageManager_->mineTexture);
	    mine.setPositionGlobal(player_->getPositionGlobal());
	    mine.update(dT);

	    bool placed = false;
	    
	    for(auto& altMine : vMines_)
		if(isColliding(mine.getMine(), altMine.getMine()) == sf::Vector2f(0.0f, 0.0f))
		    placed = true;

	    for(auto& partition : pSpatialPartitions_)
		for(auto& altMine : vMines_)
		    if(isColliding(mine.getMine(), altMine.getMine()) == sf::Vector2f(0.0f, 0.0f))
			placed = true;

	    //if(placed)
		vMines_.push_back(mine);
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

	for (auto iTurret = vTurrets_.begin(); iTurret != vTurrets_.end();)
	{

		iTurret->pushMuzzleLightSprite(player_->getHeadSprite());

		for (auto& zombie : vZombies_)		
				iTurret->pushMuzzleLightSprite(zombie.getHeadSprite());

		//Neigboring partition's zombies
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& zombie : partition->vZombies_)
						iTurret->pushMuzzleLightSprite(zombie.getHeadSprite());
		
		//This partition's trees
		for (auto& tree : vTrees_)
			iTurret->pushMuzzleLightSprite(tree.getTrunk());

		//Neighboring partitions's tree
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& tree : partition->vTrees_)
					iTurret->pushMuzzleLightSprite(tree.getTrunk());
		//This partition's turrets
		for (auto& turret : vTurrets_)
			if (turret.getPositionGlobal() != iTurret->getPositionGlobal())
				iTurret->pushMuzzleLightSprite(turret.getBaseSprite());

		//Neighboring partition's turrets
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& turret : vTurrets_)
					if (turret.getPositionGlobal() != iTurret->getPositionGlobal())
						iTurret->pushMuzzleLightSprite(turret.getBaseSprite());

		//This partition's barricades
		for (auto& barricade : vBarricades_)
			iTurret->pushMuzzleLightSprite(barricade.getSprite());

		//Neighboring partition's barricades
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& barricade : partition->vBarricades_)
					iTurret->pushMuzzleLightSprite(barricade.getSprite());



		iTurret->preUpdate(zomPositions);
		iTurret->update(dT);
		if (iTurret->isSafeToDelete())
		{
			sf::Vector2f turretPosition = iTurret->getPositionGlobal();
			pVTiles_->at((turretPosition.x - fmod(turretPosition.x, 32.0f)) / 32).at((turretPosition.y - fmod(turretPosition.y, 32.0f)) / 32).setHasItem(false);
			iTurret = vTurrets_.erase(iTurret);
		}
		else
			++iTurret;
	}

	for (auto iBarricade = vBarricades_.begin(); iBarricade != vBarricades_.end();)
	{
		iBarricade->update(dT);
		if (iBarricade->isSafeToDelete())
		{
			sf::Vector2f barricadePosition = iBarricade->getPositionGlobal();
			pVTiles_->at((barricadePosition.x - fmod(barricadePosition.x, 32.0f)) / 32).at((barricadePosition.y - fmod(barricadePosition.y, 32.0f)) / 32).setHasItem(false);
			iBarricade = vBarricades_.erase(iBarricade);
		}
		else
			++iBarricade;
	}

	for (auto iMine = vMines_.begin(); iMine != vMines_.end(); ++iMine)
	{
	    iMine->update(dT);
	    //if(iMine->exploded())
		//iMine = vMines_.erase(iMine);
	}


	//-----------------COLLISION--------------------------------
	for (auto& bullet : lBullets_)
	{
		float rotation = (bullet.getSprite().getRotation() - 180.0f);
		float rotationRadians = rotation * 3.14159265358f / 180.0f;
		sf::Vector2f position(bullet.getPositionGlobal() - sf::Vector2f(cos(rotationRadians) * 7.5f, sin(rotationRadians) * 7.5f));

		//This partition's zombies
		for (auto& zombie : vZombies_)
		{
			if (isColliding(zombie.getHeadSprite(), bullet.getSprite(), bullet.getLastPosition()) && zombie.getHealth() > 0)
			{
				zombie.injure();
				zombie.setHealth(zombie.getHealth() - bullet.getDamage());
				bullet.setHit(true);
				pSoundManager_->playSound("hit");
				player_->setPoints(player_->getPoints() + 10);

				
				vEmitters_.push_back(Emitter(true,
					position,
					true,
					5,
					1000,
					sf::Vector2f(5.0f, 5.0f),
					sf::Vector2f(3.0f, 3.0f),
					rotation - 25.0f,
					rotation + 25.0f,
					150.0f,
					250.0f,
					0.0f,
					0.1f,
					0.2f,
					sf::Color(255, 25, 25, 255),
					sf::Color(255, 25, 25, 255)));

			}
		}

		//Neighboring partition's zombies
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& zombie : partition->vZombies_)
				{
					if (isColliding(zombie.getHeadSprite(), bullet.getSprite(), bullet.getLastPosition()) == true && zombie.getHealth() > 0)
					{
						zombie.injure();
						zombie.setHealth(zombie.getHealth() - bullet.getDamage());
						bullet.setHit(true);
						pSoundManager_->playSound("hit");

						vEmitters_.push_back(Emitter(true,
							position,
							true,
							5,
							1000,
							sf::Vector2f(5.0f, 5.0f),
							sf::Vector2f(3.0f, 3.0f),
							rotation - 25.0f,
							rotation + 25.0f,
							150.0f,
							250.0f,
							0.0f,
							0.1f,
							0.2f,
							sf::Color(255, 25, 25, 255),
							sf::Color(255, 25, 25, 255)));
					}
				}

		//This partition's trees
		for (auto& tree : vTrees_)
		{
			if ((isColliding(tree.getTrunk(), bullet.getSprite(), bullet.getLastPosition())) == true)
			{
				bullet.setHit(true);

				vEmitters_.push_back(Emitter(true,
					position,
					true,
					5,
					1000,
					sf::Vector2f(5.0f, 5.0f),
					sf::Vector2f(3.0f, 3.0f),
					rotation - 25.0f,
					rotation + 25.0f,
					150.0f,
					250.0f,
					0.0f,
					0.1f,
					0.2f,
					sf::Color(93, 64, 40, 255),
					sf::Color(93, 64, 40, 255)));
			}
		}

		//Neighboring partition's trees
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& tree : partition->vTrees_)
				{
					if ((isColliding(tree.getTrunk(), bullet.getSprite(), bullet.getLastPosition())) == true)
					{
						bullet.setHit(true);

						vEmitters_.push_back(Emitter(true,
							position,
							true,
							5,
							1000,
							sf::Vector2f(5.0f, 5.0f),
							sf::Vector2f(3.0f, 3.0f),
							rotation - 25.0f,
							rotation + 25.0f,
							150.0f,
							250.0f,
							0.0f,
							0.1f,
							0.2f,
							sf::Color(93, 64, 40, 255),
							sf::Color(93, 64, 40, 255)));
					}
				}
	}
	for (auto& zombie : vZombies_)
	{
		sf::Sprite zombieHeadSprite(zombie.getHeadSprite());

		if (!zombie.isDead())
		{
			//This partition's zombies
			for (auto& zombie2 : vZombies_)
			{ 
				if (!zombie2.isDead() && &zombie != &zombie2)
					zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombieHeadSprite, zombie2.getHeadSprite()));
			}
			//Neigboring partition's zombies
			for (auto& partition : pSpatialPartitions_)
				if (partition != nullptr)
					for (auto& zombie2 : partition->vZombies_)
						if (!zombie2.isDead())
							zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombieHeadSprite, zombie2.getHeadSprite()));
		}

		//These collision checks should still occur when dead

		//This partition's trees
		for (auto& tree : vTrees_)
		{
			sf::Vector2f collisionResolution = isColliding(zombieHeadSprite, tree.getTrunk());
			if (collisionResolution != sf::Vector2f(0.0f, 0.0f))
			{
				zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombieHeadSprite, tree.getTrunk()));
				zombie.setNeedsPath(true);
			}
		}
				
		//Neighboring partitions's tree
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& tree : partition->vTrees_)
				{
					sf::Vector2f collisionResolution = isColliding(zombieHeadSprite, tree.getTrunk());
					if (collisionResolution != sf::Vector2f(0.0f, 0.0f))
					{
						zombie.setPositionGlobal(zombie.getPositionGlobal() - isColliding(zombieHeadSprite, tree.getTrunk()));
						zombie.setNeedsPath(true);
					}
				}
		//This partition's barricades
		for (auto& barricade : vBarricades_)
			zombie.setPositionGlobal(zombie.getPositionGlobal() + isColliding(barricade.getSprite(), zombieHeadSprite));
		
		//Neighboring partition's barricades
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& barricade : partition->vBarricades_)
					zombie.setPositionGlobal(zombie.getPositionGlobal() + isColliding(barricade.getSprite(), zombieHeadSprite));


		//This partition's turret
		for (auto& turret : vTurrets_)
			zombie.setPositionGlobal(zombie.getPositionGlobal() + isColliding(turret.getBaseSprite(), zombieHeadSprite));

		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& turret : vTurrets_)
					zombie.setPositionGlobal(zombie.getPositionGlobal() + isColliding(turret.getBaseSprite(), zombieHeadSprite));

		for(auto& mine : vMines_)
		    if(isColliding(zombie.getHeadSprite(), mine.getMine()) != sf::Vector2f(0.0f, 0.0f))
			mine.explode();

		for(auto& partition : pSpatialPartitions_)
		    for(auto& mine : vMines_)
			if(isColliding(zombie.getHeadSprite(), mine.getMine()) != sf::Vector2f(0.0f, 0.0f))
			    mine.explode();

		//Unwalkable collision checks
		for (int i = 0; i < 8; ++i)
		{
			int xPos;
			int yPos;

			//xPos
			if (i == 0 || i == 4)
				xPos = 0;
			else if (i > 0 && i < 4)
				xPos = 1;
			else
				xPos = -1;

			//yPos
			if (i == 2 || i == 6)
				yPos = 0;
			else if (i < 2 || i > 6)
				yPos = 1;
			else
				yPos = -1;

			sf::Vector2f zombiePosition(zombie.getPositionGlobal());
			sf::Vector2u zombieTilePos((zombiePosition.x - fmod(zombiePosition.x, 32.0f) + 16) / 32, (zombiePosition.y - fmod(zombiePosition.y, 32.0f) + 16) / 32);
			std::string tileType = pVTiles_->at(zombieTilePos.x + xPos).at(zombieTilePos.y + yPos).getType();
			if (tileType == "rock" || tileType == "water")
			{
				zombie.setPositionGlobal(zombiePosition + isColliding(pVTiles_->at(zombieTilePos.x + xPos).at(zombieTilePos.y + yPos).getSprite(), zombie.getHeadSprite()));
				zombie.setNeedsPath(true);
			}
		}
	}

	if (hasPlayer_)
	{
		sf::Sprite playerHeadSprite(player_->getHeadSprite());
		//This partition's zombies
		for (auto& zombie : vZombies_)
		{
			if (!zombie.isDead())
				player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(playerHeadSprite, zombie.getHeadSprite()));
				player_->pushLightingSprite(zombie.getHeadSprite());
		}
		//Neigboring partition's zombies
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& zombie : partition->vZombies_)
				{
					player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(playerHeadSprite, zombie.getHeadSprite()));
					player_->pushLightingSprite(zombie.getHeadSprite());
				}
		//This partition's turrets
		for (auto& turret : vTurrets_)
		{
			player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(playerHeadSprite, turret.getBaseSprite()));
			player_->pushLightingSprite(turret.getBaseSprite());
		}
		//Neighboring partition's turrets
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& turret : partition->getTurrets())
				{
					player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(playerHeadSprite, turret.getBaseSprite()));
					player_->pushLightingSprite(turret.getBaseSprite());
				}

		//This partition's trees
		for (auto& tree : vTrees_)
		{
			player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(playerHeadSprite, tree.getTrunk()));
			player_->pushLightingSprite(tree.getTrunk());
		}
		//Neighboring partitions's tree
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& tree : partition->vTrees_)
				{
					player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(playerHeadSprite, tree.getTrunk()));
					player_->pushLightingSprite(tree.getTrunk());
				}
		//This partition's barricades
		for (auto& barricade : vBarricades_)
		{
			player_->setPositionGlobal(player_->getPositionGlobal() + isColliding(barricade.getSprite(), playerHeadSprite));
			player_->pushLightingSprite(barricade.getSprite());
		}
		//Neighboring partition's barricades
		for (auto& partition : pSpatialPartitions_)
			if (partition != nullptr)
				for (auto& barricade : partition->vBarricades_)
				{
					player_->setPositionGlobal(player_->getPositionGlobal() + isColliding(barricade.getSprite(), playerHeadSprite));
					player_->pushLightingSprite(barricade.getSprite());
				}
		//Unwalkable collision checks
		for (int i = 0; i < 8; ++i)
		{
			int xPos;
			int yPos;

			//xPos
			if (i == 0 || i == 4)
				xPos = 0;
			else if (i > 0 && i < 4)
				xPos = 1;
			else
				xPos = -1;

			//yPos
			if (i == 2 || i == 6)
				yPos = 0;
			else if (i < 2 || i > 6)
				yPos = 1;
			else
				yPos = -1;

			sf::Vector2f playerPosition = player_->getPositionGlobal();
			sf::Vector2u playerTilePos((playerPosition.x - fmod(playerPosition.x, 32.0f) + 16) / 32, (playerPosition.y - fmod(playerPosition.y, 32.0f) + 16) / 32);
			if (playerTilePos.x + xPos >= 0 && playerTilePos.y + yPos >= 0 && playerTilePos.x + xPos < 256 && playerTilePos.y + yPos < 256)
			{
				std::string tileType = pVTiles_->at(playerTilePos.x + xPos).at(playerTilePos.y + yPos).getType();
				if (tileType == "rock" || tileType == "water")
					player_->setPositionGlobal(playerPosition + isColliding(pVTiles_->at(playerTilePos.x + xPos).at(playerTilePos.y + yPos).getSprite(), player_->getHeadSprite()));
			}
		}
	}

	//Draw selection rect
	if (selecting_)
	{
		selectionRect_.setPosition(pAboveTile->getSprite().getPosition());
		if (pAboveTile->getType() == "walkable" && !pAboveTile->hasItem())
			selectionRect_.setFillColor(sf::Color(0, 200, 50, 25));
		else
			selectionRect_.setFillColor(sf::Color(200, 0, 50, 25));
	}
	else
		selectionRect_.setFillColor(sf::Color::Transparent);

	if (clickTurretDown_ && !sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		clickTurretDown_ = false;
	}
	if (clickBarricadeDown_ && !sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
	{
		clickBarricadeDown_ = false;
	}
	if(clickMineDown_ && !sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
	{
	    clickMineDown_ = false;
	}
	for (auto& emitter : vEmitters_)
		emitter.update(dT);
}

//Setters
void SpatialPartition::setImageManagerPointer(ImageManager* imageManager) { imageManager_ = imageManager; }
void SpatialPartition::setTilesPointer(std::vector<std::vector<Tile> >* pTiles) { pVTiles_ = pTiles; }


//Getters
sf::FloatRect SpatialPartition::getPartitionSpace() const { return partitionSpace_; }
sf::RectangleShape SpatialPartition::getSelectionRect() const { return selectionRect_; }
std::vector<Zombie> SpatialPartition::getZombies() const { return vZombies_; }
std::list<Bullet> SpatialPartition::getBullets() const { return lBullets_; }
std::vector<Tree> SpatialPartition::getTrees() const { return vTrees_; }
std::vector<Den> SpatialPartition::getDens() const { return vDens_; }
std::deque<BloodSplat> SpatialPartition::getBloodSplats() const { return dBloodSplats_; }
std::vector<Turret> SpatialPartition::getTurrets() const { return vTurrets_; }
std::vector<Barricade> SpatialPartition::getBarricades() const { return vBarricades_; }
std::vector<Mine> SpatialPartition::getMines() const { return vMines_; }
std::array<SpatialPartition*, 8> SpatialPartition::getNeigborPartitions() const { return pSpatialPartitions_; }
std::vector<Emitter> SpatialPartition::getEmitters() const { return vEmitters_; }

//Pushers
void SpatialPartition::pushZombie(const Zombie& zombie) { vZombies_.push_back(zombie); }
void SpatialPartition::pushBullet(const Bullet& bullet) { lBullets_.push_back(bullet); }
void SpatialPartition::pushTree(const Tree& tree) { vTrees_.push_back(tree); }
void SpatialPartition::pushDen(const Den& den) { vDens_.push_back(den); }
void SpatialPartition::pushBloodSplat(const BloodSplat& splat) { dBloodSplats_.push_back(splat); }
