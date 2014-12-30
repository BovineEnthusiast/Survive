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
    sf::Texture humanoidZombieTexture;
    sf::Texture humanoidPlayerTexture;
    sf::Texture gunPistolTexture;
    sf::Texture gunRifleTexture;
    sf::Texture zombieDenTexture;
    std::vector<sf::Texture> vBloodSplatTextures;
};

#endif	

