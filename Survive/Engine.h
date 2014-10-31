#ifndef ENGINE_H
#define	ENGINE_H
#include <SFML/Graphics.hpp>
class Engine 
{
public:
    Engine();
    
    bool initialize();
    int run();  
    
private:
    sf::RenderWindow _window;
    
    void handleInput();
    void update();
    void draw();
};

#endif	

