#ifndef CAMERA_H
#define	CAMERA_H
#include <SFML/Graphics.hpp>

class Camera 
{
public:
    Camera();
    void update(const sf::Vector2u&);
    void move(const sf::Vector2f&);
    void changeSize(const int);
    //Setters
    void setPosition(const sf::Vector2f&);
 
    //Getters
    sf::View getView();
private:
    //Size in game units, of the viewport's smaller side
    unsigned int _size = 100;
    
    //Size of the actual view
    sf::Vector2f _viewSize = sf::Vector2f(_size, _size);
    
    //The view to send to the window
    sf::View _view;

    
    sf::Vector2f _centerPos = sf::Vector2f(0,0);
};

#endif	

