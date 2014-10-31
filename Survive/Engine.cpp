#include "Engine.h"

Engine::Engine() 
{
    
}


bool Engine::initialize()
{
    _window = sf::RenderWindow(sf::VideoMode(500, 500), "Survive");
    
    if(!_window)
        return false;
    else
        return true;
}   