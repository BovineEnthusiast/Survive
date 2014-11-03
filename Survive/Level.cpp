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
    
    //A heightmap array that will end up being between -1 to 1
    float heightmap[width][height];
    
    //Initial fill of four corners -1 to 0.99
    heightmap[0][0] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[width - 1][0] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[0][height - 1] = (float)(std::rand() % 100 - 50) / 50.0f;
    heightmap[width - 1][height - 1] = (float)(std::rand() % 100 - 50) / 50.0f;
    std::cout << (float)(std::rand() % 100 - 50) / 50.0f << std::endl;
    
    //Used to finalize the array to be -1 to 1
   // float highestValue = initialCornerValue;
   // float lowestValue = initialCornerValue;
    
    //The amount to multiply the displacement iteration
    float displacementMultiplier = 0.5f;
    
    for(int iteration = 1; iteration <= log10(width - 1) / log10(2) + 1; ++iteration)
    {
        //The size of diamonds or squares
        int shapeSize =  (width - 1) / iteration;
        
        //-----------------------------DIAMOND STEP-----------------------------
        
        //Used to store the square midpoints for the square step
        //vector used instead of static array due to need for length
        std::vector<sf::Vector2i> squareMidpoints(iteration * iteration);
        
        //Used in order to reset x pos after reaching last square
        int modNumber = 0;
        std::cout << "-----DIAMOND STEP-----" << std::endl;
        for(int square = 0; square < pow(2, iteration - 1) * pow(2, iteration - 1); ++square)
        {
            std::cout << "Square: " << square << " Iteration: " << iteration << std::endl;
            float squareValueAverage = 0;
            
            //First row of squares separate because X % 0 = :(
            if(square < iteration)
            {
                //Adds the values of the four corners to the average, then divides
                squareValueAverage += heightmap[square * shapeSize][0]; //Top-left
                std::cout << "[" << square * shapeSize << "][0] = " << heightmap[square * shapeSize][0] << std::endl;
                squareValueAverage += heightmap[shapeSize + square * shapeSize][0]; //Top-right
                std::cout << "[" << shapeSize + square * shapeSize << "][0] = " << heightmap[shapeSize + square * shapeSize][0] << std::endl;
                squareValueAverage += heightmap[square * shapeSize][shapeSize]; //Bottom-left
                std::cout << "[" << square * shapeSize << "][" << shapeSize << "] = " << heightmap[square * shapeSize][shapeSize] << std::endl;
                squareValueAverage += heightmap[shapeSize + square * shapeSize][shapeSize]; //Bottom-right
                std::cout << "[" << shapeSize + square * shapeSize << "][" << shapeSize << "] = " << heightmap[shapeSize + square * shapeSize][ shapeSize] << std::endl;
                squareValueAverage /= 4;
                
                //Assigns midpoint and adds a std::random variance that is reduced every iteration
                std::cout << "average: " << squareValueAverage << std::endl;
                heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] = squareValueAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);   
                std::cout << "with rand: " <<  heightmap[square * shapeSize + (shapeSize / 2)][shapeSize / 2] << std::endl << std::endl;
                //Stores the midpoint position
                squareMidpoints[square] = sf::Vector2i(square * shapeSize + (shapeSize / 2), shapeSize / 2);
            }
            else
            {
                //Increases the modNumber every time the final square on the x axis is evaluated.
                if(square % iteration == 0)
                    modNumber += iteration;
                
                //Adds the values of the four corners to the average, then divides
                squareValueAverage += heightmap[(square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize]; //Top-left
                std::cout << "[" << (square % modNumber) * shapeSize << "][" << (int)floor(square / iteration) * shapeSize << "] = " << heightmap[(square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize] << std::endl;
                squareValueAverage += heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize]; //Top-right
                std::cout << "[" << shapeSize + (square % modNumber) * shapeSize << "][" << (int)floor(square / iteration) * shapeSize << "] = " << heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize] << std::endl;
                squareValueAverage += heightmap[(square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize + shapeSize]; //Bottom-left
                std::cout << "[" << (square % modNumber) * shapeSize << "][" << (int)floor(square / iteration) * shapeSize + shapeSize << "] = " << heightmap[(square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize + shapeSize] << std::endl;
                squareValueAverage += heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize + shapeSize]; //Bottom-right
                std::cout << "[" << shapeSize + (square % modNumber) * shapeSize << "][" << (int)floor(square / iteration) * shapeSize + shapeSize << "] = " << heightmap[shapeSize + (square % modNumber) * shapeSize][(int)floor(square / iteration) * shapeSize + shapeSize] << std::endl;
                squareValueAverage /= 4;       
                
                std::cout << "average: " << squareValueAverage << std::endl;

                //Assigns midpoint and adds a std::random variance that is reduced every iteration
                heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / iteration) * shapeSize + shapeSize / 2] = squareValueAverage  + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
                std::cout << "with rand: " <<  heightmap[(square % modNumber) * shapeSize + (shapeSize / 2)][(int)floor(square / iteration) * shapeSize + shapeSize / 2] << std::endl << std::endl;

                //Stores midpoint position
                squareMidpoints[square] = sf::Vector2i((square % modNumber) * shapeSize + (shapeSize / 2), (int)floor(square / iteration) * shapeSize + shapeSize / 2);
            }             
        }
        //-----------------------------SQUARE STEP-----------------------------
        //four diamonds for every square midpoint
        std::cout << "-----SQUARE STEP-----" << std::endl;
        for(int midpoint = 0; midpoint < squareMidpoints.size(); ++midpoint)
        {
            
            std::cout << "Triangle: " << midpoint << "Iteration: " << iteration << std::endl; 
            //---------------Left(square midpoint is right point)---------------
            
            float leftPointAverage = 0;
            
            std::cout << "Left:" << std::endl;
            //if the left point is out of array bounds
            if(squareMidpoints[midpoint].x - shapeSize < 0)
            {
                //Gets the average of four points
                leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                leftPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of leftmost point
                leftPointAverage /= 4;   
                
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x<< "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << " x2" << std::endl;
            }
            else
            {
                 //Gets the average of four points
                leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                leftPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
                leftPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize][squareMidpoints[midpoint].y];
                leftPointAverage /= 4;   
                
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize][squareMidpoints[midpoint].y] << std::endl;
            }
                        
            //Assigns the average +- std::random to the midpoint of the diamond
            heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] = leftPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            std::cout << "Average: " << leftPointAverage << std::endl;
            std::cout << "With rand: " <<  heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y] << std::endl << std::endl;
            
            
            //---------------Right(square midpoint is left point)---------------
            
            float rightPointAverage = 0;
            
            std::cout << "Right:" << std::endl;
            //if the right point is out of array bounds
            if(squareMidpoints[midpoint].x + shapeSize > width - 1)
            {
                //Gets the average of four points
                rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                rightPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of rightmost point
                rightPointAverage /= 4; 
                
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x<< "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << " x2" << std::endl;
            }
            else
            {
                 //Gets the average of four points
                rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                rightPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize][squareMidpoints[midpoint].y];
                rightPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
                rightPointAverage /= 4;   
                
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize][squareMidpoints[midpoint].y] << std::endl;
            }
            
            //Assigns the average +- std::random to the midpoint of the diamond
            heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] = rightPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            std::cout << "Average: " << rightPointAverage << std::endl;
            std::cout << "With rand: " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y] << std::endl << std::endl;
            //---------------Top(square midpoint is bottom point)---------------
            
            float topPointAverage = 0;
            
            std::cout << "Top:" << std::endl;
            //if the top point is out of array bounds
            if(squareMidpoints[midpoint].y - shapeSize < 0)
            {
                //Gets the average of four points
                topPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                topPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                topPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of topmost point
                topPointAverage /= 4;          
                
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << " x2" << std::endl;
            }
            else
            {
                 //Gets the average of four points
                topPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                topPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2];
                topPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize];
                topPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
                topPointAverage /= 4;   
                
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y - shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y - shapeSize << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize] << std::endl;
            }
            
            //Assigns the average +- std::random to the midpoint of the diamond
            heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] = topPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            std::cout << "Average: " << topPointAverage << std::endl;
            std::cout << "With rand: " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y - shapeSize / 2] << std::endl << std::endl;
            //---------------Bottom(square midpoint is top point)---------------
            
            float bottomPointAverage = 0;
            
            std::cout << "Bottom:" << std::endl;
            //if the bottom point is out of array bounds
            if(squareMidpoints[midpoint].y + shapeSize > height - 1)
            {
                //Gets the average of four points
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] * 2; // * 2 to compensate for lack of bottommost point
                bottomPointAverage /= 4;  
                
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << " x2" << std::endl;
            }
            else
            {
                 //Gets the average of four points
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2];
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize];
                bottomPointAverage += heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y];
                bottomPointAverage /= 4;   
                
                std::cout << "[" << squareMidpoints[midpoint].x + shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x + shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x - shapeSize / 2 << "][" << squareMidpoints[midpoint].y + shapeSize / 2 << "] = " << heightmap[squareMidpoints[midpoint].x - shapeSize / 2][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y] << std::endl;
                std::cout << "[" << squareMidpoints[midpoint].x << "][" << squareMidpoints[midpoint].y + shapeSize << "] = " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize] << std::endl;
            }
            
            //Assigns the average +- std::random to the midpoint of the diamond
            heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] = bottomPointAverage + (float)(std::rand() % 100 - 50) / 50.0f * pow(0.5, iteration);
            std::cout << "Average: " << bottomPointAverage << std::endl;
            std::cout << "With rand: " << heightmap[squareMidpoints[midpoint].x][squareMidpoints[midpoint].y + shapeSize / 2] << std::endl << std::endl;
        }  
    }
    

    for(int i = 0; i < width; ++i)
    {
        std::cout << "[ ";
        for(int j = 0; j < height; ++j)
            std::cout << round(heightmap[i][j]) << " ";
        std::cout << "]" << std::endl;    
    }
       
}