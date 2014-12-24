#ifndef MENUMANAGER_H
#define MENUMANAGER_H
#include <SFML/Graphics.hpp>

enum menuType
{
	mainMenu,
	settingsMenu,
	gameplayMenu,
	audioMenu,
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
	MenuManager(sf::RenderWindow*);
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
	sf::Text getAudioText() const;
	sf::Text getGameplayText() const;
	sf::Text getWindowText() const;

	sf::Text getBackText() const;

private:
	sf::RenderWindow* pWindow_;
	sf::Font font_;
	bool clicked_ = false;
	bool click_ = false;
	bool playClicked_ = false;
	//The type of menu that is currently visible
	menuType currentMenu_ = mainMenu;
	//Remembers which state the window is currently in
	windowMode currentWindowMode_ = windowed;
	//Rectangle that appears below selected button/text
	sf::RectangleShape selectionRect_;
	//Text that is shown at the bottom to describe what the setting does
	sf::Text descriptionText_;

	sf::Text backText_;
	//The sizes of selectable buttons and title texts in percent
	float titleSize_ = 0.15f;
	float buttonSize_ = 0.05;
	float titleOffset_ = 0.15f;
	//==================================Main Screen===================================

	//Text
	sf::Text surviveTitleText_;
	sf::Text playText_;
	sf::Text settingsText_;
	sf::Text quitText_;

	//====================================Settings====================================

	sf::Text settingsTitleText_;
	sf::Text audioText_;
	sf::Text gameplayText_;
	sf::Text windowText_;

	//====================================GAMEPLAY====================================
	sf::Text gameplayTitleText_;
	sf::Text mapSizeText_;
	sf::Text difficultyMultiplierText_;

};
#endif

