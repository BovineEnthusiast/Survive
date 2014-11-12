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
    
    void virtual update(const sf::Time&) = 0;
    void assignParent(const GameObject&);
    void removeParent();
    
    //Getters
    sf::Vector2f getPositionGlobal() const;
    sf::Vector2f getScaleGlobal() const;
    float getRotationGlobal() const;
    
 
protected:
 
    //Transform values all GameObjects will contain
    //GameObject parentGameObject_;
    bool hasParent_ = false;
    sf::Vector2f positionGlobal_ = sf::Vector2f(350, 350);
    sf::Vector2f positionLocal_ = sf::Vector2f(0,0);
    sf::Vector2f scaleGlobal_ = sf::Vector2f(1, 1);
    sf::Vector2f scaleLocal_ = sf::Vector2f(1,1);
    float rotationGlobal_ = 0;
    float rotationLocal_ = 0;
};

#endif	
