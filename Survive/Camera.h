#ifndef CAMERA_H
#define	CAMERA_H
#include <SFML/Graphics.hpp>

class Camera 
{
public:
    Camera();
    void resizeView(const sf::Vector2u&);
    void move(const sf::Vector2f&);
    void changeSize(const int);
    
    //Setters
    void setPosition(const sf::Vector2f&);
 
    //Getters
    sf::View getView();
    
private:
    //Size in game units, of the viewport's smaller side
    unsigned int _size = 750;
    
    //Size of the actual view
    sf::Vector2f viewSize_ = sf::Vector2f(_size, _size);
    
    //The view to send to the window
    sf::View view_;

};

#endif	

