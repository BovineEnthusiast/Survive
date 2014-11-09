#include "GameObject.h"

GameObject::GameObject() 
{
}
//Getters
sf::Vector2f GameObject::getPositionGlobal() const {return positionGlobal_;}
sf::Vector2f GameObject::getScaleGlobal() const {return scaleGlobal_;}
float GameObject::getRotationGlobal() const {return rotationGlobal_;}
