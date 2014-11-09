#include "Humanoid.h"
#include <math.h>
Humanoid::Humanoid() 
{
    //Assigns the texture and the texture rects
    legLeftSprite_.setTexture(bodySpriteSheet_);
    legLeftSprite_.setTextureRect(sf::IntRect(43, 17, 54, 32));
    legRightSprite_.setTexture(bodySpriteSheet_);
    legRightSprite_.setTextureRect(sf::IntRect(43, 17, 54, 32));
    armLeftSprite_.setTexture(bodySpriteSheet_);
    armLeftSprite_.setTextureRect(sf::IntRect(34, 5, 43, 32));
    armRightSprite_.setTexture(bodySpriteSheet_);
    armRightSprite_.setTextureRect(sf::IntRect(34, 5, 43, 32));
    headSprite_.setTexture(bodySpriteSheet_);
    headSprite_.setTextureRect(sf::IntRect(0, 0, 34, 32));
    
    //Assigns their origins
    legLeftSprite_.setOrigin(5.5f, 7.5f);
    legRightSprite_.setOrigin(5.5f, 7.5f);
    armLeftSprite_.setOrigin(4.5f, 27.0f);
    armRightSprite_.setOrigin(4.5f, 27.0f);
    headSprite_.setOrigin(17.0f, 16.0f);
}

void Humanoid::animate()
{
    //REMEMBER: normalized -1/(y/x)
    if(velocity_ != sf::Vector2f(0, 0))
    {
        
    }
}
