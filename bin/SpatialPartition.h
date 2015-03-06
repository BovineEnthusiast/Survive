#ifndef SPATIALPARTITION_H
#define	SPATIALPARTITION_H
#include <array>
#include <vector>
#include <deque>
#include <SFML/Graphics.hpp>
#include "Barricade.h"
#include "BloodSplat.h"
#include "Bullet.h"
#include "Den.h"
#include "ImageManager.h"
#include "Emitter.h"
#include "Mine.h"
#include "Player.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Tree.h"
#include "Turret.h"
#include "Zombie.h"

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
  std::vector<Barricade> getBarricades() const;
  std::vector<Mine> getMines() const;
  std::deque<BloodSplat> getBloodSplats() const;
  std::array<SpatialPartition*, 8> getNeigborPartitions() const;
  sf::RectangleShape getSelectionRect() const;
  std::vector<Emitter> getEmitters() const;

  //Pushers
  void pushZombie(const Zombie&);
  void pushBullet(const Bullet&);
  void pushTree(const Tree&);
  void pushDen(const Den&);
  void pushBloodSplat(const BloodSplat&);
  

 private:
  //Used to select the area to place items before placing them
  static sf::RectangleShape selectionRect_;
  static bool selecting_;
  
  //Used to determine clicks for turret/barricade placement
  static bool clickTurretDown_;
  static bool clickBarricadeDown_;
  static bool clickMineDown_;
  int xPos_;
  int yPos_;
  sf::Vector2f lastPlayerPos_;
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
  std::vector<Barricade> vBarricades_;
  std::vector<Mine> vMines_;
  std::vector<Emitter> vEmitters_; //Used for zombie blood
  std::deque<BloodSplat> dBloodSplats_;
  Player* player_;
  std::vector<std::vector<SpatialPartition>>* partitions_;
  bool hasPlayer_ = false;
  sf::FloatRect partitionSpace_;
  ImageManager* imageManager_;
  SoundManager* pSoundManager_;
  std::vector<std::vector<Tile>>* pVTiles_;
  
  //Clocks
  sf::Clock targetClock_;
  sf::Clock pathClock_;
  
};

#endif	

