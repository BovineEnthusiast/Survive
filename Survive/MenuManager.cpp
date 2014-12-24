#include "MenuManager.h"
#include <iostream>

MenuManager::MenuManager(sf::RenderWindow* pWindow)
	:pWindow_(pWindow)
{
	if (!font_.loadFromFile("font.otf"))
	{
		//std::cout << "Failed to load font." << std::endl;
	}
	selectionRect_.setFillColor(sf::Color(200, 200, 200, 50));
	selectionRect_.setPosition(sf::Vector2f(-10000.0f, -10000.0f));

	surviveTitleText_.setFont(font_);
	playText_.setFont(font_);
	settingsText_.setFont(font_);
	quitText_.setFont(font_);
	settingsTitleText_.setFont(font_);
	audioText_.setFont(font_);
	gameplayText_.setFont(font_);
	windowText_.setFont(font_);
	gameplayTitleText_.setFont(font_);
	mapSizeText_.setFont(font_);
	difficultyMultiplierText_.setFont(font_);
	backText_.setFont(font_);

	surviveTitleText_.setColor(sf::Color(226, 232, 235));
	playText_.setColor(sf::Color(226, 232, 235));
	settingsText_.setColor(sf::Color(226, 232, 235));
	quitText_.setColor(sf::Color(226, 232, 235));
	settingsText_.setColor(sf::Color(226, 232, 235));
	audioText_.setColor(sf::Color(226, 232, 235));
	gameplayText_.setColor(sf::Color(226, 232, 235));
	windowText_.setColor(sf::Color(226, 232, 235));
	gameplayTitleText_.setColor(sf::Color(226, 232, 235));
	mapSizeText_.setColor(sf::Color(226, 232, 235));
	difficultyMultiplierText_.setColor(sf::Color(226, 232, 235));
	backText_.setColor(sf::Color(226, 232, 235));

	//Assigns all the strings to the texts
	surviveTitleText_.setString("Survive");
	playText_.setString("Play");
	settingsText_.setString("Settings");
	quitText_.setString("Quit");
	settingsTitleText_.setString("Settings");
	audioText_.setString("Audio");
	gameplayText_.setString("Gameplay");
	windowText_.setString("Window: Windowed");
	gameplayTitleText_.setString("Gameplay");
	mapSizeText_.setString("Map Size: 257");
	difficultyMultiplierText_.setString("Difficulty: Easy");
	backText_.setString("Back");
}

