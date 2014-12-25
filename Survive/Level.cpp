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
	:pWindow_(pWindow), pSoundManager_(pSoundManager)
{
    //Loads the tile sheet then assigns their locations to a map
    if(!tileSpriteSheet_.loadFromFile("TileSpritesheet.png"))
        std::cout << "failed to load from file" << std::endl;
    tileSprites_["deepWater"] = sf::IntRect(0, 0, 60, 60);
    tileSprites_["shallowWater"] = sf::IntRect(60, 0, 60, 60);
    tileSprites_["sand"] = sf::IntRect(120, 0, 60, 60);
    tileSprites_["dirt"] = sf::IntRect(180, 0, 60, 60);
    tileSprites_["dirtyGrass"] = sf::IntRect(240, 0, 60, 60);
    tileSprites_["grass"] = sf::IntRect(300, 0, 60, 60);
    tileSprites_["hill"] = sf::IntRect(360, 0, 60, 60);
    tileSprites_["snow"] = sf::IntRect(420, 0, 60, 60);

    
    player_.pTiles = &tiles;
    player_.window = pWindow_;

}

void Level::update(const sf::Time& dT)
{
    for(auto iPartition = spatialPartitions_.begin(); iPartition != spatialPartitions_.end(); ++iPartition)
        iPartition->update(dT);

    GUIManager_.update(dT);


	if (player_.getHealth() <= 0)
		lost_ = true;
    //Adds zombies if current amount is less than max
}

