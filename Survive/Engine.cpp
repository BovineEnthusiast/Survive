#include <stack>
#include "Node.h"
#include "Engine.h"
#include "Camera.h"
#include "SpatialPartition.h"
#include "GUIManager.h"
#include <iostream>
#include <deque>
#include <string>
Engine::Engine()
	:level_(&window_, &soundManager_), menuManager_(&window_, &settingsManager_, &soundManager_)
{}
bool Engine::initialize()
{
	window_.create(sf::VideoMode(1440, 720), "Survive");
	return true;
}

void Engine::update()
{
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    drawPath_ = !drawPath_;
  
	if (inMenu_)
		menuManager_.update(_dT);
	else
	{
		level_.update(_dT);
		if (level_.hasLost())
			inMenu_ = true;
	}
	soundManager_.update(_dT);
}

void Engine::draw()
{
	if (inMenu_)
	{
		window_.draw(menuManager_.getTitleRect());
		if (menuManager_.getCurrentMenu() == 0)
		{
			window_.draw(menuManager_.getSelectionRect());
			window_.draw(menuManager_.getSurviveTitleText());
			window_.draw(menuManager_.getPlayText());
			window_.draw(menuManager_.getSettingsText());
			window_.draw(menuManager_.getQuitText());
		}
		else if (menuManager_.getCurrentMenu() == 1)
		{
			window_.draw(menuManager_.getSelectionRect());
			window_.draw(menuManager_.getSettingsTitleText());
			window_.draw(menuManager_.getAudioText());
			window_.draw(menuManager_.getGameplayText());
			window_.draw(menuManager_.getGraphicsText());
			window_.draw(menuManager_.getBackText());
		}
		else if (menuManager_.getCurrentMenu() == 4)
		{
			window_.draw(menuManager_.getSelectionRect());
			window_.draw(menuManager_.getGraphicsTitleText());
			window_.draw(menuManager_.getVSyncText());
			window_.draw(menuManager_.getWindowText());
			window_.draw(menuManager_.getBackText());
		}
	}
	else
	{
		//Camera view for easier reference
		sf::View camView = level_.getCameraView();

		//Thhe top left and bottom right camera view boundry positions
		sf::Vector2f camTopLeft = sf::Vector2f(camView.getCenter().x - camView.getSize().x / 2, camView.getCenter().y - camView.getSize().y / 2);
		sf::Vector2f camBottomRight = sf::Vector2f(camView.getCenter().x + camView.getSize().x / 2, camView.getCenter().y + camView.getSize().y / 2);

		//Vector positions of tiles to render
		int topLeftX = (camTopLeft.x - fmod(camTopLeft.x, tileSize_)) / tileSize_;
		int topLeftY = (camTopLeft.y - fmod(camTopLeft.y, tileSize_)) / tileSize_;
		int bottomRightX;
		int bottomRightY;

		if (32 - fmod(camBottomRight.x, tileSize_) == 0)
			bottomRightX = (camBottomRight.x / tileSize_);
		else
			bottomRightX = (camBottomRight.x + (32 - fmod(camBottomRight.x, tileSize_))) / tileSize_;
		if (32 - fmod(camBottomRight.y, tileSize_) == 0)
			bottomRightY = (camBottomRight.y / tileSize_);
		else
			bottomRightY = (camBottomRight.y + (32 - fmod(camBottomRight.y, tileSize_))) / tileSize_;

		//Prevents out of bounds exception
		if (bottomRightX > 256)
			bottomRightX = 256;
		if (bottomRightY > 256)
			bottomRightY = 256;


		//Draws Tiles inside the range of the camera
		for (size_t vTile = topLeftX; vTile <= bottomRightX; ++vTile)
			for (size_t tile = topLeftY; tile <= bottomRightY; ++tile)
			{
				level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * tileSize_, (float)tile * tileSize_));
				window_.draw(level_.tiles[vTile][tile].getSprite());
			}


		std::vector<std::vector<SpatialPartition>> spatialPartitions = level_.getSpatialPartitions();

		for (auto iPartitionRow = spatialPartitions.begin(); iPartitionRow != spatialPartitions.end(); ++iPartitionRow)
			for (auto iPartition = iPartitionRow->begin(); iPartition != iPartitionRow->end(); ++iPartition)
			{
				//Dens
				std::vector<Den> vDens = iPartition->getDens();
				for (auto iDen = vDens.begin(); iDen != vDens.end(); ++iDen)
					window_.draw(iDen->getSprite());


				//Blood splats
				std::deque<BloodSplat> dBloodSplats = iPartition->getBloodSplats();
				for (size_t bloodSplat = 0; bloodSplat < dBloodSplats.size(); ++bloodSplat)
					window_.draw(dBloodSplats.at(bloodSplat).getSprite());

			}
		//Player and gun
		window_.draw(level_.getPlayer().getLegLeftSprite());
		window_.draw(level_.getPlayer().getLegRightSprite());
		window_.draw(level_.getPlayer().getArmLeftSprite());
		window_.draw(level_.getPlayer().getArmRightSprite());
		window_.draw(level_.getPlayer().getHeadSprite());
		window_.draw(level_.getPlayer().getGuns().at(level_.getPlayer().getCurrentGunIndex()).getSprite());

		for (auto iPartitionRow = spatialPartitions.begin(); iPartitionRow != spatialPartitions.end(); ++iPartitionRow)
			for (auto iPartition = iPartitionRow->begin(); iPartition != iPartitionRow->end(); ++iPartition)
			{


				//Zombies
				std::vector<Zombie> vZombies = iPartition->getZombies();
				for (auto iZombie = vZombies.begin(); iZombie != vZombies.end(); ++iZombie)
				{
				  std::stack<Node> nodes = iZombie->getNodes();
					if (!iZombie->isDead())
					{
						window_.draw(iZombie->getLegLeftSprite());
						window_.draw(iZombie->getLegRightSprite());
						window_.draw(iZombie->getArmLeftSprite());
						window_.draw(iZombie->getArmRightSprite());
						window_.draw(iZombie->getHeadSprite());

						while(!nodes.empty() && drawPath_)
						  {
						    sf::Vector2i pos = nodes.top().getPosition();
						    nodes.pop();
						    sf::RectangleShape visualNode(sf::Vector2f(32.0f, 32.0f));
						    visualNode.setOrigin(16.0f, 16.0f);
						    visualNode.setFillColor(sf::Color(255,0,0,25));
						    visualNode.setPosition(pos.x, pos.y);
						    window_.draw(visualNode);
						  }
					}
					else
						window_.draw(iZombie->getCorpseSprite());
				}
				std::vector<Turret> vTurrets = iPartition->getTurrets();
				for (auto& turret : vTurrets)
				{
					window_.draw(turret.getBaseSprite());
					window_.draw(turret.getTurretSprite());
				}
				std::vector<Barricade> vBarricades = iPartition->getBarricades();
				for (auto& barricade : vBarricades)
					window_.draw(barricade.getSprite());

				
				//Draws bullets
				std::list<Bullet> vBullets = iPartition->getBullets();
				for (auto iBullet = vBullets.begin(); iBullet != vBullets.end(); ++iBullet)
				{
					window_.draw(iBullet->getSprite());
				}

				//Draws trees
				std::vector<Tree> vTrees = iPartition->getTrees();
				for (auto iTree = vTrees.begin(); iTree != vTrees.end(); ++iTree)
				{
					window_.draw(iTree->getLowerLeafOne());
					window_.draw(iTree->getLowerLeafTwo());
					window_.draw(iTree->getLowerLeafThree());
					window_.draw(iTree->getLowerLeafFour());
					window_.draw(iTree->getUpperLeafOne());
					window_.draw(iTree->getUpperLeafTwo());
					window_.draw(iTree->getUpperLeafThree());
					window_.draw(iTree->getUpperLeafFour());
					window_.draw(iTree->getTrunk());
				}
			}

		//GUI elements
		GUIManager GUIManagerCopy = level_.getGUIManager();

		window_.draw(GUIManagerCopy.getWaveBackground());
		window_.draw(GUIManagerCopy.getWaveText());
		window_.draw(GUIManagerCopy.getZombiesText());

		window_.draw(GUIManagerCopy.getHealthOutOf());
		window_.draw(GUIManagerCopy.getHealthCurrent());

		//Ammo
		window_.draw(GUIManagerCopy.getReloadOutOf());
		window_.draw(GUIManagerCopy.getReloadCurrent());
		window_.draw(GUIManagerCopy.getTotalAmmo());

		//Store
		window_.draw(GUIManagerCopy.getBuyMenuBackground());
		window_.draw(GUIManagerCopy.getSelectionRect());
		window_.draw(GUIManagerCopy.getPistolText());
		window_.draw(GUIManagerCopy.getMagnumText());
		window_.draw(GUIManagerCopy.getShotgunText());
		window_.draw(GUIManagerCopy.getRifleText());
		window_.draw(GUIManagerCopy.getRocketText());
		window_.draw(GUIManagerCopy.getBarricadeText());
		window_.draw(GUIManagerCopy.getTurretText());
		window_.draw(GUIManagerCopy.getRocketTurretText());
		window_.draw(GUIManagerCopy.getMineText());
		window_.draw(GUIManagerCopy.getBuyMagnumText());
		window_.draw(GUIManagerCopy.getBuyShotgunText());
		window_.draw(GUIManagerCopy.getBuyRifleText());
		window_.draw(GUIManagerCopy.getBuyRocketText());
		window_.draw(GUIManagerCopy.getBuyBarricadeText());
		window_.draw(GUIManagerCopy.getBuyTurretText());
		window_.draw(GUIManagerCopy.getBuyRocketTurretText());
		window_.draw(GUIManagerCopy.getBuyMineText());
	}
}
int Engine::run()
{

	//Game Loop
	while (window_.isOpen())
	{
		// delta time
		_dT = _dTClock.restart();

		update();

		if (inMenu_)
		{
			if (menuManager_.isPlayClicked())
			{
				inMenu_ = false;
				level_.generateLevel(257, 257);
			}
			window_.setView(sf::View(sf::FloatRect(0.0f, 0.0f, window_.getSize().x, window_.getSize().y)));
		}
		else
		{

			//update cam
			level_.setCameraPosition(level_.getPlayer().getPositionGlobal());
			//Clamps the camera to edges
			if (level_.getCameraView().getCenter().x - level_.getCameraView().getSize().x / 2 < 0)
				level_.setCameraPosition(sf::Vector2f(level_.getCameraView().getSize().x / 2, level_.getCameraView().getCenter().y));
			if (level_.getCameraView().getCenter().y - level_.getCameraView().getSize().y / 2 < 0)
				level_.setCameraPosition(sf::Vector2f(level_.getCameraView().getCenter().x, level_.getCameraView().getSize().y / 2));
			if (level_.getCameraView().getCenter().x + level_.getCameraView().getSize().x / 2 > level_.tiles.size() * tileSize_)
				level_.setCameraPosition(sf::Vector2f(level_.tiles.size() * tileSize_ - level_.getCameraView().getSize().x / 2, level_.getCameraView().getCenter().y));
			if (level_.getCameraView().getCenter().y + level_.getCameraView().getSize().y / 2 > level_.tiles.size() * tileSize_)
				level_.setCameraPosition(sf::Vector2f(level_.getCameraView().getCenter().x, level_.tiles.size() * tileSize_ - level_.getCameraView().getSize().y / 2));

			level_.resizeCamera(window_.getSize());
			window_.setView(level_.getCameraView());
			level_.updateGUI(_dT);

		}

		sf::Event event;
		while (window_.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window_.close();
				return 0;
			}
			else if (event.type == sf::Event::Resized)
			{
				if (inMenu_)
				{
					sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
					window_.setView(sf::View(visibleArea));
				}
				else
				{
					//Resets viewport
					level_.resizeCamera(window_.getSize());
					window_.setView(level_.getCameraView());
				}
			}
			else if (event.type == sf::Event::LostFocus)
				windowFocused_ = false;
			else if (event.type == sf::Event::GainedFocus)
				windowFocused_ = true;
		}

		window_.clear(sf::Color(40, 42, 43));
		draw();
		window_.display();
	}
	return 0;
}
