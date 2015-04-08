#include "GameObject.h"
#include <iostream>
GameObject::GameObject() 
{
}
//Setters
void GameObject::setPositionGlobal(const sf::Vector2f& position) {positionGlobal_ = position;}

//Getters

sf::Vector2f GameObject::getPositionGlobal() const {return positionGlobal_;}
sf::Vector2f GameObject::getScaleGlobal() const {return scaleGlobal_;}
float GameObject::getRotationGlobal() const {return rotationGlobal_;}
