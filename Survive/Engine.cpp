#include "Engine.h"
#include "Camera.h"
#include <iostream>
Engine::Engine() 
{
    
}


bool Engine::initialize()
{
    _window.create(sf::VideoMode(645, 645), "Survive");
    return true;
}
void Engine::handleInput()
{
    if(_windowFocused)
    {
        if(!level_.isMenu())
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                level_.moveCamera(sf::Vector2f(100,0) * _dT.asSeconds());
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
               level_.moveCamera(sf::Vector2f(-100,0) * _dT.asSeconds());
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                level_.moveCamera(sf::Vector2f(0,-100) * _dT.asSeconds());
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                level_.moveCamera(sf::Vector2f(0,100) * _dT.asSeconds());
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                level_.zoomCamera(1);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                level_.zoomCamera(-1);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                level_.generateLevel(129,129);
        }
    }
}

void Engine::update()
{
    
}

void Engine::draw()
{
    //Draws Tiles first
    for(size_t vTile = 0; vTile < level_.tiles.size(); ++vTile)
    	for(size_t tile = 0; tile < level_.tiles[vTile].size(); ++tile)
        {
            level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * 5, (float)tile * 5));
            _window.draw(level_.tiles[vTile][tile].getSprite());
        }
}
int Engine::run()
{ 
    //Sets the viewport to the camera
    _window.setView(level_.getCameraView());
    level_.generateLevel(129, 129);

    //Game Loop
    while(_window.isOpen())
    {
        //delta time
        _dT = _dTClock.restart();
        
         //Updates Level
        level_.update(_dT);
        
        //update cam
        level_.resizeCamera(_window.getSize());
        _window.setView(level_.getCameraView());
        
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
                level_.resizeCamera(_window.getSize());
                _window.setView(level_.getCameraView());
            }
            else if(event.type == sf::Event::LostFocus)
                _windowFocused = false;
            else if(event.type == sf::Event::GainedFocus)
                _windowFocused = true;
        }
        
        _window.clear();
        handleInput();
        update();
        draw();
        _window.display();
    }  
    
}
