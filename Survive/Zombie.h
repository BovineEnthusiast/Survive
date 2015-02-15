#ifndef ZOMBIE_H
#define	ZOMBIE_H
#include <memory>
#include <mutex>
#include <stack>
#include "Humanoid.h"
#include "Player.h"
#include "Tile.h"
#include "Turret.h"
#include "Node.h"
#include "Barricade.h"

class Zombie : public Humanoid
{
public:
    Zombie(Player*, sf::Texture*, sf::Texture*, const int, const int);
    void update(const sf::Time&);

    //Fills the sPNodes_ stack with node ptr
    void findPath(std::vector< std::vector<Tile> >*);

    //Getters
    bool needsPath();
    bool countedDead();
    bool bled();
    bool isStill() const;
    bool isDead() const;
    bool isDeletable() const;
    bool isReadyToRepath() const;
    sf::Sprite getCorpseSprite() const;
    std::stack<Node> getNodes() const;

    //Setters
    void setTurretPtr(Turret*);
    void setBarricadePtr(Barricade*);
    void setNeedsPath(bool);
    
    //Operator overloading
    bool operator== (const Zombie&) const;
    /*Zombie& operator= (Zombie&& other);
    Zombie& operator= (const Zombie& other);*/
 private:

    //Protects the path node stack from being accessed concurrently
    //std::mutex mutexStack_;
    
    //A pointer to the player used to get information such as position
    Player* pPlayer_;

    //A stack of nodes that leads to the target
    std::stack<Node> sPNodes_;

    //Needs path if stuck
    bool needsPath_ = false;
    
    //A pointer to the nearest attackable item
    Turret* pTurret_ = nullptr;
    Barricade* pBarricade_ = nullptr;

    //The position of the target
    sf::Vector2i targetPosition_;
    
    //A normalized vector that points towards the player, from the zombie
    sf::Vector2f targetVector_;

    //Used to find out when pathing should run
    sf::Vector2i lastTargetPosition_;
    bool readyToRepath_ = true;
    
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
    sf::Clock pathClock_;
};

//Functor used to compare Node pointers
struct compNode
{
	bool operator() (Node*, Node*);
};
#endif	

