#include "ImageManager.h"

ImageManager::ImageManager() 
{
	tileSheetTexture.loadFromFile("tiles.jpg");
    //Guns
    gunPistolTexture.loadFromFile("guns/pistol.png");
    gunRifleTexture.loadFromFile("guns/rifle.png");
	gunShotgunTexture.loadFromFile("guns/shotgun.png");
	gunMagnumTexture.loadFromFile("guns/magnum.png");
	turretBaseTexture.loadFromFile("guns/turret/base.png");
	turretTexture.loadFromFile("guns/turret/turret.png");
	barricadeTexture.loadFromFile("guns/barricade.png");

    treeUpperLeafTexture.loadFromFile("tree_upper.png");
    treeLowerLeafTexture.loadFromFile("tree_lower.png");
    treeTrunkTexture.loadFromFile("tree_trunk.png");
	zombieCorpseTexture.loadFromFile("zombie_new_corpse.png");
    humanoidZombieTexture.loadFromFile("zombie_new.png");
    humanoidPlayerTexture.loadFromFile("player_new.png");
    zombieDenTexture.loadFromFile("den.png");
	    
    //Blood splatters
    vBloodSplatTextures.push_back(sf::Texture());
    vBloodSplatTextures.push_back(sf::Texture());
    vBloodSplatTextures.at(0).loadFromFile("blood/splat_1.png");
    vBloodSplatTextures.at(1).loadFromFile("blood/splat_2.png");
}
