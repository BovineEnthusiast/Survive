#include "ImageManager.h"

ImageManager::ImageManager() 
{
    gunPistolTexture.loadFromFile("gunSpriteSheet.png");
    treeUpperLeafTexture.loadFromFile("tree_upper.png");
    treeLowerLeafTexture.loadFromFile("tree_lower.png");
    treeTrunkTexture.loadFromFile("tree_trunk.png");
    humanoidZombieTexture.loadFromFile("zombie.png");
    humanoidPlayerTexture.loadFromFile("player.png");
}
