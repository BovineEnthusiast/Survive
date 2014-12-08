#include "SpatialPartition.h"
#include "Collision.h"
#include <iostream>

SpatialPartition::SpatialPartition(const sf::FloatRect& partitionSpace, Player* player, std::vector<SpatialPartition>* partitions)
:partitionSpace_(partitionSpace), player_(player), partitions_(partitions)
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

    for(auto iZombies = vZombies_.begin(); iZombies != vZombies_.end();)
    {
        std::cout << "zombie loop" << std::endl;
          
        //Checks player collision
        if(hasPlayer_ && isColliding(player_->getHeadSprite(), iZombies->getHeadSprite()))
        {
            player_->setPosition(player_->getPositionGlobal() - player_->getVelocity() * dT.asSeconds() * 20.0f);
        }
        for(auto iBullet = lBullets_.begin(); iBullet != lBullets_.end(); ++iBullet)
        {
            if((isColliding(iZombies->getHeadSprite(), iBullet->getSprite(), iBullet->getLastPosition())) == true)            
            {
                // std::cout << "zombie---bullet loop" << std::endl;
                iZombies->setHealth(iZombies->getHealth() - 25);
                iBullet->setHit(true);
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
    for(auto iTree = vTrees_.begin(); iTree != vTrees_.end(); ++iTree)
    {
         //std::cout << "tree loop" << std::endl;
        iTree->update(dT);
       
        //Checks for collisions
        sf::Vector2f treePos = iTree->getTrunk().getPosition();
        
        if(hasPlayer_ && isColliding(player_->getHeadSprite(), iTree->getTrunk()))
        {
            player_->setPosition(player_->getPositionGlobal() - player_->getVelocity() * dT.asSeconds() * 20.0f);
        }
        
        for(auto iBullet = lBullets_.begin(); iBullet != lBullets_.end(); ++iBullet)
        {
             //std::cout << "tree---bullet loop" << std::endl;
            if((isColliding(iTree->getTrunk(), iBullet->getSprite(), iBullet->getLastPosition())) == true)
                iBullet->setHit(true);
        }
        
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

//Getters
sf::FloatRect SpatialPartition::getPartitionSpace() const {return partitionSpace_;}
std::vector<Zombie> SpatialPartition::getZombies() const {return vZombies_;}
std::list<Bullet> SpatialPartition::getBullets() const {return lBullets_;}
std::vector<Tree> SpatialPartition::getTrees() const {return vTrees_;}

//Pushers
void SpatialPartition::pushZombie(const Zombie& zombie) {vZombies_.push_back(zombie);}
void SpatialPartition::pushBullet(const Bullet& bullet) {lBullets_.push_back(bullet);}
void SpatialPartition::pushTree(const Tree& tree) {vTrees_.push_back(tree);}