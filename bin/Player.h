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
    int getPoints() const;
    int getTurrets() const;
    int getBarricades() const;
    bool hasMagnum() const;
    bool hasShotgun() const;
    bool hasRifle() const;
    
    //Setters
    void setPoints(const int);
    void setPosition(const sf::Vector2f&);
    void setGunBulletPointers(std::list<Bullet>*);
    void setTurrets(const int);
    void setBarricades(const int);
    void setHasMagnum(const bool);
    void setHasShotgun(const bool);
    void setHasRifle(const bool);
 private:
    //Weapon unlocks/amounts
    bool hasMagnum_ = false;
    bool hasShotgun_ = false;
    bool hasRifle_ = false;
    
    int turrets_ = 0;
    int barricades_ = 0;
    
    int points_ = 100000;
    int currentGun_ = 0;
    std::vector<Gun> vGuns_;
    SoundManager* pSoundManager_;
    sf::Clock footstepClock_;
    
};

#endif	

