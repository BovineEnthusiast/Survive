#include "Engine.h"
#include "Level.h"
#include "Camera.h"
#include <iostream>
Engine::Engine() 
{
    
}


bool Engine::initialize()
{
    _window.create(sf::VideoMode(513, 513), "Survive");
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
    _window.setView(_camera.getView());
    
    //Game Loop
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
            else if(event.type == sf::Event::Resized)
            {
                //Resets viewport
                _camera.resize(_window.getSize());
                _window.setView(_camera.getView());
            }         
        }
        
         _window.clear();
        handleInput();
        update();
        draw();
        _window.display();
    }  
    
}