#ifndef LEVEL_H
#define	LEVEL_H
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Zombie.h"
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
    std::vector<Zombie> getZombies();
    
    //Setters
    void setCameraPosition(const sf::Vector2f&);
    void setPlayerWindow(sf::RenderWindow&);
private:
    sf::Texture tileSpriteSheet_;
        
   Camera camera_;    
        
    //A map of IntRect locations on the sprite that take a name key.
    std::map<std::string, sf::IntRect> tileSprites_; 
    Player player_;
    //Will only be one at most
    //std::vector<Player> players_;
    
    //Stores all the zombies
    std::vector<Zombie> vZombies_;

    
    //Used to tell whether level is a menu
    bool menu_ = false;
};

#endif

