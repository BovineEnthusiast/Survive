#include "SpatialPartition.h"
#include "Collision.h"
#include <iostream>

SpatialPartition::SpatialPartition(const sf::FloatRect& partitionSpace, Player* player, std::vector<SpatialPartition>* partitions, SoundManager* pSoundManager)
:partitionSpace_(partitionSpace), player_(player), partitions_(partitions), pSoundManager_(pSoundManager)
{
}
void SpatialPartition::update(const sf::Time& dT)
{
    //Checks if the player is within this partition
    if(partitionSpace_.contains(player_->getPositionGlobal()))
        hasPlayer_ = true;
    else
        hasPlayer_ = false;
    
    if(hasPlayer_)
    {
        player_->update(dT);
        player_->setGunBulletPointers(&lBullets_);
    }
    
    for(auto iBloodSplat = dBloodSplats_.begin(); iBloodSplat != dBloodSplats_.end();)
    {
        iBloodSplat->update(dT);
        if(iBloodSplat->isFaded())
            iBloodSplat = dBloodSplats_.erase(iBloodSplat);
        else
            ++iBloodSplat;
    }
    for(auto iDen = vDens_.begin(); iDen != vDens_.end(); ++iDen)
    {
        iDen->update(dT);
        if(iDen->isReadyToSpawn())
        {
            Zombie zombie = Zombie(player_, &imageManager_->humanoidZombieTexture);
            zombie.pTiles = pVTiles_;
            zombie.setPosition(iDen->getPositionGlobal());
            vZombies_.push_back(zombie);
        }
    }
    for(auto iZombies = vZombies_.begin(); iZombies != vZombies_.end();)
    {
       // std::cout << "zombie loop" << std::endl;
          
        //Checks player collision
        if(hasPlayer_ && isColliding(player_->getHeadSprite(), iZombies->getHeadSprite()) != sf::Vector2f(-1.0f, -1.0f))
        {
           // player_->setPosition(player_->getPositionGlobal() - player_->getVelocity() * dT.asSeconds() * 20.0f);
            player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(player_->getHeadSprite(), iZombies->getHeadSprite()));
        }
        for(auto iZombies2 = vZombies_.begin(); iZombies2 != vZombies_.end(); ++iZombies2)
            if(iZombies != iZombies2 && isColliding(iZombies->getHeadSprite(), iZombies2->getHeadSprite()) != sf::Vector2f(-1.0f, -1.0f))
                iZombies->setPositionGlobal(iZombies->getPositionGlobal() - isColliding(iZombies->getHeadSprite(), iZombies2->getHeadSprite()));
        
        for(auto iBullet = lBullets_.begin(); iBullet != lBullets_.end(); ++iBullet)
        {
            if((isColliding(iZombies->getHeadSprite(), iBullet->getSprite(), iBullet->getLastPosition())) == true)            
            {
                // std::cout << "zombie---bullet loop" << std::endl;
                iZombies->injure();
                iZombies->setHealth(iZombies->getHealth() - iBullet->getDamage());
                dBloodSplats_.push_back(BloodSplat(&imageManager_->vBloodSplatTextures.at(std::rand() % imageManager_->vBloodSplatTextures.size())));
                dBloodSplats_.at(dBloodSplats_.size() - 1).setPositionGlobal(iBullet->getSprite().getPosition());
                iBullet->setHit(true);
                
                //Sound
                pSoundManager_->playSound("hit");
                
                if(iZombies->getHealth() <= 0)
                    pSoundManager_->playSound("zombie_death");

            }     
        }
         
        iZombies->update(dT);
        
        if(iZombies->getHealth() <= 0)
           iZombies = vZombies_.erase(iZombies);
        else
        {
             //Moves if in another partition
            if(!partitionSpace_.contains(iZombies->getPositionGlobal()))
            {
                for(auto iPartition = partitions_->begin(); iPartition != partitions_->end(); ++iPartition)
                    if(iPartition->getPartitionSpace().contains(iZombies->getPositionGlobal()))
                    {
                        iPartition->pushZombie(*iZombies);
                        iZombies = vZombies_.erase(iZombies);
                        break;
                    }
            }
            else
                ++iZombies;
        }
        
        
    }
    for(auto iTree = vTrees_.begin(); iTree != vTrees_.end();)
    {
        iTree->update(dT);
       
        //Checks for collisions
        sf::Vector2f treePos = iTree->getTrunk().getPosition();
        
        if(hasPlayer_ && isColliding(player_->getHeadSprite(), iTree->getTrunk()) != sf::Vector2f(-1.0f, -1.0f))
        {
           // player_->setPosition(player_->getPositionGlobal() - player_->getVelocity() * dT.asSeconds() * 20.0f);
            player_->setPositionGlobal(player_->getPositionGlobal() - isColliding(player_->getHeadSprite(), iTree->getTrunk()));
                         
                
        }
        
        for(auto iZombies = vZombies_.begin(); iZombies != vZombies_.end(); ++iZombies)
            if(isColliding(iZombies->getHeadSprite(), iTree->getTrunk()) != sf::Vector2f(-1.0f, -1.0f))
                iZombies->setPositionGlobal(iZombies->getPositionGlobal() - isColliding(iZombies->getHeadSprite(), iTree->getTrunk()));
        
        for(auto iBullet = lBullets_.begin(); iBullet != lBullets_.end(); ++iBullet)
        {
             //std::cout << "tree---bullet loop" << std::endl;
            if((isColliding(iTree->getTrunk(), iBullet->getSprite(), iBullet->getLastPosition())) == true)
                iBullet->setHit(true);
        }
        
        if(!partitionSpace_.contains(iTree->getTrunk().getPosition()))
        {
            for(auto iPartition = partitions_->begin(); iPartition != partitions_->end(); ++iPartition)
                if(iPartition->getPartitionSpace().contains(iTree->getTrunk().getPosition()))
                {
                    iPartition->pushTree(*iTree);
                    iTree = vTrees_.erase(iTree);
                    break;
                }
        }
        else
            ++iTree;
        
    }
    for(auto iBullet = lBullets_.begin(); iBullet != lBullets_.end();)
    {
         //std::cout << "bullet loop" << std::endl;
        iBullet->update(dT);
        if(iBullet->isHit() || iBullet->isDead())
           iBullet = lBullets_.erase(iBullet);
        else
        {
            //Moves if in another partition
            if(!partitionSpace_.contains(iBullet->getSprite().getPosition()))
            {
                for(auto iPartition = partitions_->begin(); iPartition != partitions_->end(); ++iPartition)
                    if(iPartition->getPartitionSpace().contains(iBullet->getSprite().getPosition()))
                    {
                        iPartition->pushBullet(*iBullet);
                        iBullet = lBullets_.erase(iBullet);
                        break;
                    }
            }
            else
                ++iBullet;
        }
        
        
    }
  
}

