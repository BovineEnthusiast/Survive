#ifndef LEVEL_H
#define	LEVEL_H
#include <vector>
#include <map>
#include <list>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Tree.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "SpatialPartition.h"
class Level 
{
public:
    Level();
    
    void update(const sf::Time&);
    void generateLevel(const int width, const int height); //Fills the tiles vector with procedural tiles
    std::vector< std::vector<Tile> > tiles;

    //Camera
    void moveCamera(const sf::Vector2f&);
    void zoomCamera(const int);
    void resizeCamera(const sf::Vector2u&);
    
    //Getters
    bool isMenu();
    sf::View getCameraView();
    Player getPlayer();
    std::vector<SpatialPartition> getSpatialPartitions() const;
    //Setters
    void setCameraPosition(const sf::Vector2f&);
    void setPlayerWindow(sf::RenderWindow&);
private:
    sf::Texture tileSpriteSheet_;
    ImageManager imageManager_;    
    SoundManager soundManager_;
    Camera camera_;    
        
    //Number of zombies that can be on screen
    int maxZombies_ = 5;
    //The amount of time between zombie spawns in ms
    int zombieSpawnTime_ = 5.0f;
    
    //A map of IntRect locations on the sprite that take a name key.
    std::map<std::string, sf::IntRect> tileSprites_; 
    
   
    Player player_ = Player(&imageManager_.humanoidPlayerTexture, &imageManager_, &soundManager_);
    
     //Stores all the different objects in the level
    std::vector<SpatialPartition> spatialPartitions_;
    
    //Used to tell whether level is a menu
    bool menu_ = false;

    //Clocks
    sf::Clock zombieSpawnClock_;
};

#endif

