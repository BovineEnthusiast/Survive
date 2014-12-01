#include "Engine.h"
#include "Camera.h"
#include <iostream>
#include <deque>
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
 
    
    //Camera view for easier reference
    sf::View camView = level_.getCameraView();
    
    //Thhe top left and bottom right camera view boundry positions
    sf::Vector2f camTopLeft = sf::Vector2f(camView.getCenter().x - camView.getSize().x / 2, camView.getCenter().y - camView.getSize().y / 2); 
    sf::Vector2f camBottomRight = sf::Vector2f(camView.getCenter().x + camView.getSize().x / 2, camView.getCenter().y + camView.getSize().y / 2);
    
    //Vector positions of tiles to render
    int topLeftX = (camTopLeft.x - fmod(camTopLeft.x, tileSize_)) / tileSize_;
    int topLeftY = (camTopLeft.y - fmod(camTopLeft.y, tileSize_)) / tileSize_;
    int bottomRightX = (camBottomRight.x + fmod(camBottomRight.x, tileSize_)) / tileSize_;
    int bottomRightY = (camBottomRight.y - fmod(camBottomRight.y, tileSize_)) / tileSize_;

    //Draws Tiles inside the range of the camera first
    for(size_t vTile = topLeftX; vTile <= bottomRightX; ++vTile)
    	for(size_t tile = topLeftY; tile <= bottomRightY; ++tile)
        {
            level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * tileSize_, (float)tile * tileSize_));
            window_.draw(level_.tiles[vTile][tile].getSprite());
        }
    
    //Draws tile items. Separate because tile items have to be drawn on top to prevent being clipped(at least for now)
    for(size_t vTile = topLeftX; vTile <= bottomRightX; ++vTile)
    	for(size_t tile = topLeftY; tile <= bottomRightY; ++tile)
        {
            level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * tileSize_, (float)tile * tileSize_));
            window_.draw(level_.tiles[vTile][tile].getSprite());
        }
    
    //The player and current gun
    window_.draw(level_.getPlayer().getLegLeftSprite());
    window_.draw(level_.getPlayer().getLegRightSprite());
    window_.draw(level_.getPlayer().getArmLeftSprite());
    window_.draw(level_.getPlayer().getArmRightSprite());
    window_.draw(level_.getPlayer().getHeadSprite());
    window_.draw(level_.getPlayer().getGuns().at(0).getSprite());
    
    std::vector<Zombie> vZombies = level_.getZombies();
    for(auto iZombie = vZombies.begin(); iZombie != vZombies.end(); ++iZombie)
    {
        window_.draw(iZombie->getArmLeftSprite());
        window_.draw(iZombie->getArmRightSprite());
        window_.draw(iZombie->getHeadSprite());
        window_.draw(iZombie->getLegLeftSprite());
        window_.draw(iZombie->getLegRightSprite());
    }
    
    //Draws bullets
    std::list<Bullet> vBullets = level_.getBullets();
    for(auto iBullet = vBullets.begin(); iBullet != vBullets.end(); ++iBullet)
    {
        std::cout << vBullets.size() << std::endl;
        window_.draw(iBullet->getSprite());
        std::cout << "Draw bullet!" << std::endl;
    }
        
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
        
        //Clamps the camera to edges
        if(level_.getCameraView().getCenter().x - level_.getCameraView().getSize().x / 2 < 0)
            level_.setCameraPosition(sf::Vector2f(level_.getCameraView().getSize().x / 2, level_.getCameraView().getCenter().y));
        if(level_.getCameraView().getCenter().y - level_.getCameraView().getSize().y / 2 < 0)
            level_.setCameraPosition(sf::Vector2f(level_.getCameraView().getCenter().x, level_.getCameraView().getSize().y / 2));
        if(level_.getCameraView().getCenter().x + level_.getCameraView().getSize().x / 2 > level_.tiles.size() * tileSize_)
            level_.setCameraPosition(sf::Vector2f(level_.tiles.size() * tileSize_ - level_.getCameraView().getSize().x / 2, level_.getCameraView().getCenter().y));
        if(level_.getCameraView().getCenter().y + level_.getCameraView().getSize().y / 2 > level_.tiles.size() * tileSize_)
            level_.setCameraPosition(sf::Vector2f(level_.getCameraView().getCenter().x, level_.tiles.size() * tileSize_ - level_.getCameraView().getSize().y / 2));
        
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
