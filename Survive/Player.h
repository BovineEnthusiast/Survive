#ifndef PLAYER_H
#define	PLAYER_H
#include <SFML/Graphics.hpp>
#include "Humanoid.h"
#include "Gun.h"
#include "Bullet.h"
#include "SoundManager.h"
class Player : public Humanoid
{
public:
    Player(sf::Texture*, ImageManager*, SoundManager*);
    void update(const sf::Time&);
    bool windowFocus;
    sf::RenderWindow* window;
    
    //Getters
    std::vector<Gun> getGuns();
    int getCurrentGunIndex() const;
    
    //Setters
    void setPosition(const sf::Vector2f&);
    void setGunBulletPointers(std::list<Bullet>*);
private:
    int currentGun_ = 0;
    std::vector<Gun> vGuns_;
    SoundManager* pSoundManager_;
    sf::Clock footstepClock_;
    
};

#endif	

