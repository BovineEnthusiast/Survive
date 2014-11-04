#ifndef CAMERA_H
#define	CAMERA_H
#include <SFML/Graphics.hpp>

class Camera 
{
public:
    Camera();
    void resize(const sf::Vector2i&);
    //Setters
    void setPosition(const sf::Vector2f&);
    //Getters
    sf::View getView();
private:
    //Size in game units, of the viewport
    unsigned int _size = 500;
    
    //Size of the actual view
    sf::Vector2i _viewSize;
    
    //The view to send to the window
    sf::View _view;

    
    sf::Vector2f _centerPos = sf::Vector2f(0,0);
};

#endif	

