#ifndef TREE_H
#define	TREE_H
#include "GameObject.h"
#include <SFML/Graphics.hpp>
class Tree : public GameObject
{
public:
    Tree(sf::Texture*, sf::Texture*, sf::Texture*);
    void update(const sf::Time&);
    
    //Setters
    void setHealth(const int);
    
    //Getters
    sf::Sprite getUpperLeafOne();
    sf::Sprite getUpperLeafTwo();
    sf::Sprite getUpperLeafThree();
    sf::Sprite getUpperLeafFour();
    sf::Sprite getLowerLeafOne();
    sf::Sprite getLowerLeafTwo();
    sf::Sprite getLowerLeafThree();
    sf::Sprite getLowerLeafFour();
    sf::Sprite getTrunk();
    int getHealth();
    

private:
    
    //Graphical
    sf::Sprite upperLeafOne_;
    sf::Sprite upperLeafTwo_;
    sf::Sprite upperLeafThree_;
    sf::Sprite upperLeafFour_;
    sf::Sprite lowerLeafOne_;
    sf::Sprite lowerLeafTwo_;
    sf::Sprite lowerLeafThree_;
    sf::Sprite lowerLeafFour_;
    sf::Sprite trunk_;
    
    //Pointers to textures
    sf::Texture* upperLeafTexture_;
    sf::Texture* lowerLeafTexture_;
    sf::Texture* trunkTexture_;
    
    
    //Item properties
    int health_ = 250;
    int rewardMin_ = 2;
    int rewardMax_ = 4;
    float swayMultiplier_ = 2; //Used to increase sway when being shot
    //Clocks
    sf::Clock swayClock_; //Used for sway animation
    
};

#endif	