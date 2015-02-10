#ifndef ENGINE_H
#define	ENGINE_H
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Level.h"
#include "MenuManager.h"
#include "SettingsManager.h"
#include "SoundManager.h"

class Engine 
{
public:
    Engine();
    
    bool initialize();
    int run();  
    
private:
    sf::RenderWindow window_;
    sf::Time _dT;
    sf::Clock _dTClock;
    bool drawPath_ = false;
    Level level_;
	SettingsManager settingsManager_;
	SoundManager soundManager_;
	MenuManager menuManager_;
    int tileSize_ = 32;
    bool windowFocused_ = true;
	bool inMenu_ = true;
    void update();
    void draw();
};

#endif	

