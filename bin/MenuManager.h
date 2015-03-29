#ifndef MENUMANAGER_H
#define MENUMANAGER_H
#include <SFML/Graphics.hpp>
#include "SettingsManager.h"
#include "SoundManager.h"

enum menuType
{
	mainMenu,
	settingsMenu,
	gameplayMenu,
	audioMenu,
	graphicsMenu,
	controlsMenu
};

enum windowMode
{
	windowed,
	fullscreen,
	borderless
};

class MenuManager
{
public:
	MenuManager(sf::RenderWindow*, SettingsManager*, SoundManager*);
	void update(const sf::Time&);

	//Getters
	bool isPlayClicked();
	int getCurrentMenu() const;
	sf::RectangleShape getSelectionRect() const;
	sf::Text getSurviveTitleText() const;
	sf::Text getPlayText() const;
	sf::Text getSettingsText() const;
	sf::Text getQuitText() const;
	sf::Text getSettingsTitleText() const;
	sf::Text getGraphicsTitleText() const;
	sf::Text getGraphicsText() const;
	sf::Text getVSyncText() const;
	sf::Text getWindowText() const;

	sf::Text getBackText() const;

	sf::RectangleShape getTitleRect() const;

private:

	//Helper functions to ease the creation and updating of text
	void setUp(sf::Text&);
	void position(sf::Text&, const int);
	bool hover(const sf::Text&);
	void highlight(const sf::Text&);
	
	bool soundPlayed_ = false;
	sf::RenderWindow* pWindow_;
	SettingsManager* pSettingsManager_;
	SoundManager* pSoundManager_;
	sf::Font font_;
	bool clicked_ = false;
	bool click_ = false;
	bool playClicked_ = false;
	bool firstUpdateRan_ = false;
	bool vSync_ = false;
	//The type of menu that is currently visible
	menuType currentMenu_ = mainMenu;
	//Remembers which state the window is currently in
	windowMode currentWindowMode_ = windowed;
	//Rectangle that appears below selected button/text
	sf::RectangleShape selectionRect_;
	//Rectangle as a backdrop for titles
	sf::RectangleShape titleRect_;
	//Text that is shown at the bottom to describe what the setting does
	sf::Text descriptionText_;

	sf::Text backText_;
	//The sizes of selectable buttons and title texts in percent
	float titleSize_ = 0.15f;
	float buttonSize_ = 0.05f;
	float textOffset_ = 0.1f;
	//==================================Main Screen===================================

	//Text
	sf::Text surviveTitleText_;
	sf::Text playText_;
	sf::Text settingsText_;
	sf::Text quitText_;

	//====================================Settings====================================

	sf::Text settingsTitleText_;
	sf::Text controlsText_;
	sf::Text graphicsText_;

	//====================================Graphics====================================

	sf::Text graphicsTitleText_;
	sf::Text windowText_;
	sf::Text vSyncText_;

	//====================================GAMEPLAY====================================
	sf::Text gameplayTitleText_;
	sf::Text mapSizeText_;
	sf::Text difficultyMultiplierText_;

};
#endif

