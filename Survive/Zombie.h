#ifndef ZOMBIE_H
#define	ZOMBIE_H
#include "Humanoid.h"
#include "Player.h"
#include "Turret.h"

class Zombie : public Humanoid
{
public:
    Zombie(Player*, sf::Texture*, sf::Texture*, const int, const int);
    void update(const sf::Time&);
    

	//Getters
	bool countedDead();
	bool bled();
	bool isStill() const;
	bool isDead() const;
	bool isDeletable() const;
	sf::Sprite getCorpseSprite() const;

	//Setters
	void setTurretPtr(Turret*);
private:
    //A pointer to the player used to get information such as position
    Player* pPlayer_;
    
	//A pointer to the nearest turret
	Turret* pTurret_;

    //A normalized vector that points towards the player, from the zombie
    sf::Vector2f targetVector_;
    
	//The corpse texture
	sf::Texture* pCorpseTexture_;
	sf::Sprite corpseSprite_;

    sf::Clock armClock_;
    sf::Clock attackClock_;
    bool attacking_ = false;
    bool hit_ = false;
	bool dead_ = false;
	bool still_ = false;
	bool delete_ = false;
	bool countedDead_ = false;
	bool bleed_ = false; //Used for bleeding when sliding
    float armLeftVerticalOffset_ = std::rand() % 7;
    float armRightVerticalOffset_ = std::rand() % 7;
    float armLeftHorizontalOffset_ = std::rand() % 7;
    float armRightHorizontalOffset_ = std::rand() % 7;
	float corpseSpeed_;

	//Clocks
	sf::Clock deathClock_;
	sf::Clock bleedClock_;
};

#endif	

