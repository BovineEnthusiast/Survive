#include "ImageManager.h"

ImageManager::ImageManager() 
{
  tileSheetTexture.loadFromFile("assets/tiles.jpg");
  //Guns
  gunPistolTexture.loadFromFile("assets/guns/pistol.png");
  gunRifleTexture.loadFromFile("assets/guns/rifle.png");
  gunShotgunTexture.loadFromFile("assets/guns/shotgun.png");
  gunMagnumTexture.loadFromFile("assets/guns/magnum.png");
  turretBaseTexture.loadFromFile("assets/guns/turret/base.png");
  turretTexture.loadFromFile("assets/guns/turret/turret.png");
  barricadeTexture.loadFromFile("assets/guns/barricade.png");
  mineTexture.loadFromFile("assets/guns/mine.png");
  treeUpperLeafTexture.loadFromFile("assets/tree/tree_upper.png");
  treeLowerLeafTexture.loadFromFile("assets/tree/tree_lower.png");
  treeTrunkTexture.loadFromFile("assets/tree/tree_trunk.png");
  zombieCorpseTexture.loadFromFile("assets/humanoid/zombie_new_corpse.png");
  humanoidZombieTexture.loadFromFile("assets/humanoid/zombie_new.png");
  humanoidPlayerTexture.loadFromFile("assets/humanoid/player_new.png");
  zombieDenTexture.loadFromFile("assets/den.png");
  
  //Blood splatters
  vBloodSplatTextures.push_back(sf::Texture());
  vBloodSplatTextures.push_back(sf::Texture());
  vBloodSplatTextures.at(0).loadFromFile("assets/blood/splat_1.png");
  vBloodSplatTextures.at(1).loadFromFile("assets/blood/splat_2.png");
}
