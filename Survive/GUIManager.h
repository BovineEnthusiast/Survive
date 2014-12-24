#ifndef GUIMANAGER_H
#define	GUIMANAGER_H
#include <SFML/Graphics.hpp>
#include "Player.h"
class GUIManager
{
public:
    GUIManager(sf::RenderWindow*, Player*);
    void update(const sf::Time&);
    
    //Getters
    sf::RectangleShape getHealthOutOf() const;
    sf::RectangleShape getHealthCurrent() const;
    sf::Text getCurrentAmmo() const;
    sf::Text getTotalAmmo() const;
	sf::RectangleShape getReloadOutOf() const;
	sf::RectangleShape getReloadCurrent() const;
    
    
private:
    sf::RenderWindow* pWindow_;
    Player* pPlayer_;
    sf::Font font_;
    
    //Health
    sf::RectangleShape healthOutOf_;
    sf::RectangleShape healthCurrent_;
    sf::Vector2f healthOffset_ = sf::Vector2f(0.01f, 0.01f); //In percent of window size
    sf::Vector2f healthSize_ = sf::Vector2f(0.075f, 0.01f); //In percent of window size
    
	//Weapon
	sf::Text ammo_;
	sf::Vector2f ammoOffset_ = sf::Vector2f(0.015f, 0.015f);
	float ammoSize_ = 0.035f; //In percent 

	//Reload
	sf::RectangleShape reloadOutOf_;
	sf::RectangleShape reloadCurrent_;
	sf::Vector2f reloadOffset_ = sf::Vector2f(0.01f, 0.01f);
	sf::Vector2f reloadSize_ = sf::Vector2f(0.075f, 0.01f);
};

#endif	