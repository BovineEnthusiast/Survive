#ifndef IMAGEMANAGER_H
#define	IMAGEMANAGER_H
#include <SFML/Graphics.hpp>
class ImageManager 
{
 public:
  ImageManager();
  sf::Texture tileSheetTexture;
  sf::Texture treeUpperLeafTexture;
  sf::Texture treeLowerLeafTexture;
  sf::Texture treeTrunkTexture;
  sf::Texture zombieCorpseTexture;
  sf::Texture humanoidZombieTexture;
  sf::Texture humanoidPlayerTexture;
  sf::Texture gunPistolTexture;
  sf::Texture gunMagnumTexture;
  sf::Texture gunRifleTexture;
  sf::Texture gunRocketTexture;
  sf::Texture gunShotgunTexture;
  sf::Texture turretBaseTexture;
  sf::Texture mineTexture;
  sf::Texture turretTexture;
  sf::Texture barricadeTexture;
  sf::Texture zombieDenTexture;
  sf::Texture humanoidBoomTexture;
  sf::Texture humanoidRangedTexture;
  std::vector<sf::Texture> vBloodSplatTextures;
};

#endif	

