#ifndef BULLET_H
#define	BULLET_H
#include "GameObject.h"
#include "LightingPolygon.h"
#include "Emitter.h"
#include <SFML/Graphics.hpp>
class Bullet : public GameObject
{
public:
    Bullet(const bool, const sf::Vector2f&, const sf::Vector2f&, const int);
    void update(const sf::Time&);
    
    //Setters
    void setHit(bool);
	void setFromTurret(const bool);
    //Getters
    sf::RectangleShape getSprite() const;
    sf::CircleShape getLight() const;
    int getDamage() const;
    bool isDead() const;
    bool isHit() const;
	bool isRocket() const;
    sf::Vector2f getLastPosition();
	Emitter getRocketEmitter() const;
	Emitter getExplosionEmitter() const;
	LightingPolygon getExplosionPolygon() const;
	float getExplosionTime() const;
	bool isFromTurret() const;

	//Pushers
	void pushSprite(const sf::Sprite&);
private:
    //Light caused by the bullet
    sf::CircleShape light_;
    
    sf::RectangleShape bullet_;
    sf::Vector2f velocity_;
    sf::Vector2f lastPosition_;
    int damage_ = 25;
    //Used to determine when it should die if it hits nothing
    int deathTime_ = 1250;
    sf::Clock deathClock_;
    bool hit_ = false;
	bool fromTurret_ = false;

	//For rocket launchers
	bool rocket_ = false;
	Emitter rocketEmitter_;
	Emitter explosionEmitter_;

	//Dynamic lighting for explosion
	bool explosionLighting_ = false;
	LightingPolygon explosionLight_;
	sf::Clock explosionClock_;
};

#endif

