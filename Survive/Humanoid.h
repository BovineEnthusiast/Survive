#ifndef HUMANOID_H
#define	HUMANOID_H
#include <SFML/Graphics.hpp>
#include "GameObject.h"
class Humanoid : public GameObject
{
public:
    Humanoid();
    void animate(const sf::Time&); 
 
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
