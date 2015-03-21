#include "MenuManager.h"
#include <iostream>

MenuManager::MenuManager(sf::RenderWindow* pWindow, SettingsManager* pSettingsManager, SoundManager* pSoundManager)
	:pWindow_(pWindow), pSettingsManager_(pSettingsManager), pSoundManager_(pSoundManager)
{
	if (!font_.loadFromFile("assets/fonts/font.otf"))
	{
		std::cout << "Failed to load font." << std::endl;
	}
	selectionRect_.setOrigin(sf::Vector2f(0, 0));

	titleRect_.setFillColor(sf::Color(255, 0, 55, 200));
	titleRect_.setOrigin(0.0f, 0.0f);

	setUp(surviveTitleText_);
	setUp(playText_);
	setUp(settingsText_);
	setUp(quitText_);
	setUp(settingsTitleText_);
	setUp(audioText_);
	setUp(gameplayText_);
	setUp(graphicsText_);
	setUp(gameplayTitleText_);
	setUp(mapSizeText_);
	setUp(difficultyMultiplierText_);
	setUp(graphicsTitleText_);
	setUp(windowText_);
	setUp(vSyncText_);
	setUp(backText_);


	//Assigns all the strings to the texts
	surviveTitleText_.setString("Survive");
	playText_.setString("Play");
	settingsText_.setString("Settings");
	quitText_.setString("Quit");
	settingsTitleText_.setString("Settings");
	audioText_.setString("Audio");
	gameplayText_.setString("Gameplay");
	graphicsText_.setString("Graphics");
	gameplayTitleText_.setString("Gameplay");
	mapSizeText_.setString("Map Size: 257");
	difficultyMultiplierText_.setString("Difficulty: Easy");
	graphicsTitleText_.setString("Graphics");
	windowText_.setString("Window: Windowed");
	vSyncText_.setString("V-Sync: Off");
	backText_.setString("Back");

	
}

