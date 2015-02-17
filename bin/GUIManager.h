#ifndef GUIMANAGER_H
#define	GUIMANAGER_H
#include <SFML/Graphics.hpp>
#include "Player.h"
class GUIManager
{
public:
    GUIManager(sf::RenderWindow*, Player*, int*, int*);
    void update(const sf::Time&);
    
    //Getters
    sf::RectangleShape getHealthOutOf() const;
    sf::RectangleShape getHealthCurrent() const;
    sf::Text getCurrentAmmo() const;
    sf::Text getTotalAmmo() const;
    sf::RectangleShape getReloadOutOf() const;
    sf::RectangleShape getReloadCurrent() const;
    sf::Text getWaveText() const;
    sf::Text getZombiesText() const;
    sf::RectangleShape getWaveBackground() const;
    sf::RectangleShape getBuyMenuBackground() const;
    sf::RectangleShape getExitButton() const;
    sf::RectangleShape getSelectionRect()  const;
    sf::Text getStoreText() const;
    sf::Text getPistolText() const;
    sf::Text getMagnumText() const;
    sf::Text getShotgunText() const;
    sf::Text getRifleText() const;
    sf::Text getRocketText() const;
    sf::Text getBarricadeText() const;
    sf::Text getTurretText() const; 
    sf::Text getRocketTurretText() const;
    sf::Text getMineText() const;
    sf::Text getBuyMagnumText() const;
    sf::Text getBuyShotgunText() const;
    sf::Text getBuyRifleText() const;
    sf::Text getBuyRocketText() const;
    sf::Text getBuyBarricadeText() const;
    sf::Text getBuyTurretText() const;
    sf::Text getBuyRocketTurretText() const;
    sf::Text getBuyMineText() const;
    sf::Text getPointsText() const;
    
 private:
    //Helpers for text
    void setUpText(sf::Text&, const std::string&);
    void showInStore(sf::Text&, const int, const int, const float);
    bool hover(const sf::Text&);
    sf::RenderWindow* pWindow_;
    Player* pPlayer_;
    sf::Font font_;
    int* pWave_;
    int* pZombiesAlive_;
    
    //Wave info
    sf::Text waveText_;
    sf::Text zombiesText_;
    sf::RectangleShape waveBackground_;
    float waveTextSize_ = 0.05f;
    float zombiesTextSize_ = 0.05f;
    float pointsTextSize_ = 0.05f;
    sf::Vector2f waveBackgroundSize_ = sf::Vector2f(0.2f, 0.21f);
    sf::Vector2f waveTextOffset_ = sf::Vector2f(0.05f, 0.01f);
    sf::Vector2f zombiesTextOffset_ = sf::Vector2f(0.05f, 0.06f);

    //Points
    sf::Text pointsText_;
    sf::Vector2f pointsTextOffset_ = sf::Vector2f(0.05f, 0.11f);
    
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
    
    //----------------Buy menu--------------
    //Basic
    bool storeOpen_ = false;
    bool toggled_ = false;
    float storeOpenTime_ = 0.1f;
    float storeItemsOpenTime_ = 0.1f;
    sf::RectangleShape buyMenuBackground_;
    sf::RectangleShape exitButton_;
    sf::RectangleShape selectionRect_;
    sf::Vector2f storeSize_ = sf::Vector2f(0.9f, 0.6f);
    
    bool magnumBought_ = false;
    bool shotgunBought_ = false;
    bool rifleBought_ = false;
    bool rocketBought_ = false;
    
    //Gun/Item texts
    sf::Text storeText_;
    sf::Text pistolText_;
    sf::Text magnumText_;
    sf::Text shotgunText_;
    sf::Text rifleText_;
    sf::Text rocketText_;
    sf::Text barricadeText_;
    sf::Text turretText_;
    sf::Text rocketTurretText_;
    sf::Text mineText_;
    float itemTextSize_ = 0.05f;
    float titleTextSize_ = 0.1f;
    //Gun/Item prices
    int priceMagnum_ = 1250;
    int priceShotgun_ = 3000;
    int priceRifle_ = 5000;
    int priceBarricade_ = 300;
    int priceTurret_ = 2500;

    //Buy button texts
    sf::Text buyMagnumText_;
    sf::Text buyShotgunText_;
    sf::Text buyRifleText_;
    sf::Text buyRocketText_;
    sf::Text buyBarricadeText_;
    sf::Text buyTurretText_;
    sf::Text buyRocketTurretText_;
    sf::Text buyMineText_;
    
    //Clocks
    sf::Clock storeOpenClock_;
    
    
};

#endif	
