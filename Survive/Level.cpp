#include "Level.h"
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>
Level::Level() 
{
    //Loads the tile sheet then assigns their locations to a map
    if(!tileSpriteSheet_.loadFromFile("TileSpritesheet.png"))
        std::cout << "failed to load from file" << std::endl;
    tileSprites_["deepWater"] = sf::IntRect(0, 0, 50, 50);
    tileSprites_["shallowWater"] = sf::IntRect(50, 0, 100, 100);
    tileSprites_["sand"] = sf::IntRect(100, 0, 150, 150);
    tileSprites_["dirt"] = sf::IntRect(150, 0, 200, 200);
    tileSprites_["grass"] = sf::IntRect(200, 0, 250, 250);
    tileSprites_["hill"] = sf::IntRect(250, 0, 300, 300);
}

void Level::update(const sf::Time& dT)
{
    //NOTE: change this loop to something better
    //for(int i = 0; i < gameObjects_.size(); ++i)
        //gameObjects_[i].update(dT);
    player_.update(dT);
}

//Generates the level with the diamond-square algorithm
void Level::generateLevel(const int width, const int height) 
{
    //Resizes tiles vectores
    tiles.resize(width);
    for(int vec = 0; vec < tiles.size(); ++vec)
        tiles[vec].resize(height);
    
    //Seeds random
    std::srand(time(0));
    
    float rangeDeepWater = -0.3;
    float rangeShallowWater = 0;
    float rangeSand = 0.05;
    float rangeDirt = 0.1;
    float rangeGrass = 0.9;
    float rangeHill = 1;
    //A heightmap array that will (hopefully) end up being between -1 to 1
    float heightmap[width][height];
    
    //Initial fill of four corners -1 to 0.99
    for(int xPos = 0; xPos < width; ++xPos)
        for(int yPos = 0; yPos < height; ++yPos)
            heightmap[xPos][yPos] = -10;
    
    //Initial circle of deep water to guarantee island
    heightmap[0][0] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[width - 1][0] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[0][height - 1] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[width - 1][height - 1] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[(width - 1) / 2][0] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[(width - 1) / 2][height - 1] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[0][(height - 1) / 2] = (std::rand() % 80) / 100 * - 0.2f;
    heightmap[width - 1][(height - 1) / 2] = (std::rand() % 80) / 100 * - 0.2f;
    
    //Island seeds
    heightmap[(width - 1) / 2][(height - 1) / 2] = 0.8f;
    
    heightmap[(width - 1) / 4][(height - 1) / 4] = ((std::rand() % 200) - 100) / 100;
    heightmap[(width - 1) * 3 / 4][(height - 1) / 4] = ((std::rand() % 200) - 100) / 100;
    heightmap[(width - 1) / 4][(height - 1) / 4] = ((std::rand() % 200) - 100) / 100;
    heightmap[(width - 1) * 3 / 4][(height - 1) * 3 / 4] = ((std::rand() % 200) - 100) / 100;


   
    

        
    
    
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
                    heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = squareValueAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);  
               
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
                    heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = squareValueAverage  + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
                
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
    float gradientArray[width][height];
    for(int xPos = 0; xPos < width; ++xPos)
        {
            for(int yPos = 0; yPos < height; ++yPos)
            {
                gradientArray[xPos][yPos] = 2;
            }
        }
    //Fills gradientArray array with (5-7) circle gradients
    for(int circle = 0; circle < 9; ++ circle)
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
    }
    //Adds tiles to tile array
    int xCenter = (width - 1) / 2;
    int yCenter = xCenter;
    for(int xPos = 0; xPos < width; ++xPos)
    {
       for(int yPos = 0; yPos < height; ++yPos)
        {    
            heightmap[xPos][yPos] -= gradientArray[xPos][yPos] * 0.1;
            //heightmap[xPos][yPos] -= sqrt(pow(xPos - xCenter, 2) + pow(yPos - yCenter, 2)) / width * 0.75f;
            //Assigns tiles based on height
            float height = heightmap[xPos][yPos];
            if(height < rangeDeepWater)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["deepWater"]);
            else if(height < rangeShallowWater)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["shallowWater"]);
            else if(height < rangeSand)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["sand"]);
            else if(height < rangeDirt)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["dirt"]);
            else if(height < rangeGrass)
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["grass"]);
            else
                tiles[xPos][yPos] = Tile(tileSpriteSheet_, tileSprites_["hill"]);
        }
    }
       
}
//Camera
void Level::moveCamera(const sf::Vector2f& move) {camera_.move(move);}
void Level::zoomCamera(const int zoom) {camera_.changeSize(zoom);}
void Level::resizeCamera(const sf::Vector2u& size) {camera_.resizeView(size);}
//Getters
bool Level::isMenu() {return menu_;}
sf::View Level::getCameraView() {return camera_.getView();}
Player Level::getPlayer() {return player_;}
//Setters
void Level::setCameraPosition(const sf::Vector2f& position) {camera_.setPosition(position);}
