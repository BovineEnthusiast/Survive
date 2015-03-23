#include "ImageManager.h"

ImageManager::ImageManager()
{
  tileSheetTexture.loadFromFile("assets/tiles.jpg");
  //Guns
  gunPistolTexture.loadFromFile("assets/guns/pistol.png");
  gunRocketTexture.loadFromFile("assets/guns/rocket.png");
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
  zombieRangedCorpseTexture.loadFromFile("assets/humanoid/zombie_ranged_corpse.png");
  zombieTankCorpseTexture.loadFromFile("assets/humanoid/zombie_tank_corpse.png");
  humanoidZombieTexture.loadFromFile("assets/humanoid/zombie_new.png");
  humanoidPlayerTexture.loadFromFile("assets/humanoid/player_new.png");
  humanoidBoomTexture.loadFromFile("assets/humanoid/zombie_boom.png");
  humanoidRangedTexture.loadFromFile("assets/humanoid/zombie_ranged.png");
  humanoidTankTexture.loadFromFile("assets/humanoid/zombie_tank.png");
  zombieDenTexture.loadFromFile("assets/den.png");
  healthkitTexture.loadFromFile("assets/healthkit.png");

  //Store pics
  storePistol.loadFromFile("assets/store/pistol.png");
  storeMagnum.loadFromFile("assets/store/magnum.png");
  storeShotgun.loadFromFile("assets/store/shotgun.png");
  storeRifle.loadFromFile("assets/store/rifle.png");
  storeRocket.loadFromFile("assets/store/rocket.png");
  storeBarricade.loadFromFile("assets/store/barricade.png");
  storeTurret.loadFromFile("assets/store/turret.png");
  storeMine.loadFromFile("assets/store/mine.png");

  storePistol.setSmooth(true);
  storeMagnum.setSmooth(true);
  storeShotgun.setSmooth(true);
  storeRifle.setSmooth(true);
  storeRocket.setSmooth(true);
  storeBarricade.setSmooth(true);
  storeTurret.setSmooth(true);
  storeMine.setSmooth(true);

  //Blood splatters
  vBloodSplatTextures.push_back(sf::Texture());
  vBloodSplatTextures.push_back(sf::Texture());
  vBloodSplatTextures.at(0).loadFromFile("assets/blood/splat_1.png");
  vBloodSplatTextures.at(1).loadFromFile("assets/blood/splat_2.png");
}