void MenuManager::update(const sf::Time& dT)
{
	selectionRect_.setFillColor(sf::Color::Transparent);

	if (!firstUpdateRan_)
	{
		//Window Mode
		int windowMode = pSettingsManager_->getWindowMode();
		if (windowMode == windowed)
		{
			currentWindowMode_ = windowed;
			windowText_.setString("Window: Windowed");
			pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::Default);
		}
		else if (windowMode == fullscreen)
		{
			currentWindowMode_ = fullscreen;
			pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::Fullscreen);
			windowText_.setString("Window: Fullscreen");
		}
		else
		{
			currentWindowMode_ = borderless;
			pWindow_->create(sf::VideoMode::getDesktopMode(), "Survive", sf::Style::None);
			windowText_.setString("Window: Borderless");
		}
		
		//VSync
		int vSync = pSettingsManager_->getVSync();
		if (vSync == 0)
		{
			vSync_ = false;
			pWindow_->setVerticalSyncEnabled(false);
			vSyncText_.setString("V-Sync: Off");
		}
		else
		{
			vSync_ = true;
			pWindow_->setVerticalSyncEnabled(true);
			vSyncText_.setString("V-Sync: On");
		}
		firstUpdateRan_ = true;
	}

	if (clicked_ && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		clicked_ = false;
	}
	else if (!clicked_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		clicked_ = true;
		click_ = true;
	}
	else if (clicked_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		click_ = false;

	sf::Vector2f windowSize = pWindow_->getView().getSize();
	if (currentMenu_ == mainMenu)
	{
		highlight(surviveTitleText_);
		position(surviveTitleText_, 1);
		position(playText_, 3);
		position(settingsText_, 4);
		position(quitText_, 5);

		if (hover(playText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			playClicked_ = true;
		else if (hover(settingsText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			currentMenu_ = settingsMenu;
		else if (hover(quitText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			pWindow_->close();
		
	}
	else if (currentMenu_ == settingsMenu)
	{
		selectionRect_.setFillColor(sf::Color::Transparent);
		highlight(settingsTitleText_);
		position(settingsTitleText_, 1);
		position(audioText_, 3);
		position(gameplayText_, 4);
		position(graphicsText_, 5);
		position(backText_, 6);
		
		if (hover(audioText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			currentMenu_ = audioMenu;
		else if (hover(gameplayText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			currentMenu_ = gameplayMenu;
		else if (hover(graphicsText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			currentMenu_ = graphicsMenu;
		}
		else if (hover(backText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			currentMenu_ = mainMenu;


	}
	else if (currentMenu_ == graphicsMenu)
	{
		selectionRect_.setFillColor(sf::Color::Transparent);
		highlight(graphicsTitleText_);
		position(graphicsTitleText_, 1);
		position(vSyncText_, 3);
		position(windowText_, 4);
		position(backText_, 5);

		if (hover(vSyncText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (vSync_)
			{
				vSync_ = false;
				vSyncText_.setString("V-Sync: Off");
				pWindow_->setVerticalSyncEnabled(false);
				pSettingsManager_->setVSync(0);
				pSettingsManager_->save();
			}
			else
			{
				vSync_ = true;
				vSyncText_.setString("V-Sync: On");
				pWindow_->setVerticalSyncEnabled(true);
				pSettingsManager_->setVSync(1);
				pSettingsManager_->save();
			}
		}
		else if (hover(windowText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (currentWindowMode_ == windowed)
			{
				currentWindowMode_ = fullscreen;
				pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::Fullscreen);
				windowText_.setString("Window: Fullscreen");
				pSettingsManager_->setWindowMode(fullscreen);
				pSettingsManager_->save();
			}
			else if (currentWindowMode_ == fullscreen)
			{
				currentWindowMode_ = borderless;
				pWindow_->create(sf::VideoMode::getDesktopMode(), "Survive", sf::Style::None);
				windowText_.setString("Window: Borderless");
				pSettingsManager_->setWindowMode(borderless);
				pSettingsManager_->save();
			}
			else
			{
				currentWindowMode_ = windowed;
				pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::Default);
				windowText_.setString("Window: Windowed");
				pSettingsManager_->setWindowMode(windowed);
				pSettingsManager_->save();
			}

		}
		else if (hover(backText_) && click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			currentMenu_ = settingsMenu;

	}
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && selectionRect_.getFillColor() == sf::Color(255, 0, 55, 200))
	//	pSoundManager_->playSound("click");

	//Plays hover sound
	if (!soundPlayed_ && selectionRect_.getFillColor() == sf::Color(255, 0, 55, 200))
	{
		pSoundManager_->playSound("hover", sf::Vector2f(1.0f, 1.0f), sf::Vector2f(1.0f, 1.0f));
		soundPlayed_ = true;
	}
	else if (selectionRect_.getFillColor() != sf::Color(255, 0, 55, 200))
		soundPlayed_ = false;
}
void MenuManager::setUp(sf::Text& text)
{
	text.setFont(font_);
	text.setColor(sf::Color(226, 232, 235));
}
void MenuManager::position(sf::Text& text, const int multiplier)
{
	sf::Vector2f windowSize = pWindow_->getView().getSize();

	if (multiplier == 1)
	{
		text.setCharacterSize((int)(titleSize_ * windowSize.y));
		text.setOrigin(sf::Vector2f(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f));
		text.setPosition(windowSize.x / 2.0f, windowSize.y * textOffset_);
	}
	else
	{
		text.setCharacterSize((int)(buttonSize_ * windowSize.y));
		text.setOrigin(sf::Vector2f(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f));
		text.setPosition(windowSize.x / 2.0f, windowSize.y * textOffset_ * multiplier);
	}

}

bool MenuManager::hover(const sf::Text& text)
{
	if (text.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
	{
		sf::FloatRect bounds = text.getGlobalBounds();
		selectionRect_.setFillColor(sf::Color(255, 0, 55, 200));
		selectionRect_.setSize(sf::Vector2f(bounds.width + bounds.width * 0.75f, bounds.height + bounds.height * 0.25f));
		selectionRect_.setPosition(text.getGlobalBounds().left - text.getGlobalBounds().width * 0.375f, text.getGlobalBounds().top - text.getGlobalBounds().height * 0.175f);
		return true;
	}
	else
		return false;
}

void MenuManager::highlight(const sf::Text& text)
{
	sf::FloatRect bounds = text.getGlobalBounds();
	titleRect_.setFillColor(sf::Color(255, 0, 55, 200));
	titleRect_.setSize(sf::Vector2f(pWindow_->getSize().x, bounds.height + bounds.height * 0.25f));
	titleRect_.setPosition(0, text.getGlobalBounds().top - text.getGlobalBounds().height * 0.175f);
}
//Getters
bool MenuManager::isPlayClicked()
{ 
	if (playClicked_)
	{
		playClicked_ = false;
		return true;
	}
	else
		return false;
}
int MenuManager::getCurrentMenu() const { return currentMenu_; }
sf::RectangleShape MenuManager::getSelectionRect() const { return selectionRect_; }
sf::RectangleShape MenuManager::getTitleRect() const { return titleRect_; }
sf::Text MenuManager::getSurviveTitleText() const { return surviveTitleText_; }
sf::Text MenuManager::getPlayText() const { return playText_; }
sf::Text MenuManager::getSettingsText() const { return settingsText_; }
sf::Text MenuManager::getQuitText() const { return quitText_; }
sf::Text MenuManager::getSettingsTitleText() const { return settingsTitleText_; }
sf::Text MenuManager::getAudioText() const { return audioText_; }
sf::Text MenuManager::getGameplayText() const { return gameplayText_; }
sf::Text MenuManager::getGraphicsTitleText() const { return graphicsTitleText_; }
sf::Text MenuManager::getVSyncText() const { return vSyncText_; }
sf::Text MenuManager::getWindowText() const { return windowText_; }
sf::Text MenuManager::getGraphicsText() const { return graphicsText_; }
sf::Text MenuManager::getBackText() const { return backText_; }
