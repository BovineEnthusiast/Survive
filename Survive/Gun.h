#ifndef GUN_H
#define	GUN_H
#include "GameObject.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Gun : public GameObject
{
public:
    Gun(const std::string&);
    void update(const sf::Time&);
    
    //Getters
    sf::Vector2f getArmRightPos();
    sf::Vector2f getArmLeftPos();
    std::vector<Bullet> getBullets();
    sf::Sprite getSprite();
    //Setters
    void setLocalPosition(const sf::Vector2f&);
    void setPlayerPosition(const sf::Vector2f&);
    void setPlayerHeadRotation(const float);
private:
    sf::Texture gunSpriteSheet_;
    sf::Sprite gun_;
    
    //The player's properties
    sf::Vector2f playerPos_;
    float playerRotation_;
    
    //The position on the gun that the left/right arm is touching
    sf::Vector2f armRightPos_;
    sf::Vector2f armLeftPos_;
    
    //The position that the bullet spawns
    sf::Vector2f bulletSpawnPos_ = sf::Vector2f(2.5, 0);
    
    //Stores all active bullets
    std::vector<Bullet> vBullets_;
    
    //Gun properties
    float reloadTime_ = 1.5f; //In Seconds
    int bulletsPerMag_ = 31; //How much per reload
    int currentBullets_ = 15; //In current reload
    int totalBullets_ = 45; //Total ammo
    float fireRate_ = 0.1f; // bullets/sec
    bool reloading_ = false;
    bool auto_ = true;
    bool clicked_ = false; //Used to enforce semi-auto
    //Bullet properties
    float bulletSpeed_ = 750.0f; //per second
    int bulletDamage_ = 25;
    
    //Clocks
    sf::Clock fireRateClock_;
    sf::Clock reloadClock_;

};

#endif

