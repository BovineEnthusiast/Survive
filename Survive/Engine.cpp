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
    //Draws Tiles first
    for(Tile tile : _tiles)
    	_window.draw(tile.getSprite());
}
int Engine::run()
{
	//Sets the viewport to the camera
    _window.setView(_camera.getView());
    
    //Generates a level by filling the tiles
    Level _level;
    _level.generateLevel(257, 257);
    
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
