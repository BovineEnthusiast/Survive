#ifndef ENGINE_H
#define	ENGINE_H
#include <SFML/Graphics.hpp>
class Engine 
{
public:
    Engine();
    
    bool initialize();
    int run();
    void handleInput();
    void update();
    void draw();
    
private:
    sf::RenderWindow window;
};

#endif	

