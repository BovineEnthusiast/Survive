#include "Camera.h"

Camera::Camera() 
{
    view_.setCenter(0,0);
    view_.setSize(viewSize_.x, viewSize_.y);
}

void Camera::move(const sf::Vector2f& move) 
{
    view_.setCenter(view_.getCenter() + move);
    
}

//Resizes the viewport for when the window is resized
void Camera::resizeView(const sf::Vector2u& size)
{
    //Sets the smaller side to a viewport of _size and the other one to _size time the ratio of the two sides
    if(size.x >= size.y)
        viewSize_ = sf::Vector2f(_size *  size.x / size.y, _size);
    else
        viewSize_ = sf::Vector2f(_size, _size * size.y / size .x); 
    view_.setSize(viewSize_);
}
void Camera::changeSize(const int change) 
{
    _size += change;
    //resizeView(sf::Vector2u(viewSize_.x, viewSize_.y));
}
//Setters
void Camera::setPosition(const sf::Vector2f& position) {view_.setCenter(position);}

//Getters
sf::View Camera::getView() {return view_;}