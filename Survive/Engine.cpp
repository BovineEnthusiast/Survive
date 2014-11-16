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
    //Temporary
    int tileSize = 50;
    
    //Camera view for easier reference
    sf::View camView = level_.getCameraView();
    
    //Thhe top left and bottom right camera view boundry positions
    sf::Vector2f camTopLeft = sf::Vector2f(camView.getCenter().x - camView.getSize().x / 2, camView.getCenter().y - camView.getSize().y / 2); 
    sf::Vector2f camBottomRight = sf::Vector2f(camView.getCenter().x + camView.getSize().x / 2, camView.getCenter().y + camView.getSize().y / 2);
    
    //Vector positions of tiles to render
    int topLeftX = (camTopLeft.x - fmod(camTopLeft.x, tileSize)) / tileSize;
    int topLeftY = (camTopLeft.y - fmod(camTopLeft.y, tileSize)) / tileSize;
    int bottomRightX = (camBottomRight.x + fmod(camBottomRight.x, tileSize)) / tileSize;
    int bottomRightY = (camBottomRight.y + fmod(camBottomRight.y, tileSize)) / tileSize;

    //Draws Tiles inside the range of the camera first
    for(size_t vTile = topLeftX; vTile <= bottomRightX; ++vTile)
    	for(size_t tile = topLeftY; tile <= bottomRightY; ++tile)
        {
            level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * 50, (float)tile * 50));
            window_.draw(level_.tiles[vTile][tile].getSprite());
        }
    
    //The player and current gun
    window_.draw(level_.getPlayer().getArmLeftSprite());
    window_.draw(level_.getPlayer().getArmRightSprite());
    window_.draw(level_.getPlayer().getLegLeftSprite());
    window_.draw(level_.getPlayer().getLegRightSprite());
    window_.draw(level_.getPlayer().getHeadSprite());
    window_.draw(level_.getPlayer().getGuns().at(0).getSprite());
    
    std::vector<Bullet> vBullets = level_.getPlayer().getGuns().at(0).getBullets();
    for(size_t bullet = 0; bullet < vBullets.size(); ++bullet)
        window_.draw(vBullets.at(bullet).getSprite());
}
int Engine::run()
{ 
    //Sets the viewport to the camera
    window_.setView(level_.getCameraView());
    level_.generateLevel(129, 129);
    
    //TEMPORARY
    level_.setPlayerWindow(window_);
    
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
