#ifndef ZOMBIE_H
#define	ZOMBIE_H
#include "Humanoid.h"
#include "Player.h"

class Zombie : public Humanoid
{
public:
    Zombie(Player*);
    void update(const sf::Time&);
    
    void setPosition(const sf::Vector2f&);//TEMP PROB!?
private:
    //A pointer to the player used to get information such as position
    Player* pPlayer_;
    
    //A normalized vector that points towards the player, from the zombie
    sf::Vector2f targetVector_;

};

#endif	

