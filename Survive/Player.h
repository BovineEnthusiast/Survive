#ifndef PLAYER_H
#define	PLAYER_H
#include "Humanoid.h"
#include "Gun.h"
#include <SFML/Graphics.hpp>
class Player : public Humanoid
{
public:
    Player();
    void update(const sf::Time&);
    bool windowFocus;
    sf::RenderWindow* window;
    
    //Getters
    
    sf::Vector2f getVelocity();
    std::vector<Gun> getGuns();
    
    //Setters
    void setVelocity(const sf::Vector2f&);
    void setPosition(const sf::Vector2f&);
private:
    std::vector<Gun> vGuns_;
    
};

#endif	

