#include "GameObject.h"
#include <iostream>
GameObject::GameObject() 
{
}
//Getters
sf::Vector2f GameObject::getPositionGlobal() const {return positionGlobal_;}
sf::Vector2f GameObject::getScaleGlobal() const {return scaleGlobal_;}
float GameObject::getRotationGlobal() const {return rotationGlobal_; std::cout << "test" << std::endl;}