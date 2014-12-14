#ifndef HUMANOID_H
#define	HUMANOID_H
#include "GameObject.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"


class Humanoid : public GameObject
{
public:
    Humanoid(sf::Texture*);
    void animate(const sf::Time&); 
    std::vector<std::vector<Tile>>* pTiles;
 
    //Getters
    sf::Sprite getLegLeftSprite();
    sf::Sprite getLegRightSprite();
    sf::Sprite getArmLeftSprite();
    sf::Sprite getArmRightSprite();
    sf::Sprite getHeadSprite();
    sf::Vector2f getVelocity();
    int getHealth();
    
    
    //Setters
    void setVelocity(const sf::Vector2f&);
    void setHealth(const int);
protected:

    //Humanoid properties
    int health_ = 100;
    float speed_ = 4;
        
    //Body sprites
    sf::Texture* bodySpriteSheet_;
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
