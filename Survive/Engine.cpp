#include "Engine.h"
#include "Camera.h"
#include <iostream>
Engine::Engine() 
{
    
}


bool Engine::initialize()
{
    window_.create(sf::VideoMode(645, 645), "Survive");
    return true;
}

void Engine::update()
{
    level_.update(_dT);
}

void Engine::draw()
{
    //Draws Tiles first
    for(size_t vTile = 0; vTile < level_.tiles.size(); ++vTile)
    	for(size_t tile = 0; tile < level_.tiles[vTile].size(); ++tile)
        {
            level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * 50, (float)tile * 50));
            window_.draw(level_.tiles[vTile][tile].getSprite());
        }
    window_.draw(level_.getPlayer().getArmLeftSprite());
    window_.draw(level_.getPlayer().getArmRightSprite());
    window_.draw(level_.getPlayer().getLegLeftSprite());
    window_.draw(level_.getPlayer().getLegRightSprite());
    window_.draw(level_.getPlayer().getHeadSprite());
}
int Engine::run()
{ 
    //Sets the viewport to the camera
    window_.setView(level_.getCameraView());
    level_.generateLevel(129, 129);

    //Game Loop
    while(window_.isOpen())
    {
        //delta time
        _dT = _dTClock.restart();
        
         //Updates Level
        level_.update(_dT);
        
        //update cam
        level_.setCameraPosition(level_.getPlayer().getPositionGlobal());
        level_.resizeCamera(window_.getSize());
        window_.setView(level_.getCameraView());
        
        sf::Event event;
        while(window_.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window_.close();
                return 0;
            }       
            else if(event.type == sf::Event::Resized)
            {
                //Resets viewport
                level_.resizeCamera(window_.getSize());
                window_.setView(level_.getCameraView());
            }
            else if(event.type == sf::Event::LostFocus)
                windowFocused_ = false;
            else if(event.type == sf::Event::GainedFocus)
                windowFocused_ = true;
        }
        
        window_.clear();
        update();
        draw();
        window_.display();
    }  
    
}
