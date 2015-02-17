#include "TileItem.h"

TileItem::TileItem(const std::string& type)
: type_(type)
{
    tileSprite_.setTexture(tileItemSpriteSheet_);
    
    //Assigns the respective sprite/values for the type
    if(type == "tree")
    {
        health_ = 150;
    }
    else if(type == "boulder")
    {
        health_ = 1000;
    }
    else if(type == "twig")
    {
        health_ = 10;
    }
}

void TileItem::update(const sf::Time& dT){}

//Getters
std::string TileItem::getType() {return type_;}
sf::Sprite TileItem::getSprite() {return tileSprite_;}
int TileItem::getHealth() {return health_;}

//Setters
void TileItem::setHealth(const int health) {health_ = health;}