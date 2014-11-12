#ifndef ENGINE_H
#define	ENGINE_H
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Level.h"

class Engine 
{
public:
    Engine();
    
    bool initialize();
    int run();  
    
private:
    sf::RenderWindow window_;
   // Level mainMenu_;
    //Level gameLevel_;
    Level level_;
    sf::Time _dT;
    sf::Clock _dTClock;
    bool windowFocused_ = true;
    void update();
    void draw();
};

#endif	