//Generates the level with the diamond-square algorithm
void Level::generateLevel(const int width, const int height) 
{
	//Resets the level in case of replay
	player_.setHealth(100);
	spatialPartitions_.clear();
	tiles.clear();

    //Resizes tiles vectores
    tiles.resize(width);
    for(int vec = 0; vec < tiles.size(); ++vec)
        tiles[vec].resize(height);
    
    //Seeds random
    std::srand(time(0));
    
    float rangeDeepWater = -0.8f;
    float rangeShallowWater = -0.6f;
    float rangeSand = -0.4f;
    float rangeDirt = -0.2f;
    float rangeDirtyGrass = -0.1f;
    float rangeGrass = 0.6f;
    float rangeHill = 0.8f;
    float rangeSnow = 1.0f;
    
    //A heightmap array that will (hopefully) end up being between -1 to 1
	std::vector<std::vector<float>> heightmap;
	heightmap.resize(width);
	for (int i = 0; i < heightmap.size(); ++i)
		heightmap[i].resize(width);
    
    //Initial fill of four corners -1 to 0.99
    for(int xPos = 0; xPos < width; ++xPos)
        for(int yPos = 0; yPos < height; ++yPos)
            heightmap[xPos][yPos] = -10;
    
    //Initial circle of deep water to guarantee island
   
    heightmap[0][0] = (std::rand() % 200) / 100 * - 1.0f;
    heightmap[width - 1][0] = (std::rand() % 200) / 100 * - 1.0f;
    heightmap[0][height - 1] = (std::rand() % 200) / 100 * - 1.0f;
    heightmap[width - 1][height - 1] = (std::rand() % 200) / 100 * - 1.0f;
    heightmap[(width - 1) / 2][(height - 1) / 2] = 1.0f;
   
    //Island seeds
    


   
    

        
    
    
    //The amount to multiply the displacement iteration
    float displacementMultiplier = 0.5f;
    
    for(int iteration = 1; iteration <= log10(width - 1) / log10(2) + 1; ++iteration)
    {
        //The size of diamonds or squares
        int shapeSize =  (width - 1) / pow(2, iteration - 1);
        
        //-----------------------------DIAMOND STEP-----------------------------
        
        //Used to store the square midpoints for the square step
        //vector used instead of static array due to need for length
        std::vector<sf::Vector2i> squareMidpoints(pow(2, iteration - 1) * pow(2, iteration - 1));
        
        //Used in order to reset x pos after reaching last square
        int modNumber = 0;
        for(int square = 0; square < pow(2, iteration - 1) * pow(2, iteration - 1); ++square)
        {
            float squareValueAverage = 0;
            
           
            //First row of squares separate because X % 0 = :(
            if(square < pow(2, iteration - 1))
            {
                //Adds the values of the four corners to the average, then divides
                squareValueAverage += heightmap[square * shapeSize][0]; //Top-left
                squareValueAverage += heightmap[shapeSize + square * shapeSize][0]; //Top-right
                squareValueAverage += heightmap[square * shapeSize][shapeSize]; //Bottom-left
                squareValueAverage += heightmap[shapeSize + square * shapeSize][shapeSize]; //Bottom-right
                squareValueAverage /= 4;
                
                //Assigns midpoint and adds a std::random variance that is reduced every iteration
                if(heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] == -10)
                    heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = squareValueAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.6, iteration);  
               
                if(heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] > 1)
                    heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = 1;
                else if(heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] < -1)
                    heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = -1;
                //Stores the midpoint position
                
                squareMidpoints[square] = sf::Vector2i(square * shapeSize + (shapeSize / 2), shapeSize / 2);
            }
            else
            {
                //Increases the modNumber every time the final square on the x axis is evaluated.
                if(square % (int)pow(2, iteration - 1) == 0)
                    modNumber += pow(2, iteration - 1);
                
                //Adds the values of the four corners to the average, then divides
                squareValueAverage += heightmap[(square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize]; //Top-left
                squareValueAverage += heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize]; //Top-right
                squareValueAverage += heightmap[(square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize]; //Bottom-left
                squareValueAverage += heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize]; //Bottom-right
                squareValueAverage /= 4;       
                

                //Assigns midpoint and adds a std::random variance that is reduced every iteration
                if(heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] == -10)
                    heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = squareValueAverage  + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.6, iteration);
                
                //Clamps to -1 to 1
                if(heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] > 1)
                    heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = 1;
                else if(heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] < -1)
                    heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = -1;
                //Stores midpoint position
                squareMidpoints[square] = sf::Vector2i((square % modNumber) * shapeSize + (shapeSize / 2), (int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2);
            }             
        }
        //-----------------------------SQUARE STEP-----------------------------
        //four diamonds for every square midpoint
        for(int midpoint = 0; midpoint < squareMidpoints.size(); ++midpoint)
        {
            
            //---------------Left(square midpoint is right point)---------------
            
            float leftPointAverage = 0;
            
            //if the left point is out of array bounds
            if(squareMidpoints[midpoint].x - shapeSize < 0)
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
            if(heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] == -10)
                heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = leftPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            
            //binds it to -1 - 1
            if(heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] > 1)
                heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = 1;
            else if(heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] < -1)
                heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = -1;
            //---------------Right(square midpoint is left point)---------------
            
            float rightPointAverage = 0;
            
            //if the right point is out of array bounds
            if(squareMidpoints[midpoint].x + shapeSize > width - 1)
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
            if(heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] == -10)
                heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = rightPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            
            //binds it to -1 - 1
            if(heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] > 1)
                heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = 1;
            else if(heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] < -1)
                heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = -1;
            //---------------Top(square midpoint is bottom point)---------------
            
            float topPointAverage = 0;
            
            //if the top point is out of array bounds
            if(squareMidpoints[midpoint].y - shapeSize < 0)
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
            if(heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] == -10)
                heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = topPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
           
            //binds it to -1 - 1
            if(heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] > 1)
                heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = 1;
            else if(heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] < -1)
                heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = -1;
            //---------------Bottom(square midpoint is top point)---------------
            
            float bottomPointAverage = 0;
            
            //if the bottom point is out of array bounds
            if(squareMidpoints[midpoint].y + shapeSize > height - 1)
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
            if(heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] == -10)
                heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = bottomPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            
            //binds it to -1 - 1
            if(heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] > 1)
                heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = 1;
            else if(heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] < -1)
                heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = -1;
        }  
    }
    
    //Initializes gradientArray to 1's
	std::vector<std::vector<float>> gradientArray;
	gradientArray.resize(width);
	for (int i = 0; i < gradientArray.size(); ++i)
		gradientArray[i].resize(width);
    for(int xPos = 0; xPos < width; ++xPos)
        {
            for(int yPos = 0; yPos < height; ++yPos)
            {
                gradientArray[xPos][yPos] = 2;
            }
        }
    //Fills gradientArray array with (5-7) circle gradients
   /* for(int circle = 0; circle < 9; ++ circle)
    {
        int xCenter = std::rand() % (int)(width / 4.0f) + (int)(width / 8.0f);
        int yCenter = std::rand() % (int)(width / 4.0f) + (int)(width / 8.0f);
        int radius = std::rand() % (int)(width / 10.0f) + width / 10.0f;
        
        for(int xPos = 0; xPos < width; ++xPos)
        {
            for(int yPos = 0; yPos < height; ++yPos)
            {
                if(sqrt(pow(xPos - xCenter, 2) + pow(yPos - yCenter, 2)) <= radius)
                    gradientArray[xPos][yPos] -= (1 - (sqrt(pow(xPos - xCenter, 2) + pow(yPos - yCenter, 2)) / radius));
                if(gradientArray[xPos][yPos] < 0)
                    gradientArray[xPos][yPos] = 0;
            }
        }
    }*/
    //Adds tiles to tile array
    int xCenter = (width - 1) / 2;
    int yCenter = xCenter;
    //Used to place player on a green tile
    bool playerSet = false; 
    for(int xPos = 0; xPos < width; ++xPos)
    {
       for(int yPos = 0; yPos < height; ++yPos)
        {    
           //Create a spacial partition if possible
           if(xPos % 10 == 0 && yPos % 10 == 0)
           {
               spatialPartitions_.push_back(SpatialPartition(sf::FloatRect(xPos * 50.0f ,yPos * 50.0f, 500.0f ,500.0f), &player_, &spatialPartitions_, pSoundManager_));
               spatialPartitions_.at(spatialPartitions_.size() - 1).setImageManagerPointer(&imageManager_);
               spatialPartitions_.at(spatialPartitions_.size() - 1).setTilesPointer(&tiles);

           }
           
            heightmap[xPos][yPos] -= sqrt(pow(xPos - width / 2, 2) + pow(yPos - height / 2, 2)) * 0.025f ;
            //Assigns tiles based on height
            float height = heightmap[xPos][yPos];
            
            //Places trees
            if(height > rangeDirtyGrass && height < rangeHill && std::rand() % 100 <= 2)
            {
                Tree tree = Tree(&imageManager_.treeUpperLeafTexture, &imageManager_.treeLowerLeafTexture, &imageManager_.treeTrunkTexture);
                tree.setPositionGlobal(sf::Vector2f(xPos * 50 + 25, yPos * 50 + 25));
                spatialPartitions_.at(spatialPartitions_.size() - 1).pushTree(tree);
            }
            else if(height > rangeSand && height < rangeHill && std::rand() % 100 <= 1)
            {
                Tree tree = Tree(&imageManager_.treeUpperLeafTexture, &imageManager_.treeLowerLeafTexture, &imageManager_.treeTrunkTexture);
                tree.setPositionGlobal(sf::Vector2f(xPos * 50 + 25, yPos * 50 + 25));
                spatialPartitions_.at(spatialPartitions_.size() - 1).pushTree(tree);
            }
            if(height > rangeShallowWater && height < rangeHill && std::rand() % 2500 < 5)
            {
                Den den = Den(&imageManager_.zombieDenTexture);
                den.setPositionGlobal(sf::Vector2f(xPos * 50 + 25, yPos * 50 + 25));
                spatialPartitions_.at(spatialPartitions_.size() - 1).pushDen(den);
            }
            
            //Assigns tiles
            if(height < rangeDeepWater)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["deepWater"], "deepWater");
            else if(height < rangeShallowWater)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["shallowWater"], "shallowWater");
            else if(height < rangeSand)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["sand"], "sand");
            else if(height < rangeDirt)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["dirt"], "dirt");
            else if(height < rangeDirtyGrass)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["dirtyGrass"], "dirtyGrass");
            else if(height < rangeGrass)
            {
                if(!playerSet)
                {
                    player_.setPosition(sf::Vector2f(xPos * 50, yPos * 50));
                    playerSet = true;
                }
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["grass"], "grass");
            }
            else if(height < rangeHill)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["hill"], "hill");
            else
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["snow"], "snow");
        }
    }
       
	std::cout << "Done gen";
}
//Camera
void Level::moveCamera(const sf::Vector2f& move) {camera_.move(move);}
void Level::zoomCamera(const int zoom) {camera_.changeSize(zoom);}
void Level::resizeCamera(const sf::Vector2u& size) {camera_.resizeView(size);}

//Getters
bool Level::isMenu() const {return menu_;}
sf::View Level::getCameraView() {return camera_.getView();}
Player Level::getPlayer() const {return player_;}
std::vector<SpatialPartition> Level::getSpatialPartitions() const {return spatialPartitions_;}
GUIManager Level::getGUIManager() const {return GUIManager_;}
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
void Level::setCameraPosition(const sf::Vector2f& position) {camera_.setPosition(position);}