void MenuManager::update(const sf::Time& dT)
{

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
	//std::cout << "update?" << std::endl;
	if (currentMenu_ == mainMenu)
	{
		surviveTitleText_.setCharacterSize((int)(titleSize_ * windowSize.y));
		playText_.setCharacterSize((int)(buttonSize_ * windowSize.y));
		settingsText_.setCharacterSize((int)(buttonSize_ * windowSize.y));
		quitText_.setCharacterSize((int)(buttonSize_ * windowSize.y));

		surviveTitleText_.setOrigin(sf::Vector2f(surviveTitleText_.getLocalBounds().width / 2.0f, surviveTitleText_.getLocalBounds().height / 2.0f));
		playText_.setOrigin(sf::Vector2f(playText_.getLocalBounds().width / 2.0f, playText_.getLocalBounds().height / 2.0f));
		settingsText_.setOrigin(sf::Vector2f(settingsText_.getLocalBounds().width / 2.0f, settingsText_.getLocalBounds().height / 2.0f));
		quitText_.setOrigin(sf::Vector2f(quitText_.getLocalBounds().width / 2.0f, quitText_.getLocalBounds().height / 2.0f));

		surviveTitleText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_);
		playText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 2);
		settingsText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 3);
		quitText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 4);


		if (playText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(playText_.getGlobalBounds().width, playText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(playText_.getPosition());

			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				playClicked_ = true;
		}
		else if (settingsText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(settingsText_.getGlobalBounds().width, settingsText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(settingsText_.getPosition());

			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				currentMenu_ = settingsMenu;
		}
		else if (quitText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(quitText_.getGlobalBounds().width, quitText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(quitText_.getPosition());
			
			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				pWindow_->close();
		}
	}
	else if (currentMenu_ = settingsMenu)
	{
		settingsTitleText_.setCharacterSize((int)(titleSize_ * windowSize.y));
		audioText_.setCharacterSize((int)(buttonSize_ * windowSize.y));
		gameplayText_.setCharacterSize((int)(buttonSize_ * windowSize.y));
		windowText_.setCharacterSize((int)(buttonSize_ * windowSize.y));
		backText_.setCharacterSize((int)(buttonSize_ * windowSize.y));

		settingsTitleText_.setOrigin(sf::Vector2f(settingsTitleText_.getGlobalBounds().width / 2.0f, settingsTitleText_.getGlobalBounds().height / 2.0f));
		audioText_.setOrigin(sf::Vector2f(audioText_.getGlobalBounds().width / 2.0f, audioText_.getGlobalBounds().height / 2.0f));
		gameplayText_.setOrigin(sf::Vector2f(gameplayText_.getGlobalBounds().width / 2.0, gameplayText_.getGlobalBounds().height / 2.0f));
		windowText_.setOrigin(sf::Vector2f(windowText_.getGlobalBounds().width / 2.0f, windowText_.getGlobalBounds().height / 2.0f));
		backText_.setOrigin(sf::Vector2f(backText_.getGlobalBounds().width / 2.0f, backText_.getGlobalBounds().height / 2.0f));

		settingsTitleText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_);
		audioText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 2);
		gameplayText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 3);
		windowText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 4);
		backText_.setPosition(windowSize.x / 2.0f, windowSize.y * titleOffset_ + windowSize.y * 0.15f * 5);
		
		if (audioText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(audioText_.getGlobalBounds().width, audioText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(audioText_.getPosition());

			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				currentMenu_ = audioMenu;
		}
		else if (gameplayText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(gameplayText_.getGlobalBounds().width, gameplayText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(gameplayText_.getPosition());

			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				currentMenu_ = gameplayMenu;
		}
		else if (windowText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(windowText_.getGlobalBounds().width, windowText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(windowText_.getPosition());

			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (currentWindowMode_ == windowed)
				{
					currentWindowMode_ = fullscreen;
					pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::Fullscreen);
					windowText_.setString("Window: Fullscreen");
				}
				else if (currentWindowMode_ == fullscreen)
				{
					currentWindowMode_ = borderless;
					pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::None);
					windowText_.setString("Window: Borderless");
				}
				else
				{
					currentWindowMode_ = windowed;
					pWindow_->create(sf::VideoMode(pWindow_->getSize().x, pWindow_->getSize().y), "Survive", sf::Style::Default);
					windowText_.setString("Window: Windowed");
				}
			}
		}
		else if (backText_.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*pWindow_)))
		{
			selectionRect_.setSize(sf::Vector2f(backText_.getGlobalBounds().width, backText_.getGlobalBounds().height));
			selectionRect_.setOrigin(sf::Vector2f(selectionRect_.getSize().x / 2.0f, selectionRect_.getSize().y / 2.0f));
			selectionRect_.setPosition(backText_.getPosition());

			if (click_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				currentMenu_ = mainMenu;
		}
	}
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
sf::Text MenuManager::getSurviveTitleText() const { return surviveTitleText_; }
sf::Text MenuManager::getPlayText() const { return playText_; }
sf::Text MenuManager::getSettingsText() const { return settingsText_; }
sf::Text MenuManager::getQuitText() const { return quitText_; }
sf::Text MenuManager::getSettingsTitleText() const { return settingsTitleText_; }
sf::Text MenuManager::getAudioText() const { return audioText_; }
sf::Text MenuManager::getGameplayText() const { return gameplayText_; }
sf::Text MenuManager::getWindowText() const { return windowText_; }

sf::Text MenuManager::getBackText() const { return backText_; }