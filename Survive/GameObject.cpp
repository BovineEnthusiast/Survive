#include "GameObject.h"

GameObject::GameObject() 
{
}
//Getters
sf::Vector2f GameObject::getPositionGlobal() const {return positionGlobal_;}
sf::Vector2f GameObject::getScaleGlobal() const {return scaleGlobal_;}
float GameObject::getRotationGlobal() const {return rotationGlobal_;}

void GameObject::update(const sf::Time& dT)
{
    /*if(hasParent_) //Sets the transform values based off the parent
    {
        positionGlobal_ = positionLocal_ + parentGameObject_.getPositionGlobal();
        scaleGlobal_ = scaleLocal_ * parentGameObject_.getScaleGlobal();
        rotationGlobal_ = rotationLocal_ + parentGameObject_.getRotationGlobal();
    }
    else //No parent
    {*/
        positionGlobal_ = positionLocal_;
        scaleGlobal_ = scaleLocal_;
        rotationGlobal_ = rotationLocal_;
    //}
        
        //Update the components
        for(auto const& component : components_)
            component->update(dT);
    
}