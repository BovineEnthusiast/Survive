#ifndef DEN_H
#define	DEN_H
#include "GameObject.h"

class Den : public GameObject {
public:
    Den(sf::Texture*);
    void update(const sf::Time&);
    bool isReadyToSpawn() const;
    
    //Setters
    //Getters
    sf::Sprite getSprite() const;
private:
    sf::Sprite denSprite_;
    sf::Texture* denTexture_;
    
  
    //Spawn properties
    bool readyToSpawn_ = false;
    int spawnEvery_ = 5000;
    int decreaseSpawnClockEvery_ = 15000;
    int decreaseSpawnBy_ = 100;
    sf::Clock spawnClock_;
    sf::Clock spawnSpeedClock_;
};

#endif	

