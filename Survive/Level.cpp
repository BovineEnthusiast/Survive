#include "Level.h"
#include <SFML/System.hpp>
Level::Level() 
{
}

void Level::generateLevel(const int width, const int height) //Generates the level based on perlin noise with size width and height
{
    //Stores the random vectors that fill the grid.
    //+1 because of "fence post problem" 
    //perlin value = fence | rand vector = post
    sf::Vector2i perlinVectorGrid[width + 1][height + 1]; 
    
    //Stores the gradient values calculated from the dot-product of
    //the random vectors and the vectors of a random point in a cell then LERP'd
    float perlinGradientGrid[width][height]; 
    
    //Initializes the grid with random vectors(0 to 99)
    for(size_t widthPos = 0; widthPos < width + 1; ++widthPos)
    {
        for(size_t heightPos = 0; heightPos < height + 1; ++heightPos)
        {
            perlinVectorGrid[widthPos][heightPos] = sf::Vector2i(std::rand() % 100 - 50, std::rand() % 100 - 50);
        }
    }
    
    //Assigns gradient values
     for(size_t widthPos = 0; widthPos < width; ++widthPos)
    {
        for(size_t heightPos = 0; heightPos < height; ++heightPos)
        {
            //Randomized point in a "cell" with corners containing vectors
            //[widthPos][heightPos]  [widthPos + 1][heightPos]
            //
            //      <rand, rand> <=(random point)
            //
            //[widthPos][heightPos + 1] [widthPos + 1][heightPos + 1]
            sf::Vector2i point(std::rand() % 100, std::rand() % 100);
            
            //Stores vectors that are between the point and corners
            sf::Vector2i gradientTLVec = sf::Vector2i(0,0) - point;
            sf::Vector2i gradientTRVec = sf::Vector2i(100, 0) - point;
            sf::Vector2i gradientBLVec = sf::Vector2i(0, 100) - point;
            sf::Vector2i gradientBRVec = sf::Vector2i(100, 100) - point;
            
            //Calculates the gradient values by getting the dot products of the
            //vectors associated with each corner 
            float gradientTL = perlinVectorGrid[widthPos][heightPos].x * gradientTLVec.x + perlinVectorGrid[widthPos][heightPos].y * gradientTLVec.y;
            float gradientTR = perlinVectorGrid[widthPos + 1][heightPos].x * gradientTRVec.x + perlinVectorGrid[widthPos + 1][heightPos].y * gradientTRVec.y;
            float gradientBL = perlinVectorGrid[widthPos][heightPos + 1].x * gradientBLVec.x + perlinVectorGrid[widthPos][heightPos + 1].y * gradientBLVec.y;
            float gradientBR = perlinVectorGrid[widthPos + 1][heightPos + 1].x * gradientBRVec.x + perlinVectorGrid[widthPos + 1][heightPos + 1].y * gradientBRVec.y;
            

            //Bi-linearly interpolates between the gradient values to get the
            //gradient(height) value at the point
            float topLerp = (gradientTL + gradientTR) * ((float)point.x / 100.0f);
            float bottomLerp = (gradientBL + gradientBR) * ((float)point.x / 100.0f);
            float finalLerp = (topLerp + bottomLerp) * ((float)point.y / 100.0f);
            
            //Set the final result(finalLerp) to the appropriate position
            perlinGradientGrid[width][height] = finalLerp;           
        }
    }
}