//Setters
void SpatialPartition::setImageManagerPointer(ImageManager* imageManager) {imageManager_ = imageManager;}
void SpatialPartition::setTilesPointer(std::vector<std::vector<Tile> >* pTiles) {pVTiles_ = pTiles;}
//Getters
sf::FloatRect SpatialPartition::getPartitionSpace() const {return partitionSpace_;}
std::vector<Zombie> SpatialPartition::getZombies() const {return vZombies_;}
std::list<Bullet> SpatialPartition::getBullets() const {return lBullets_;}
std::vector<Tree> SpatialPartition::getTrees() const {return vTrees_;}
std::vector<Den> SpatialPartition::getDens() const {return vDens_;}
std::deque<BloodSplat> SpatialPartition::getBloodSplats() const {return dBloodSplats_;}

//Pushers
void SpatialPartition::pushZombie(const Zombie& zombie) {vZombies_.push_back(zombie);}
void SpatialPartition::pushBullet(const Bullet& bullet) {lBullets_.push_back(bullet);}
void SpatialPartition::pushTree(const Tree& tree) {vTrees_.push_back(tree);}
void SpatialPartition::pushDen(const Den& den) {vDens_.push_back(den);}
void SpatialPartition::pushBloodSplat(const BloodSplat& splat) {dBloodSplats_.push_back(splat);}