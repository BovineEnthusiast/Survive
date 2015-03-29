#include <stack>
#include "Node.h"
#include "Engine.h"
#include "Camera.h"
#include "SpatialPartition.h"
#include "GUIManager.h"
#include "Emitter.h"
#include "Health.h"
#include "Particle.h"
#include <iostream>
#include <deque>
#include <string>
Engine::Engine()
	:level_(&window_, &soundManager_), menuManager_(&window_, &settingsManager_, &soundManager_)
{}
bool Engine::initialize()
{
	shaderGlow_.loadFromFile("shaders/frag/glow.frag", sf::Shader::Fragment);
	//shaderLighting_.loadFromFile("shaders/frag/lighting.frag", sf::Shader::Fragment);
	window_.create(sf::VideoMode(1440, 720), "Survive");
	return true;
}

void Engine::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
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

		//Used for shaders
		sf::RenderStates addBlendState(sf::BlendAdd);
		//Calculating the light position in window coordinates
		sf::Vector2f windowSize = (sf::Vector2f)window_.getSize();
		sf::View view = window_.getView();
		sf::Vector2f viewSize = view.getSize();
		sf::Vector2f viewTopLeft(view.getCenter().x - viewSize.x / 2.0f, view.getCenter().y - viewSize.y / 2.0f);

		//Vector positions of tiles to render
		int topLeftX = (int)((camTopLeft.x - fmod(camTopLeft.x, tileSize_)) / tileSize_);
		int topLeftY = (int)((camTopLeft.y - fmod(camTopLeft.y, tileSize_)) / tileSize_);
		int bottomRightX;
		int bottomRightY;

		if (32 - fmod(camBottomRight.x, tileSize_) == 0)
			bottomRightX = (int)(camBottomRight.x / tileSize_);
		else
			bottomRightX = (int)((camBottomRight.x + (32 - fmod(camBottomRight.x, tileSize_))) / tileSize_);
		if (32 - fmod(camBottomRight.y, tileSize_) == 0)
			bottomRightY = (int)(camBottomRight.y / tileSize_);
		else
			bottomRightY = (int)((camBottomRight.y + (32 - fmod(camBottomRight.y, tileSize_))) / tileSize_);

		//Prevents out of bounds exception
		if (bottomRightX > 255)
			bottomRightX = 255;
		if (bottomRightY > 255)
			bottomRightY = 255;


		//Draws Tiles inside the range of the camera
		for (size_t vTile = topLeftX; vTile <= bottomRightX; ++vTile)
			for (size_t tile = topLeftY; tile <= bottomRightY; ++tile)
			{/*
			 shaderGlow_.setParameter("frag_LightOrigin", level_.getPlayer().getPositionGlobal());
			 shaderGlow_.setParameter("frag_LightColor", sf::Color(255.0f, 0.0f, 0.0f, 255.0f));
			 shaderGlow_.setParameter("frag_LightLuminosity", 10.0f);
			 sf::RenderStates rs;
			 rs.shader = &shaderGlow_;
			 rs.blendMode = sf::BlendAdd;*/
				level_.tiles[vTile][tile].setSpritePos(sf::Vector2f((float)vTile * tileSize_ + 16, (float)tile * tileSize_ + 16));
				window_.draw(level_.tiles[vTile][tile].getSprite());
			}


		std::vector<std::vector<SpatialPartition>> spatialPartitions = level_.getSpatialPartitions();

		for (auto iPartitionRow = spatialPartitions.begin(); iPartitionRow != spatialPartitions.end(); ++iPartitionRow)
			for (auto iPartition = iPartitionRow->begin(); iPartition != iPartitionRow->end(); ++iPartition)
			{
				window_.draw(iPartition->getSelectionRect());
				//Dens
				std::vector<Den> vDens = iPartition->getDens();
				for (auto iDen = vDens.begin(); iDen != vDens.end(); ++iDen)
					window_.draw(iDen->getSprite());


				//Blood splats
				std::deque<BloodSplat> dBloodSplats = iPartition->getBloodSplats();
				for (size_t bloodSplat = 0; bloodSplat < dBloodSplats.size(); ++bloodSplat)
					window_.draw(dBloodSplats.at(bloodSplat).getSprite());

				std::vector<Mine> vMines = iPartition->getMines();
				for (auto& mine : vMines)
				{
					window_.draw(mine.getMine());
				}
			}






		Gun currentGun = level_.getPlayer().getGuns().at(level_.getPlayer().getCurrentGunIndex());


		sf::Vector2f windowCoord((currentGun.getBulletSpawnPos() - viewTopLeft) * (windowSize.x / viewSize.x));
		windowCoord = sf::Vector2f(windowCoord.x, windowSize.y - windowCoord.y);
		shaderGlow_.setParameter("frag_LightOrigin", windowCoord);
		shaderGlow_.setParameter("frag_LightColor", sf::Color(255, 255, 0, 255));
		shaderGlow_.setParameter("frag_Attenuation", 0.075f);

		sf::RenderStates rs;
		rs.shader = &shaderGlow_;
		rs.blendMode = sf::BlendAdd;

		if (currentGun.isMuzzleLight())
			for (auto& triangle : currentGun.getMuzzleTriangles())
				window_.draw(triangle, rs);

		for (auto& emitter : currentGun.getEmitters())
		{
			for (auto& particle : emitter.getParticles())
				window_.draw(particle.getParticle());
		}
		for (auto iPartitionRow = spatialPartitions.begin(); iPartitionRow != spatialPartitions.end(); ++iPartitionRow)
			for (auto iPartition = iPartitionRow->begin(); iPartition != iPartitionRow->end(); ++iPartition)
			{

				//Health kits
				for (auto& kit : iPartition->getHealthkits())
					window_.draw(kit.getSprite());

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

						while (!nodes.empty() && drawPath_)
						{
							sf::Vector2i pos = nodes.top().getPosition();
							nodes.pop();
							sf::RectangleShape visualNode(sf::Vector2f(32.0f, 32.0f));
							visualNode.setOrigin(16.0f, 16.0f);
							visualNode.setFillColor(sf::Color(255, 0, 0, 25));
							visualNode.setPosition((float)pos.x, (float)pos.y);
							window_.draw(visualNode);
						}
					}
					else if (iZombie->getType() == "boom")
						for (auto& particle : iZombie->getExplosionEmitter().getParticles())
							window_.draw(particle.getParticle());
					else
						window_.draw(iZombie->getCorpseSprite());
				}

				//Blood particles
				for (auto& emitter : iPartition->getEmitters())
					for (auto& particle : emitter.getParticles())
						window_.draw(particle.getParticle());

				for (auto& turret : iPartition->getTurrets())
				{
					window_.draw(turret.getBaseSprite());
					window_.draw(turret.getTurretSprite());
				}

				for (auto& barricade : iPartition->getBarricades())
					window_.draw(barricade.getSprite());

			}

		//Player and gun
		Player player = level_.getPlayer();
		window_.draw(player.getLegLeftSprite());
		window_.draw(player.getLegRightSprite());
		window_.draw(player.getArmLeftSprite());
		window_.draw(player.getArmRightSprite());
		window_.draw(currentGun.getSprite());

		window_.draw(player.getHeadSprite());


		for (auto iPartitionRow = spatialPartitions.begin(); iPartitionRow != spatialPartitions.end(); ++iPartitionRow)
			for (auto iPartition = iPartitionRow->begin(); iPartition != iPartitionRow->end(); ++iPartition)
			{
				//Draws bullets
				std::list<Bullet> vBullets = iPartition->getBullets();
				for (auto iBullet = vBullets.begin(); iBullet != vBullets.end(); ++iBullet)
				{
					window_.draw(iBullet->getSprite());
					if (iBullet->isRocket())
					{
						Emitter rocketEmitter = iBullet->getRocketEmitter();
						for (auto& particle : rocketEmitter.getParticles())
							window_.draw(particle.getParticle());

						if (iBullet->isHit())						
						    for (auto& particle : iBullet->getExplosionEmitter().getParticles())
							window_.draw(particle.getParticle());
						
					}
				}
				std::vector<Mine> vMines = iPartition->getMines();
				for (auto& mine : vMines)
				{
					Emitter emitter = mine.getEmitter();

					for (auto& particle : emitter.getParticles())
					{
						window_.draw(particle.getParticle());
					}
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
		window_.draw(GUIManagerCopy.getPointsText());

		window_.draw(GUIManagerCopy.getHealthOutOf());
		window_.draw(GUIManagerCopy.getHealthCurrent());

		window_.draw(GUIManagerCopy.getSprintOutOf());
		window_.draw(GUIManagerCopy.getSprintCurrent());

		//Ammo
		window_.draw(GUIManagerCopy.getReloadOutOf());
		window_.draw(GUIManagerCopy.getReloadCurrent());
		window_.draw(GUIManagerCopy.getTotalAmmo());

		//Store		
		window_.draw(GUIManagerCopy.getBuyMenuBackground());
		window_.draw(GUIManagerCopy.getPistolSprite());
		window_.draw(GUIManagerCopy.getMagnumSprite());
		window_.draw(GUIManagerCopy.getShotgunSprite());
		window_.draw(GUIManagerCopy.getRifleSprite());
		window_.draw(GUIManagerCopy.getRocketSprite());
		window_.draw(GUIManagerCopy.getBarricadeSprite());
		window_.draw(GUIManagerCopy.getTurretSprite());
		window_.draw(GUIManagerCopy.getMineSprite());
		window_.draw(GUIManagerCopy.getStoreText());
		window_.draw(GUIManagerCopy.getSelectionRect());
		window_.draw(GUIManagerCopy.getPistolText());
		window_.draw(GUIManagerCopy.getMagnumText());
		window_.draw(GUIManagerCopy.getShotgunText());
		window_.draw(GUIManagerCopy.getRifleText());
		window_.draw(GUIManagerCopy.getRocketText());
		window_.draw(GUIManagerCopy.getBarricadeText());
		window_.draw(GUIManagerCopy.getTurretText());
		window_.draw(GUIManagerCopy.getMineText());
		window_.draw(GUIManagerCopy.getBuyMagnumText());
		window_.draw(GUIManagerCopy.getBuyShotgunText());
		window_.draw(GUIManagerCopy.getBuyRifleText());
		window_.draw(GUIManagerCopy.getBuyRocketText());
		window_.draw(GUIManagerCopy.getBuyBarricadeText());
		window_.draw(GUIManagerCopy.getBuyTurretText());
		window_.draw(GUIManagerCopy.getBuyMineText());
		window_.draw(GUIManagerCopy.getBuyPistolAmmoText());
		window_.draw(GUIManagerCopy.getBuyMagnumAmmoText());
		window_.draw(GUIManagerCopy.getBuyShotgunAmmoText());
		window_.draw(GUIManagerCopy.getBuyRifleAmmoText());
		window_.draw(GUIManagerCopy.getBuyRocketAmmoText());
	}
}
int Engine::run()
{

	//Game Loop
	while (window_.isOpen())
	{
		//Force clicks(holding a key doesn't mean repeated clicks)
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			escapeClickable_ = true;

		// delta time
		_dT = _dTClock.restart();
		update();

		if (inMenu_)
		{
			if (menuManager_.isPlayClicked() || (paused_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && escapeClickable_))
			{
				escapeClickable_ = false;
				inMenu_ = false;
				
				if(!paused_)
					level_.generateLevel(257, 257);
				else
					paused_ = false;
			}
			window_.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)window_.getSize().x, (float)window_.getSize().y)));
		}
		else
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !level_.getGUIManager().isOpen() && escapeClickable_)
			{
				escapeClickable_ = false;
				paused_ = true;
				inMenu_ = true;
			}

			//update cam
			//	level_.setCameraPosition(level_.getPlayer().getPositionGlobal());
			//Clamps the camera to edges
			sf::View view = level_.getCameraView();
			sf::Vector2f size = view.getSize();
			sf::Vector2f center = view.getCenter();
			int vectorSize = level_.tiles.size();

			if (center.x - size.x / 2 < 0)
				level_.setCameraPosition(sf::Vector2f(size.x / 2, center.y));
			if (center.y - size.y / 2 < 0)
				level_.setCameraPosition(sf::Vector2f(center.x, size.y / 2));
			if (center.x + size.x / 2 > vectorSize * tileSize_)
				level_.setCameraPosition(sf::Vector2f(vectorSize * tileSize_ - size.x / 2, center.y));
			if (center.y + size.y / 2 > vectorSize * tileSize_)
				level_.setCameraPosition(sf::Vector2f(center.x, vectorSize * tileSize_ - size.y / 2));

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
					sf::FloatRect visibleArea(0.0f, 0.0f, (float)event.size.width, (float)event.size.height);
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
