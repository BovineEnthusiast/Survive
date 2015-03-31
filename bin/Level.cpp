#include "Level.h"
#include "Zombie.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <deque>
#include <SFML/System.hpp>
#include "Collision.h"
#include "Tree.h"
Level::Level(sf::RenderWindow* pWindow, SoundManager* pSoundManager)
	:pWindow_(pWindow), pSoundManager_(pSoundManager), GUIManager_(pSoundManager_, &imageManager_, pWindow_, &player_, &wave_, &zombiesAlive_)
{
	tileSprites_["deepWater"] = sf::IntRect(0, 0, 32, 32);
	tileSprites_["shallowWater"] = sf::IntRect(448, 32, 32, 32);
	tileSprites_["sand"] = sf::IntRect(0, 64, 32, 32);
	tileSprites_["dirt"] = sf::IntRect(448, 96, 32, 32);
	tileSprites_["grass"] = sf::IntRect(0, 128, 32, 32);
	tileSprites_["hill"] = sf::IntRect(448, 160, 32, 32);


	player_.pTiles = &tiles;
	player_.window = pWindow_;

}

void Level::update(const sf::Time& dT)
{
	//Prevents player from firing if the store is open
	if (GUIManager_.isOpen())
		player_.setInStore(true);
	else
		player_.setInStore(false);
	//if (zombiesAlive_ > 0)
	if (zombiesToSpawn_ == 0 && zombiesAlive_ == 0 && !beetweenWaves_)
	{
		waveClock_.restart();
		beetweenWaves_ = true;
	}
	if (waveClock_.getElapsedTime().asSeconds() > waveTime_ && beetweenWaves_)
	{
		++wave_;
		zombiesToSpawn_ = wave_ * 10;
		beetweenWaves_ = false;
	}

	for (auto iPartitionRow = spatialPartitions_.begin(); iPartitionRow != spatialPartitions_.end(); ++iPartitionRow)
		for (auto iPartition = iPartitionRow->begin(); iPartition != iPartitionRow->end(); ++iPartition)
		{
			iPartition->update(dT);
			shake_ += iPartition->getShake();
		}


	if (player_.getHealth() <= 0)
		lost_ = true;


	//Camera shake
	float cameraShake = player_.getShake() + shake_;
	shake_ = 0;
	if (cameraShake != 0)
		camera_.setShake(camera_.getShake() + cameraShake);
	camera_.setPosition(player_.getPositionGlobal());
	camera_.update(dT);

}
void Level::updateGUI(const sf::Time& dT)
{
	GUIManager_.update(dT);

}
//Generates the level with the diamond-square algorithm
void Level::generateLevel(const int width, const int height)
{
	//Resets the level in case of replay
	player_.setHealth(100);
	player_.setPoints(0);
	player_.reset();
	player_.setHasMagnum(false);
	player_.setHasShotgun(false);
	player_.setHasRifle(false);
	player_.setHasRocket(false);
	player_.setTurrets(0);
	player_.setBarricades(0);
	player_.setMines(0);
	dens_ = 0;

	spatialPartitions_.clear();
	tiles.clear();
	GUIManager_.reset();
	wave_ = 1;
	zombiesAlive_ = 0;
	zombiesToSpawn_ = 10;

	//Resizes tiles vectores
	tiles.resize(width - 1);
	for (size_t vec = 0; vec < tiles.size(); ++vec)
		tiles[vec].resize(height - 1);

	spatialPartitions_.clear();
	spatialPartitions_.resize(width / 10 + 1);

	//Seeds random
	std::srand((int)time(0));
	float rangeDeepWater = -0.5f;
	float rangeShallowWater = 0.0f;
	float rangeSand = 0.10f;
	float rangeDirt = 0.15f;
	float rangeGrass = 0.7f;

	//A heightmap array that will (hopefully) end up being between -1 to 1
	std::vector<std::vector<float> > heightmap;
	heightmap.resize(width);
	for (size_t i = 0; i < heightmap.size(); ++i)
		heightmap[i].resize(width);

	//Initial fill of four corners -1 to 0.99
	for (int xPos = 0; xPos < width; ++xPos)
		for (int yPos = 0; yPos < height; ++yPos)
			if (xPos == 0 || yPos == 0 || xPos == width - 1 || yPos == height - 1)
				heightmap[xPos][yPos] = -1.0f;
			else
				heightmap[xPos][yPos] = -10;

	//Initial circle of deep water to guarantee island

	heightmap[0][0] = -1.0f;
	heightmap[width - 1][0] = -1.0f;
	heightmap[0][height - 1] = -1.0f;
	heightmap[width - 1][height - 1] = -1.0f;
	heightmap[(width - 1) / 2][(height - 1) / 2] = 1.0f;












	for (int iteration = 1; iteration <= log10(width - 1) / log10(2) + 1; ++iteration)
	{
		//The size of diamonds or squares
		int shapeSize = (int)((width - 1) / pow(2, iteration - 1));

		//-----------------------------DIAMOND STEP-----------------------------

		//Used to store the square midpoints for the square step
		//vector used instead of static array due to need for length
		std::vector<sf::Vector2i> squareMidpoints((int)(pow(2, iteration - 1) * pow(2, iteration - 1)));

		//Used in order to reset x pos after reaching last square
		int modNumber = 0;
		for (int square = 0; square < pow(2, iteration - 1) * pow(2, iteration - 1); ++square)
		{
			float squareValueAverage = 0;


			//First row of squares separate because X % 0 = :(
			if (square < pow(2, iteration - 1))
			{
				//Adds the values of the four corners to the average, then divides
				squareValueAverage += heightmap[square * shapeSize][0]; //Top-left
				squareValueAverage += heightmap[shapeSize + square * shapeSize][0]; //Top-right
				squareValueAverage += heightmap[square * shapeSize][shapeSize]; //Bottom-left
				squareValueAverage += heightmap[shapeSize + square * shapeSize][shapeSize]; //Bottom-right
				squareValueAverage /= 4;

				//Assigns midpoint and adds a std::random variance that is reduced every iteration
				if (heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] == -10)
					heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = squareValueAverage + (float)((std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration));

				if (heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] > 1)
					heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = 1;
				else if (heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] < -1)
					heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = -1;
				//Stores the midpoint position

				squareMidpoints[square] = sf::Vector2i(square * shapeSize + (shapeSize / 2), shapeSize / 2);
			}
			else
			{
				//Increases the modNumber every time the final square on the x axis is evaluated.
				if (square % (int)pow(2, iteration - 1) == 0)
					modNumber += (int)pow(2, iteration - 1);

				//Adds the values of the four corners to the average, then divides
				squareValueAverage += heightmap[(square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize]; //Top-left
				squareValueAverage += heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize]; //Top-right
				squareValueAverage += heightmap[(square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize]; //Bottom-left
				squareValueAverage += heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize]; //Bottom-right
				squareValueAverage /= 4;


				//Assigns midpoint and adds a std::random variance that is reduced every iteration
				if (heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] == -10)
					heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = squareValueAverage + (float)((std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration));

				//Clamps to -1 to 1
				if (heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] > 1)
					heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = 1;
				else if (heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] < -1)
					heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = -1;
				//Stores midpoint position
				squareMidpoints[square] = sf::Vector2i((square % modNumber) * shapeSize + (shapeSize / 2), (int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2);
			}
		}
		//-----------------------------SQUARE STEP-----------------------------
		//four diamonds for every square midpoint
		for (size_t midpoint = 0; midpoint < squareMidpoints.size(); ++midpoint)
		{

			//---------------Left(square midpoint is right point)---------------

			float leftPointAverage = 0;

			//if the left point is out of array bounds
			if (squareMidpoints[midpoint].x - shapeSize < 0)
			{
				//Gets the average of four points
				leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				leftPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of leftmost point
				leftPointAverage /= 4;

			}
			else
			{
				//Gets the average of four points
				leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				leftPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
				leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize][squareMidpoints[midpoint].y];
				leftPointAverage /= 4;

			}

			//Assigns the average +- std::random to the midpoint of the diamond
			if (heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] == -10)
				heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = leftPointAverage + (float)((std::rand() % 100 - 50) / 50.0f * pow(0.4, iteration));

			//binds it to -1 - 1
			if (heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] > 1)
				heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = 1;
			else if (heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] < -1)
				heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = -1;
			//---------------Right(square midpoint is left point)---------------

			float rightPointAverage = 0;

			//if the right point is out of array bounds
			if (squareMidpoints[midpoint].x + shapeSize > width - 1)
			{
				//Gets the average of four points
				rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				rightPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of rightmost point
				rightPointAverage /= 4;

			}
			else
			{
				//Gets the average of four points
				rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize][squareMidpoints[midpoint].y];
				rightPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
				rightPointAverage /= 4;

			}

			//Assigns the average +- std::random to the midpoint of the diamond
			if (heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] == -10)
				heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = rightPointAverage + (float)((std::rand() % 100 - 50) / 50.0f * pow(0.4, iteration));

			//binds it to -1 - 1
			if (heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] > 1)
				heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = 1;
			else if (heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] < -1)
				heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = -1;
			//---------------Top(square midpoint is bottom point)---------------

			float topPointAverage = 0;

			//if the top point is out of array bounds
			if (squareMidpoints[midpoint].y - shapeSize < 0)
			{
				//Gets the average of four points
				topPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				topPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				topPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of topmost point
				topPointAverage /= 4;

			}
			else
			{
				//Gets the average of four points
				topPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				topPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
				topPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize];
				topPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
				topPointAverage /= 4;
			}

			//Assigns the average +- std::random to the midpoint of the diamond
			if (heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] == -10)
				heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = topPointAverage + (float)((std::rand() % 100 - 50) / 50.0f * pow(0.4, iteration));

			//binds it to -1 - 1
			if (heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] > 1)
				heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = 1;
			else if (heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] < -1)
				heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = -1;
			//---------------Bottom(square midpoint is top point)---------------

			float bottomPointAverage = 0;

			//if the bottom point is out of array bounds
			if (squareMidpoints[midpoint].y + shapeSize > height - 1)
			{
				//Gets the average of four points
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of bottommost point
				bottomPointAverage /= 4;
			}
			else
			{
				//Gets the average of four points
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize];
				bottomPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
				bottomPointAverage /= 4;
			}

			//Assigns the average +- std::random to the midpoint of the diamond
			if (heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] == -10)
				heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = bottomPointAverage + (float)((std::rand() % 100 - 50) / 50.0f * pow(0.4, iteration));

			//binds it to -1 - 1
			if (heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] > 1)
				heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = 1;
			else if (heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] < -1)
				heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = -1;
		}
	}

	for (int yPos = 0; yPos < height; ++yPos)
	{
		for (int xPos = 0; xPos < width; ++xPos)
		{
			//Create a spacial partition if possible
			if (xPos % 10 == 0 && yPos % 10 == 0)
			{
				if (xPos / 10 == 0 && yPos / 10 == 0)
				{
					spatialPartitions_.at(0).push_back(SpatialPartition(sf::FloatRect(xPos * 32.0f, yPos * 32.0f, 320.0f, 320.0f), &player_, &spatialPartitions_, pSoundManager_, &zombiesAlive_, &zombiesToSpawn_, &wave_));
					spatialPartitions_.at(0).back().setImageManagerPointer(&imageManager_);
					spatialPartitions_.at(0).back().setTilesPointer(&tiles);
				}
				else if (yPos / 10 == 0)
				{
					spatialPartitions_.at(0).push_back(SpatialPartition(sf::FloatRect(xPos * 32.0f, yPos * 32.0f, 320.0f, 320.0f), &player_, &spatialPartitions_, pSoundManager_, &zombiesAlive_, &zombiesToSpawn_, &wave_));
					spatialPartitions_.at(yPos / 10).back().setImageManagerPointer(&imageManager_);
					spatialPartitions_.at(yPos / 10).back().setTilesPointer(&tiles);
				}
				else if (xPos / 10 == 0)
				{
					spatialPartitions_.at(yPos / 10).push_back(SpatialPartition(sf::FloatRect(xPos * 32.0f, yPos * 32.0f, 320.0f, 320.0f), &player_, &spatialPartitions_, pSoundManager_, &zombiesAlive_, &zombiesToSpawn_, &wave_));
					spatialPartitions_.at(yPos / 10).back().setImageManagerPointer(&imageManager_);
					spatialPartitions_.at(yPos / 10).back().setTilesPointer(&tiles);
				}
				else if (xPos / 10 == width / 10)
				{
					spatialPartitions_.at(yPos / 10).push_back(SpatialPartition(sf::FloatRect(xPos * 32.0f, yPos * 32.0f, 320.0f, 320.0f), &player_, &spatialPartitions_, pSoundManager_, &zombiesAlive_, &zombiesToSpawn_, &wave_));
					spatialPartitions_.at(yPos / 10).back().setImageManagerPointer(&imageManager_);
					spatialPartitions_.at(yPos / 10).back().setTilesPointer(&tiles);
				}
				else
				{
					spatialPartitions_.at(yPos / 10).push_back(SpatialPartition(sf::FloatRect(xPos * 32.0f, yPos * 32.0f, 320.0f, 320.0f), &player_, &spatialPartitions_, pSoundManager_, &zombiesAlive_, &zombiesToSpawn_, &wave_));
					spatialPartitions_.at(yPos / 10).back().setImageManagerPointer(&imageManager_);
					spatialPartitions_.at(yPos / 10).back().setTilesPointer(&tiles);
				}
			}

			float height = heightmap[xPos][yPos];

			//Sets the heightmap to the tiles' numbers (0 = deep water, 1 = shallow water, etc...)
			if (height < rangeDeepWater)
				heightmap[xPos][yPos] = 0;
			else if (height < rangeShallowWater)
				heightmap[xPos][yPos] = 2;
			else if (height < rangeSand)
				heightmap[xPos][yPos] = 4;
			else if (height < rangeDirt)
				heightmap[xPos][yPos] = 6;
			else if (height < rangeGrass)
				heightmap[xPos][yPos] = 8;
			else
				heightmap[xPos][yPos] = 10;
		}
	}

	for (int xPos = 0; xPos < width - 1; ++xPos)
		for (int yPos = 0; yPos < height - 1; ++yPos)
		{
			int topLeft = (int)heightmap[xPos][yPos];
			int topRight = (int)heightmap[xPos + 1][yPos];
			int bottomLeft = (int)heightmap[xPos][yPos + 1];
			int bottomRight = (int)heightmap[xPos + 1][yPos + 1];

			int hTL = topLeft >> 1;
			int hTR = topRight >> 1;
			int hBL = bottomLeft >> 1;
			int hBR = bottomRight >> 1;

			int saddle = ((topLeft & 1) + (topRight & 1) + (bottomLeft & 1) + (bottomRight & 1) + 1) >> 2;

			int shape = (hTL & 1) | (hTR & 1) << 1 | (hBL & 1) << 2 | (hBR & 1) << 3;

			int ring = (hTL + hTR + hBL + hBR) >> 2;

			int row = ((ring << 1) | saddle) >> 1;
			int col = shape - (ring & 1);


			std::string type;

			
			

			if (row <= 1)
				type = "water";
			else if (row == 5)
				type = "rock";
			else
				type = "walkable";

			tiles[xPos][yPos] = Tile(&imageManager_.tileSheetTexture, sf::IntRect(col * 32, row * 32, 32, 32), type);


			if (type == "walkable" && row == 4)
				player_.setPosition(sf::Vector2f(32.0f * xPos, 32.0f * yPos));
		}

	for (int xPos = 0; xPos < width - 1; ++xPos)
		for (int yPos = 0; yPos < height - 1; ++yPos)
		{
			int topLeft = (int)heightmap[xPos][yPos];
			int topRight = (int)heightmap[xPos + 1][yPos];
			int bottomLeft = (int)heightmap[xPos][yPos + 1];
			int bottomRight = (int)heightmap[xPos + 1][yPos + 1];

			int hTL = topLeft >> 1;
			int hTR = topRight >> 1;
			int hBL = bottomLeft >> 1;
			int hBR = bottomRight >> 1;

			int saddle = ((topLeft & 1) + (topRight & 1) + (bottomLeft & 1) + (bottomRight & 1) + 1) >> 2;

			int shape = (hTL & 1) | (hTR & 1) << 1 | (hBL & 1) << 2 | (hBR & 1) << 3;

			int ring = (hTL + hTR + hBL + hBR) >> 2;

			int row = ((ring << 1) | saddle) >> 1;

			std::string tileOne = ""; //left
			std::string tileTwo = ""; //tl
			std::string tileThree = ""; //t
			std::string tileFour = ""; //tr
			std::string tileFive = ""; // r
			std::string tileSix = ""; //br
			std::string tileSeven = ""; //b
			std::string tileEight = ""; ///bl


			if (xPos != 0)
			{
				tileOne = tiles[xPos - 1][yPos].getType();

				if (yPos != 0)
					tileTwo = tiles[xPos - 1][yPos - 1].getType();
				if (yPos != height - 2)
					tileEight = tiles[xPos - 1][yPos + 1].getType();
			}
			if (yPos != 0)
				tileThree = tiles[xPos][yPos - 1].getType();
			if (yPos != height - 2)
				tileSeven = tiles[xPos][yPos + 1].getType();

			if (xPos != width - 2)
			{
				tileFive = tiles[xPos + 1][yPos].getType();

				if (yPos != 0)
					tileFour = tiles[xPos + 1][yPos - 1].getType();
				if (yPos != height - 2)
					tileSix = tiles[xPos + 1][yPos + 1].getType();
			}
			bool placeable = (tileOne != "tree" && tileOne != "den" && tileOne != "rock")
				&& (tileTwo != "tree" && tileTwo != "den" && tileTwo != "rock")
				&& (tileThree != "tree" && tileThree != "den" && tileThree != "rock")
				&& (tileFour != "tree" && tileFour != "den" && tileFour != "rock")
				&& (tileFive != "tree" && tileFive != "den" && tileFive != "rock")
				&& (tileFour != "tree" && tileSix != "den" && tileSix != "rock")
				&& (tileSeven != "tree" && tileSeven != "den" && tileSeven != "rock")
				&& (tileEight != "tree" && tileEight != "den" && tileEight != "rock");

			//Places trees and dens
			if (placeable && row == 4 && std::rand() % 100 <= 1)
			{
				Tree tree(&imageManager_.treeUpperLeafTexture, &imageManager_.treeLowerLeafTexture, &imageManager_.treeTrunkTexture);
				tree.setPositionGlobal(sf::Vector2f(xPos * 32.0f + 16.0f, yPos * 32.0f + 16.0f));
				spatialPartitions_.at(yPos / 10).at(xPos / 10).pushTree(tree);
				tiles.at(xPos).at(yPos).setType("tree");
			}
		}
	//Ensures 10 dens
	while (dens_ < 10)
		for (int xPos = 0; xPos < width - 1; ++xPos)
			for (int yPos = 0; yPos < height - 1; ++yPos)
			{
				int topLeft = (int)heightmap[xPos][yPos];
				int topRight = (int)heightmap[xPos + 1][yPos];
				int bottomLeft = (int)heightmap[xPos][yPos + 1];
				int bottomRight = (int)heightmap[xPos + 1][yPos + 1];

				int hTL = topLeft >> 1;
				int hTR = topRight >> 1;
				int hBL = bottomLeft >> 1;
				int hBR = bottomRight >> 1;

				int saddle = ((topLeft & 1) + (topRight & 1) + (bottomLeft & 1) + (bottomRight & 1) + 1) >> 2;

				int shape = (hTL & 1) | (hTR & 1) << 1 | (hBL & 1) << 2 | (hBR & 1) << 3;

				int ring = (hTL + hTR + hBL + hBR) >> 2;

				int row = ((ring << 1) | saddle) >> 1;

				if (row == 4)
				{
					std::string tileOne = ""; //left
					std::string tileTwo = ""; //tl
					std::string tileThree = ""; //t
					std::string tileFour = ""; //tr
					std::string tileFive = ""; // r
					std::string tileSix = ""; //br
					std::string tileSeven = ""; //b
					std::string tileEight = ""; ///bl
					std::string tileCurrent = tiles[xPos][yPos].getType();

					if (xPos != 0)
					{
						tileOne = tiles[xPos - 1][yPos].getType();

						if (yPos != 0)
							tileTwo = tiles[xPos - 1][yPos - 1].getType();
						if (yPos != height - 2)
							tileEight = tiles[xPos - 1][yPos + 1].getType();
					}
					if (yPos != 0)
						tileThree = tiles[xPos][yPos - 1].getType();
					if (yPos != height - 2)
						tileSeven = tiles[xPos][yPos + 1].getType();

					if (xPos != width - 2)
					{
						tileFive = tiles[xPos + 1][yPos].getType();

						if (yPos != 0)
							tileFour = tiles[xPos + 1][yPos - 1].getType();
						if (yPos != height - 2)
							tileSix = tiles[xPos + 1][yPos + 1].getType();
					}

					bool placeable = (tileOne != "tree" && tileOne != "den" && tileOne != "rock")
						&& (tileTwo != "tree" && tileTwo != "den" && tileTwo != "rock")
						&& (tileThree != "tree" && tileThree != "den" && tileThree != "rock")
						&& (tileFour != "tree" && tileFour != "den" && tileFour != "rock")
						&& (tileFive != "tree" && tileFive != "den" && tileFive != "rock")
						&& (tileFour != "tree" && tileSix != "den" && tileSix != "rock")
						&& (tileSeven != "tree" && tileSeven != "den" && tileSeven != "rock")
						&& (tileEight != "tree" && tileEight != "den" && tileEight != "rock")
						&& (tileCurrent != "tree" && tileCurrent != "den" && tileCurrent != "rock");

					if (dens_ < 10 && placeable && std::rand() % 1000 < 2)
					{
						++dens_;
						Den den(&imageManager_.zombieDenTexture);
						den.setPositionGlobal(sf::Vector2f(xPos * 32.0f + 16.0f, yPos * 32.0f + 16.0f));
						spatialPartitions_.at(yPos / 10).at(xPos / 10).pushDen(den);

						tiles[xPos - 1][yPos].setType("den");
						tiles[xPos - 1][yPos - 1].setType("den");
						tiles[xPos][yPos - 1].setType("den");
						tiles[xPos + 1][yPos - 1].setType("den");
						tiles[xPos + 1][yPos].setType("den");
						tiles[xPos + 1][yPos + 1].setType("den");
						tiles[xPos][yPos + 1].setType("den");
						tiles[xPos - 1][yPos + 1].setType("den");
						tiles[xPos][yPos].setType("den");
					}
				}
			}
}
//Camera
void Level::moveCamera(const sf::Vector2f& move) { camera_.move(move); }
void Level::zoomCamera(const int zoom) { camera_.changeSize(zoom); }
void Level::resizeCamera(const sf::Vector2u& size) { camera_.resizeView(size); }

//Getters
sf::View Level::getCameraView() { return camera_.getView(); }
Player Level::getPlayer() const { return player_; }
std::vector<std::vector<SpatialPartition>> Level::getSpatialPartitions() const { return spatialPartitions_; }
GUIManager Level::getGUIManager() const { return GUIManager_; }
bool Level::hasLost()
{
	if (lost_)
	{
		lost_ = false;
		return true;
	}
	else
		return false;
}
//Setters
void Level::setCameraPosition(const sf::Vector2f& position) { camera_.setPosition(position); }
