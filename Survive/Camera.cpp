#include "Camera.h"

Camera::Camera() 
{
    _viewSize = sf::Vector2i(_size, _size);
    _view.setCenter(_centerPos);
    _view.setSize(_viewSize.x, _viewSize.y);
}
//Resizes the viewport for when the window is resized
void Camera::resize(const sf::Vector2i& size)
{
    //Sets the smaller side to a viewport of _size and the other one to _size time the ratio of the two sides
    if(size.x >= size.y)
        _viewSize = sf::Vector2i(_size *  size.x / size.y  * , _size);
    else
        _viewSize = sf::Vector2i(_size, _size * size.y / size .x);        
}
//Setters
void Camera::setPosition(const sf::Vector2f& position) {_centerPos = position;}
//Getters
sf::Vector2i Camera::getView() {return _viewSize;}
sf::View Camera::getView() {return _view;}