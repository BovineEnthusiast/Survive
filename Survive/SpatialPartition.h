#ifndef SPATIALPARTITION_H
#define	SPATIALPARTITION_H
#include <array>
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
#include "SoundManager.h"
#include "Turret.h"

class SpatialPartition 
{
public:
	SpatialPartition(const sf::FloatRect&, Player*, std::vector<std::vector<SpatialPartition>>*, SoundManager*, int*, int*, int*);
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
	std::vector<Turret> getTurrets() const;
    std::deque<BloodSplat> getBloodSplats() const;
	std::array<SpatialPartition*, 8> getNeigborPartitions() const;
	int getPoints();
    //Pushers
    void pushZombie(const Zombie&);
    void pushBullet(const Bullet&);
    void pushTree(const Tree&);
    void pushDen(const Den&);
    void pushBloodSplat(const BloodSplat&);
  

private:

	int xPos_;
	int yPos_;
	
	bool firstUpdate_ = true;
	//Neighboring partitions
	std::array<SpatialPartition*, 8> pSpatialPartitions_;
	int points_ = 0;
	int* pZombiesAlive_;
	int* pZombiesToSpawn_;
	int* pWave_;
    std::vector<Zombie> vZombies_;
    std::list<Bullet> lBullets_;
    std::vector<Tree> vTrees_;
    std::vector<Den> vDens_;
	std::vector<Turret> vTurrets_;
    std::deque<BloodSplat> dBloodSplats_;
    Player* player_;
    std::vector<std::vector<SpatialPartition>>* partitions_;
    bool hasPlayer_ = false;
    sf::FloatRect partitionSpace_;
    ImageManager* imageManager_;
    SoundManager* pSoundManager_;
    std::vector<std::vector<Tile>>* pVTiles_;

	//temp 
	sf::Clock turretClock_; 
};

#endif	

