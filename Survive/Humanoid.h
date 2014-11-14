#ifndef HUMANOID_H
#define	HUMANOID_H
#include "GameObject.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"


class Humanoid : public GameObject
{
public:
    Humanoid();
    void animate(const sf::Time&); 
    std::vector<std::vector<Tile>>* pTiles;
 
protected:

    //Body sprite sheet
    sf::Texture bodySpriteSheet_;
    
    //Body sprites
    sf::Sprite legLeftSprite_;
    sf::Sprite legRightSprite_;
    sf::Sprite armLeftSprite_;
    sf::Sprite armRightSprite_;
    sf::Sprite headSprite_;
    
    sf::Vector2f velocity_;
    sf::Vector2f lookPosition_;
    sf::Vector2f armLeftTarget_;
    sf::Vector2f armRightTarget_;
    sf::Clock legAnimClock_;
    float legAnimLength_;
    
private:
    
    sf::Clock sinClock;
        
};

#endif	
