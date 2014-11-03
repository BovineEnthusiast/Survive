#include "Level.h"
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>
Level::Level() 
{
}

//Generates the level with the diamond-square algorithm
void Level::generateLevel(const int width, const int height) 
{
    std::srand(time(0));
    
    //A heightmap array that will (hopefully) end up being between -1 to 1
    float heightmap[width][height];
    
    //Initial fill of four corners -1 to 0.99
    heightmap[0][0] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[width - 1][0] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[0][height - 1] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[width - 1][height - 1] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[(width - 1) / 2][(height - 1) / 2] = (float)(std::rand() % 50) / 50 + 0.25;
    
    //Used to finalize the array to be -1 to 1
   // float highestValue = initialCornerValue;
   // float lowestValue = initialCornerValue;
    
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
                heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = squareValueAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);   
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
                heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / pow(2, iteration - 1)) * shapeSize + shapeSize / 2] = squareValueAverage  + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);

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
            heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = leftPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            
            
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
            heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = rightPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
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
            heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = topPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
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
            heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = bottomPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
        }  
    }
    
    //Subtracts a circle gradient from the heightmap in order to create islands
    for(int xPos = 0; xPos < width; ++xPos)
    {
       for(int yPos = 0; yPos < height; ++yPos)
        {
            //subtracts the distance over width from the center
            //This creates an island-like shape
            heightmap[xPos][yPos] -= sqrt(pow(xPos - (width - 1) / 2, 2) + pow(yPos - (height - 1) / 2, 2)) / (width / 2);
        }
    }
    

   /*for(int i = 0; i < width; ++i)
    {
        std::cout << "[ ";
        for(int j = 0; j < height; ++j)
        {
            if(heightmap[i][j] < -0.75)
                std::cout << " ";
            else if(heightmap[i][j] < -0.5)
                std::cout << "1";
            else if(heightmap[i][j] < -0.25)
                std::cout << "2";
            else if(heightmap[i][j] < 0)
                std::cout << "3";
            else if(heightmap[i][j] < 0.25)
                std::cout << "4";
            else if(heightmap[i][j] < 0.5)
                std::cout << "5";
            else if(heightmap[i][j] < 0.75)
                std::cout << "6";
            else
                std::cout << "7";
        }
        std::cout << "]" << std::endl;    
    }*/
       
}