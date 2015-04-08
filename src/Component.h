#ifndef COMPONENT_H
#define	COMPONENT_H
#include <SFML/Graphics.hpp>
class Component 
{
public:
    Component();
    virtual void update(const sf::Time&) = 0;
private:

};

#endif

