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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            _camera.move(sf::Vector2f(100,0) * _dT.asSeconds());
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            _camera.move(sf::Vector2f(-100,0) * _dT.asSeconds());
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            _camera.move(sf::Vector2f(0,-100) * _dT.asSeconds());
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            _camera.move(sf::Vector2f(0,100) * _dT.asSeconds());
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            _camera.changeSize(1);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            _camera.changeSize(-1);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            _level.generateLevel(129,129);
    }
}

void Engine::update()
{
    
}

void Engine::draw()
{
    //Draws Tiles first
    for(size_t vTile = 0; vTile < _level.tiles.size(); ++vTile)
    	for(size_t tile = 0; tile < _level.tiles[vTile].size(); ++tile)
        {
            _level.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * 5, (float)tile * 5));
            _window.draw(_level.tiles[vTile][tile].getSprite());
        }
}
int Engine::run()
{
    _window.setView(_camera.getView());
    _level.generateLevel(129, 129);
    //Game Loop
    while(_window.isOpen())
    {
        //delta time
        _dT = _dTClock.restart();
        
        //update cam
        _camera.update(_window.getSize());
        _window.setView(_camera.getView());
        
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
                _camera.update(_window.getSize());
                _window.setView(_camera.getView());
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