#include "Camera.h"
#include <cmath>

Camera::Camera() 
{
    view_.setCenter(0,0);
    view_.setSize(viewSize_.x, viewSize_.y);
}

void Camera::update(const sf::Time& dT)
{
  if(shakeRadius_ != 0)
    {
      if(firstShake_)
	{
	  radians_ = (std::rand() % 628) / 100.0f;
	  shakeDirection_ = sf::Vector2f(cos(radians_), sin(radians_));
	  firstShake_ = false;
	}
      else if(shakeClock_.getElapsedTime().asSeconds() > 0.025f)
	{
	  shakeClock_.restart();
	  shakeRadius_ *= 0.9f;
	  radians_ += 3.14f + ((std::rand() % 2 == 0) ? 1.05f : -1.0f);	  
	  shakeDirection_ = sf::Vector2f(cos(radians_), sin(radians_));
	}
      if(shakeRadius_ <= 0.5f)
	{
	  firstShake_ = true;
	  shakeRadius_ = 0;
	}
    }

  view_ = posView_;
  view_.setCenter(posView_.getCenter() + shakeDirection_ * shakeRadius_);
}

void Camera::move(const sf::Vector2f& move) 
{
    posView_.setCenter(view_.getCenter() + move);
}

//Resizes the viewport for when the window is resized
void Camera::resizeView(const sf::Vector2u& size)
{
    //Sets the smaller side to a viewport of _size and the other one to _size time the ratio of the two sides
    if(size.x >= size.y)
        viewSize_ = sf::Vector2f(_size *  size.x / size.y, _size);
    else
        viewSize_ = sf::Vector2f(_size, _size * size.y / size .x); 
    posView_.setSize(viewSize_);
}
void Camera::changeSize(const int change) 
{
    _size += change;
    //resizeView(sf::Vector2u(viewSize_.x, viewSize_.y));
}
//Setters
void Camera::setPosition(const sf::Vector2f& position) {posView_.setCenter(position);}
void Camera::setShake(const float shake) { shakeRadius_ = shake; }
//Getters
sf::View Camera::getView() {return view_;}
