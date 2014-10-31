#include "Engine.h"

Engine::Engine() 
{
    
}


bool Engine::initialize()
{
    window = sf::RenderWindow(sf::VideoMode(500, 500), "Survive");
    
    if(!window)
        return false;
    else
        return true;
}   