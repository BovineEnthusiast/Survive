#include "Camera.h"

Camera::Camera() 
{
    _view.setCenter(_centerPos);
    _view.setSize(_viewSize.x, _viewSize.y);
}
void Camera::changeSize(const int change) {_size += change;}
void Camera::move(const sf::Vector2f& move) {_centerPos += move;}

//Resizes the viewport for when the window is resized
void Camera::update(const sf::Vector2u& size)
{
    //Sets the smaller side to a viewport of _size and the other one to _size time the ratio of the two sides
    if(size.x >= size.y)
        _viewSize = sf::Vector2f(_size *  size.x / size.y, _size);
    else
        _viewSize = sf::Vector2f(_size, _size * size.y / size .x); 
    _view.setSize(_viewSize);
    _view.setCenter(_centerPos);
}
//Setters
void Camera::setPosition(const sf::Vector2f& position) {_centerPos = position;}

//Getters
sf::View Camera::getView() {return _view;}