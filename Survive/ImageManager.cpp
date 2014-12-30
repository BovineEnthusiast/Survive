#include "ImageManager.h"

ImageManager::ImageManager() 
{
	tileSheetTexture.loadFromFile("tiles.jpg");
    //Guns
    gunPistolTexture.loadFromFile("gunSpriteSheet.png");
    gunRifleTexture.loadFromFile("rifle.png");
    
    treeUpperLeafTexture.loadFromFile("tree_upper.png");
    treeLowerLeafTexture.loadFromFile("tree_lower.png");
    treeTrunkTexture.loadFromFile("tree_trunk.png");
    humanoidZombieTexture.loadFromFile("zombie.png");
    humanoidPlayerTexture.loadFromFile("player.png");
    zombieDenTexture.loadFromFile("den.png");
    
    //Blood splatters
    vBloodSplatTextures.push_back(sf::Texture());
    vBloodSplatTextures.push_back(sf::Texture());
    vBloodSplatTextures.at(0).loadFromFile("blood/splat_1.png");
    vBloodSplatTextures.at(1).loadFromFile("blood/splat_2.png");
}
