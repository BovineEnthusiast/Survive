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
#include "GUIManager.h"
#include "SpatialPartition.h"
class Level 
{
public:
    Level(sf::RenderWindow*);
    
    void update(const sf::Time&);
    void generateLevel(const int width, const int height); //Fills the tiles vector with procedural tiles
    std::vector< std::vector<Tile> > tiles;

    //Camera
    void moveCamera(const sf::Vector2f&);
    void zoomCamera(const int);
    void resizeCamera(const sf::Vector2u&);
    
    //Getters
    bool isMenu() const;
    sf::View getCameraView();
    Player getPlayer() const;
    std::vector<SpatialPartition> getSpatialPartitions() const;
    GUIManager getGUIManager() const;
	bool hasLost();
    
    //Setters
    void setCameraPosition(const sf::Vector2f&);
private:
    sf::RenderWindow* pWindow_;
    sf::Texture tileSpriteSheet_;
    ImageManager imageManager_;    
    SoundManager soundManager_;
    Player player_ = Player(&imageManager_.humanoidPlayerTexture, &imageManager_, &soundManager_);
    GUIManager GUIManager_ = GUIManager(pWindow_, &player_);

    Camera camera_;    

	//True if the player dies
	bool lost_ = false;
    //The amount of time between zombie spawns in ms
    int zombieSpawnTime_ = 5.0f;
    
    //A map of IntRect locations on the sprite that take a name key.
    std::map<std::string, sf::IntRect> tileSprites_; 
    
     //Stores all the different objects in the level
    std::vector<SpatialPartition> spatialPartitions_;
    
    //Used to tell whether level is a menu
    bool menu_ = false;

    //Clocks
    sf::Clock zombieSpawnClock_;
};

#endif

