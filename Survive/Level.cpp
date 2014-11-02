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
    //A heightmap array that will end up being between -1 to 1
    //Initialized to 10 to find "empty" areas
    float heightmap[width][height] = {10};
    
    //Initial fill of four corners -1 to 0.99
    float initialCornerValue = (rand() % 100 - 50) / 50f;
    
    
    //Used to finalize the array to be -1 to 1
    float highestValue = initialCornerValue;
    float lowestValue = initialCornerValue;
    
    //The amount to multipy the displacement iteration
    float displacementMultiplier = 0.5f;
    
    for(int iteration = 1; heightmap[width + 1][height] == 10; ++iteration)
    {
        //-----------------------------Diamond step-----------------------------
        int squareSize =  (width  -1) / iteration;
        
        //Used in order to reset x pos after reaching last square
        int modNumber = 0;
        for(int square = 0; square < iteration * iteration; ++square)
        {
            float squareValueAverage = 0;
            
            //First row of squares
            if(square < iteration)
            {
                //Adds the values of the four corners to the average, then divides
                squareValueAverage += heightmap[square * squareSize][0]; //Top-left
                squareValueAverage += heightmap[square + square * squareSize][0]; //Top-right
                squareValueAverage += heightmap[square * squareSize][squareSize]; //Bottom-left
                squareValueAverage += heightmap[square + square * squareSize][squareSize]; //Bottom-right
                squareValueAverage /= 4;
                
                //Assigns midpoint and adds a random variance that is reduced every iteration
                heightmap[square * squareSize - (square / 2)][squareSize / 2] = squareValueAverage + (rand() % 100 - 50 / 50) * pow(0.5, iteration);
                
                
            }
            else
            {
                //Increases the modNumber every time the final square on the x axis is evaluated.
                if(square % iteration == 0)
                    modNumber += iteration;
                
                //Adds the values of the four corners to the average, then divides
                squareValueAverage += heightmap[(square % modNumber) * squareSize][floor(square / iteration) * squareSize]; //Top-left
                squareValueAverage += heightmap[square + (square % modNumber) * squareSize][floor(square / iteration) * squareSize]; //Top-right
                squareValueAverage += heightmap[(square % modNumber) * squareSize][floor(square / iteration) * squareSize + squareSize]; //Bottom-left
                squareValueAverage += heightmap[square + (square % modNumber) * squareSize][floor(square / iteration) * squareSize + squareSize]; //Top-left
                squareValueAverage /= 4;       
                
                //Assigns midpoint and adds a random variance that is reduced every iteration
                heightmap[(square % modNumber) * squareSize - (square / 2)][floor(square / iteration) * squareSize + squareSize / 2] = squareValueAverage  + (rand() % 100 - 50 / 50) * pow(0.5, iteration);
            }
            
        //-----------------------------Square step-----------------------------
            
            

        }
    }
    //Diamond step
    
    
}