#ifndef CAMERA_H
#define	CAMERA_H
#include <SFML/Graphics.hpp>

class Camera 
{
public:
    Camera();
    void update(const sf::Time&);
    void resizeView(const sf::Vector2u&);
    void move(const sf::Vector2f&);
    void changeSize(const int);
    void setShake(const float);
    
    //Setters
    void setPosition(const sf::Vector2f&);
 
    //Getters
    sf::View getView();
	float getShake() const;
    
private:
    //Size in game units, of the viewport's smaller side
    unsigned int _size = 480;
    
    //Size of the actual view
    sf::Vector2f viewSize_ = sf::Vector2f(_size, _size);
    
    //The view to send to the window
    sf::View view_;

    //The view that stores the position view
    sf::View posView_;

    //Camera shake
    float shakeRadius_ = 0.0f;
    sf::Clock shakeClock_;
    float radians_ = 0.0f;
    bool firstShake_ = true;
    sf::Vector2f shakeDirection_;
};

#endif	

