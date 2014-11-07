#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Component.h"
class GameObject 
{
public:
    GameObject();
    
    void update(const sf::Time&);
    void assignParent(const GameObject&);
    void removeParent();
    
    //Template function that will return a desired component type else throw an exception
    template <typename Type>
    Type& getComponent()
    {
        for(size_t component = 0; component < components_.size(); ++component)
        { 
            if(Type &componentRef = dynamic_cast<Type>(components_[component].get()))
                return componentRef;
            //else
            //throw an exception
        }
    }
    
    //Getters
    sf::Vector2f getPositionGlobal() const;
    sf::Vector2f getScaleGlobal() const;
    float getRotationGlobal() const;
    
 
private:
    //Holds derived components in a base container. Needs dynamic casting
    std::vector<std::unique_ptr<Component>> components_;
    
    //Transform values all GameObjects will contain
    //GameObject parentGameObject_;
    bool hasParent_ = false;
    sf::Vector2f positionGlobal_ = sf::Vector2f(0, 0);
    sf::Vector2f positionLocal_ = sf::Vector2f(0,0);
    sf::Vector2f scaleGlobal_ = sf::Vector2f(1, 1);
    sf::Vector2f scaleLocal_ = sf::Vector2f(1,1);
    float rotationGlobal_ = 0;
    float rotationLocal_ = 0;
};

#endif	
