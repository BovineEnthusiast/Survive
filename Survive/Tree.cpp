#include "Tree.h"

Tree::Tree(sf::Texture* upperTexture, sf::Texture* lowerTexture, sf::Texture* trunkTexture)
:upperLeafTexture_(upperTexture), lowerLeafTexture_(lowerTexture), trunkTexture_(trunkTexture)
{
    
    //Assigns textures
    upperLeafOne_.setTexture(*upperLeafTexture_);
    upperLeafTwo_.setTexture(*upperLeafTexture_);
    upperLeafThree_.setTexture(*upperLeafTexture_);
    upperLeafFour_.setTexture(*upperLeafTexture_);
    lowerLeafOne_.setTexture(*lowerLeafTexture_);
    lowerLeafTwo_.setTexture(*lowerLeafTexture_);
    lowerLeafThree_.setTexture(*lowerLeafTexture_);
    lowerLeafFour_.setTexture(*lowerLeafTexture_);
    trunk_.setTexture(*trunkTexture_);
    
    //Sets up trunk
    trunk_.setOrigin(10.0f, 10.0f);
    
   
    //Origins
    upperLeafOne_.setOrigin(20, 79);
    upperLeafTwo_.setOrigin(20, 79);
    upperLeafThree_.setOrigin(20, 79);
    upperLeafFour_.setOrigin(20, 79);
    lowerLeafOne_.setOrigin(19, 70);
    lowerLeafTwo_.setOrigin(19, 70);
    lowerLeafThree_.setOrigin(19, 70);
    lowerLeafFour_.setOrigin(19, 70);
  
    //Rotations
    upperLeafOne_.setRotation(30);
    upperLeafTwo_.setRotation(120);
    upperLeafThree_.setRotation(210);
    upperLeafFour_.setRotation(300);
    lowerLeafOne_.setRotation(75);
    lowerLeafTwo_.setRotation(165);
    lowerLeafThree_.setRotation(255);
    lowerLeafFour_.setRotation(345);
}
void Tree::update(const sf::Time& dT)
{
    lowerLeafOne_.setPosition(positionGlobal_);
    lowerLeafTwo_.setPosition(positionGlobal_);
    lowerLeafThree_.setPosition(positionGlobal_);
    lowerLeafFour_.setPosition(positionGlobal_);
    upperLeafOne_.setPosition(positionGlobal_);
    upperLeafTwo_.setPosition(positionGlobal_);
    upperLeafThree_.setPosition(positionGlobal_);
    upperLeafFour_.setPosition(positionGlobal_);
    trunk_.setPosition(positionGlobal_);
    
    lowerLeafOne_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 2.0f) / 25.0f);
    lowerLeafTwo_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 4.0f) / 25.0f);
    lowerLeafThree_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 6.0f) / 25.0f);
    lowerLeafFour_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 8.0f) / 25.0f);
    upperLeafOne_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 10.0f) / 25.0f);
    upperLeafTwo_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 12.0f) / 25.0f);
    upperLeafThree_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 14.0f) / 25.0f);
    upperLeafFour_.rotate(sin(swayClock_.getElapsedTime().asSeconds() + 16.0f) / 25.0f);
}

//Setters
void Tree::setHealth(const int health) {health_ = health;}
 
//Getters
sf::Sprite Tree::getLowerLeafOne() {return lowerLeafOne_;}
sf::Sprite Tree::getLowerLeafTwo() {return lowerLeafTwo_;}
sf::Sprite Tree::getLowerLeafThree() {return lowerLeafThree_;}
sf::Sprite Tree::getLowerLeafFour() {return lowerLeafFour_;}
sf::Sprite Tree::getUpperLeafOne() {return upperLeafOne_;}
sf::Sprite Tree::getUpperLeafTwo() {return upperLeafTwo_;}
sf::Sprite Tree::getUpperLeafThree() {return upperLeafThree_;}
sf::Sprite Tree::getUpperLeafFour() {return upperLeafFour_;}
sf::Sprite Tree::getTrunk() {return trunk_;}
int Tree::getHealth() {return health_;}