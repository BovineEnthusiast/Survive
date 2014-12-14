#ifndef PLAYER_H
#define	PLAYER_H
#include "Humanoid.h"
#include "Gun.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
class Player : public Humanoid
{
public:
    Player(sf::Texture*);
    void update(const sf::Time&);
    bool windowFocus;
    sf::RenderWindow* window;
    
    //Getters
    std::vector<Gun> getGuns();
    
    //Setters
    void setPosition(const sf::Vector2f&);
    void setGunBulletPointers(std::list<Bullet>*);
private:
    std::vector<Gun> vGuns_;
    
};

#endif	

