#ifndef TILEITEM_H
#define	TILEITEM_H
#include "GameObject.h"
#include <SFML/Graphics.hpp>
class TileItem : public GameObject
{
public:
    TileItem(const std::string&);
    void update(const sf::Time&);
    
    //Getters
    std::string getType();
    sf::Sprite getSprite();
    int getHealth();
    
    //Setters
    void setHealth(const int);
private:
    std::string type_ = "";
    
    //Graphical
    sf::Texture tileItemSpriteSheet_;
    sf::Sprite tileSprite_;
    
    
    //Item properties
    int health_ = 100;
    int rewardMin_ = 2;
    int rewardMax_ = 4;
    
};

#endif	

