#ifndef GUN_H
#define	GUN_H
#include "GameObject.h"
#include <deque>
#include <list>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "LightingPolygon.h"
#include "SoundManager.h"
#include "ImageManager.h"
#include "Emitter.h"

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
	int getCurrentBullets() const;
	int getBulletsPerMag() const;
	int getTotalAmmo() const;
	sf::Time getCurrentReloadTime() const;
	float getReloadTime() const;
	bool isReloading() const;
	bool isMuzzleLight() const;
	std::vector<sf::ConvexShape> getMuzzleTriangles() const;
	float getShake();
	std::vector<Emitter> getEmitters() const;
	sf::Vector2f getBulletSpawnPos() const;
	//Setters
	void setLocalPosition(const sf::Vector2f&);
	void setPlayerPosition(const sf::Vector2f&);
	void setPlayerVelocity(const sf::Vector2f&);
	void setPlayerHeadRotation(const float);
	void setBulletsPtr(std::list<Bullet>*);

	//Pushers
	void pushMuzzleLightSprite(const sf::Sprite&);
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
	sf::Vector2f bulletSpawnPos_ = sf::Vector2f(10.0f, 0);

	//Stores all active bullets
	std::list<Bullet>* pLBullets_;

	//Gun properties
	sf::Vector2f gunPosition_;
	std::string gunType_;
	float reloadTime_ = 1.5f; //In Seconds
	int bulletsPerMag_ = 15; //How much per reload
	int currentBullets_ = 15; //In current reload
	int totalBullets_ = 9999; //Total ammo
	float recoilAmount_ = 4.0f; //In game "pixels"
	float fireRate_ = 0.1f; // bullets/sec
	bool reloading_ = false;
	bool auto_ = false;
	bool clicked_ = false; //Used to enforce semi-auto
	bool ptrSet_ = false;
	bool shotgun_ = false;
	bool rocket_ = false;
	float shake_ = 0.0f;
	//Animation variables
	sf::Vector2f swayOffset_;
	sf::Vector2f recoilOffset_;

	//Bullet properties
	float bulletSpeed_ = 1500.0f; //per second
	int bulletDamage_ = 25;

	//Used for muzzle flashes
	std::vector<Emitter> vEmitters_;

	//Dynamic lighting for flashes
	bool muzzleLighting_ = false;
	LightingPolygon muzzleLight_;

	//Clocks
	sf::Clock muzzleClock_;
	sf::Clock fireRateClock_;
	sf::Clock reloadClock_;
	sf::Clock animationClock_;


};

#endif

