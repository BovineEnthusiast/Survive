#ifndef PLAYER_H
#define	PLAYER_H
#include <SFML/Graphics.hpp>
#include "Humanoid.h"
#include "Gun.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "LightingPolygon.h"
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
    int getMines() const;
    float getShake();
    bool hasMagnum() const;
    bool hasShotgun() const;
    bool hasRifle() const;
	bool hasRocket() const;
    bool isMuzzleFlash() const;
    std::vector<sf::ConvexShape> getTriangles() const;
    
    //Setters
    void setPoints(const int);
    void setPosition(const sf::Vector2f&);
    void setGunBulletPointers(std::list<Bullet>*);
    void setTurrets(const int);
    void setBarricades(const int);
    void setMines(const int);
    void setHasMagnum(const bool);
    void setHasShotgun(const bool);
    void setHasRifle(const bool);
    void setHasRocket(const bool);
    void increaseAmmo(const int, const int);
	void setInStore(const bool);
    //Pushers
    void pushLightingSprite(const sf::Sprite&);

 private:
    //The lighting/line of sight of the player
    LightingPolygon lighting_;
    //Button clicks
    bool downSwapRight_ = false;
    bool downSwapLeft_ = false;
    
	bool inStore_ = false; //Can't fire whilst in store
    //Weapon unlocks/amounts
    bool hasMagnum_ = false;
    bool hasShotgun_ = false;
    bool hasRifle_ = false;
	bool hasRocket_ = false;
    bool muzzleFlash_ = false;

    int turrets_ = 0;
    int barricades_ = 0;
    int mines_ = 0;
    
    int points_ = 0;
    int currentGun_ = 0;
    std::vector<Gun> vGuns_;
    SoundManager* pSoundManager_;
    float shake_;
    sf::Clock footstepClock_;

    //Helper functions
    void swapGun(const bool);
};

#endif	

