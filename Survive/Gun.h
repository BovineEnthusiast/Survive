#ifndef GUN_H
#define	GUN_H
#include "GameObject.h"
#include <deque>
#include <list>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "SoundManager.h"
#include "ImageManager.h"

class Gun : public GameObject
{
public:
    Gun(const std::string&, ImageManager*, SoundManager*);
    void update(const sf::Time&);
    
    sf::RenderWindow* window;
    
    //Getters
    sf::Vector2f getArmRightPos();
    sf::Vector2f getArmLeftPos();
    std::deque<Bullet> getBullets();
    sf::Sprite getSprite();
    int getDamage() const;
    
    //Setters
    void setLocalPosition(const sf::Vector2f&);
    void setPlayerPosition(const sf::Vector2f&);
    void setPlayerVelocity(const sf::Vector2f&);
    void setPlayerHeadRotation(const float);
    void setBulletsPtr(std::list<Bullet>*);
private:
    sf::Texture* pTexture_;
    sf::Sprite gun_;
    
    //Used to play sounds
    SoundManager* pSoundManager_;
    
    //Used to get texture
    ImageManager* pImageManager_;
    
    //The player's properties
    sf::Vector2f playerPos_;
    sf::Vector2f playerVelocity_;
    float playerRotation_;
    
    //The position on the gun that the left/right arm is touching
    sf::Vector2f armRightPosGlobal_;
    sf::Vector2f armLeftPosGlobal_;
    sf::Vector2f armRightPosLocal_;
    sf::Vector2f armLeftPosLocal_;
    
    //The position that the bullet spawns
    sf::Vector2f bulletSpawnPos_ = sf::Vector2f(2.5, 0);
    
    //Stores all active bullets
    std::list<Bullet>* pLBullets_;
    
    //Gun properties
    sf::Vector2f gunPosition_;
    std::string gunType_;
    float reloadTime_ = 1.5f; //In Seconds
    int bulletsPerMag_ = 15; //How much per reload
    int currentBullets_ = 15; //In current reload
    int totalBullets_ = 9999; //Total ammo
    int recoilAmount_ = 15; //In game "pixels"
    float fireRate_ = 0.1f; // bullets/sec
    bool reloading_ = false;
    bool auto_ = false;
    bool clicked_ = false; //Used to enforce semi-auto
    
    //Animation variables
    sf::Vector2f swayOffset_;
    sf::Vector2f recoilOffset_;
    
    //Bullet properties
    float bulletSpeed_ = 750.0f; //per second
    int bulletDamage_ = 25;
    
    //Clocks
    sf::Clock fireRateClock_;
    sf::Clock reloadClock_;
    sf::Clock animationClock_;

    
};

#endif

