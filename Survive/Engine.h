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
    sf::RenderWindow _window;
    Camera _camera;
    Level _level;
    sf::Time _dT;
    sf::Clock _dTClock;
    bool _windowFocused = true;
    void handleInput();
    void update();
    void draw();
};

#endif	

