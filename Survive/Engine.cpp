#include "Engine.h"
Engine::Engine() 
{
    
}


bool Engine::initialize()
{
    _window.create(sf::VideoMode(500, 500), "Survive");
    return true;
}
void Engine::handleInput()
{
    
}

void Engine::update()
{
    
}

void Engine::draw()
{
    
}
int Engine::run()
{
    while(_window.isOpen())
    {
        sf::Event event;
        while(_window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                _window.close();
                return 0;
            }
            
        }
        
         _window.clear();
        handleInput();
        update();
        draw();
    }  
    
}