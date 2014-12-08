#ifndef SPATIALPARTITION_H
#define	SPATIALPARTITION_H
#include <vector>
#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "Bullet.h"
#include "Tree.h"
#include "Player.h"

class SpatialPartition 
{
public:
    SpatialPartition(const sf::FloatRect&, Player*, std::vector<SpatialPartition>*);
    void update(const sf::Time&);
    
    //Setters
    
    //Getters
    sf::FloatRect getPartitionSpace() const;
    std::vector<Zombie> getZombies() const;
    std::list<Bullet> getBullets() const;
    std::vector<Tree> getTrees() const;
    
    //Pushers
    void pushZombie(const Zombie&);
    void pushBullet(const Bullet&);
    void pushTree(const Tree&);

private:
    std::vector<Zombie> vZombies_;
    std::list<Bullet> lBullets_;
    std::vector<Tree> vTrees_;
    Player* player_;
    std::vector<SpatialPartition>* partitions_;
    bool hasPlayer_ = false;
    sf::FloatRect partitionSpace_;
};

#endif	

