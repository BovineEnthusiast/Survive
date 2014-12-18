#ifndef SPATIALPARTITION_H
#define	SPATIALPARTITION_H
#include <vector>
#include <deque>
#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "Bullet.h"
#include "Tree.h"
#include "Tile.h"
#include "Player.h"
#include "Den.h"
#include "BloodSplat.h"
#include "ImageManager.h"

class SpatialPartition 
{
public:
    SpatialPartition(const sf::FloatRect&, Player*, std::vector<SpatialPartition>*);
    void update(const sf::Time&);
    
    //Setters
    void setImageManagerPointer(ImageManager*);
    void setTilesPointer(std::vector<std::vector<Tile>>*);
    //Getters
    sf::FloatRect getPartitionSpace() const;
    std::vector<Zombie> getZombies() const;
    std::list<Bullet> getBullets() const;
    std::vector<Tree> getTrees() const;
    std::vector<Den> getDens() const;
    std::deque<BloodSplat> getBloodSplats() const;
   
    //Pushers
    void pushZombie(const Zombie&);
    void pushBullet(const Bullet&);
    void pushTree(const Tree&);
    void pushDen(const Den&);
    void pushBloodSplat(const BloodSplat&);
  

private:
    std::vector<Zombie> vZombies_;
    std::list<Bullet> lBullets_;
    std::vector<Tree> vTrees_;
    std::vector<Den> vDens_;
    std::deque<BloodSplat> dBloodSplats_;
    Player* player_;
    std::vector<SpatialPartition>* partitions_;
    bool hasPlayer_ = false;
    sf::FloatRect partitionSpace_;
    ImageManager* imageManager_;
    std::vector<std::vector<Tile>>* pVTiles_;
};

#endif	

