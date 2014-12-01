#include "Collision.h"
#include <math.h>
#include <iostream>

//Checks if two square sprites are colliding
bool isColliding(const sf::Sprite& squareOne, const sf::Sprite& squareTwo)
{
    sf::Vector2f axis[4];
    
    //Gets all the axis
    //Axis 1
    float squareOneRotation = squareOne.getRotation() * 3.14159265358f / 180.0f;
    axis[0] = sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation));
   
    //Axis 2
    float squareOneRotationPerp = squareOneRotation + 3.14159265358f / 2.0f;
    axis[1] = sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp));
     
    //Axis 3
    float squareTwoRotation = squareTwo.getRotation()  * 3.14159265358f / 180.0f;
    axis[2] = sf::Vector2f(cos(squareTwoRotation), sin(squareTwoRotation));
    
    //Axis 4 
    float squareTwoRotationPerp = squareTwoRotation + 3.14159265358f / 2.0f;
    axis[3] = sf::Vector2f(cos(squareTwoRotationPerp), sin(squareTwoRotationPerp));
    
    //Tests all of them
    for(size_t numAxis = 0; numAxis < 4; ++numAxis)
    {
        sf::Vector2f squareOneVertices[4];
        sf::Vector2f squareTwoVertices[4];
        //Gets the four vertices for both squares
        //Square 1
        squareOneVertices[0] = squareOne.getPosition() + sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        squareOneVertices[1] = squareOne.getPosition() - sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        squareOneVertices[2] = squareOne.getPosition() + sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        squareOneVertices[3] = squareOne.getPosition() - sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        
        //Square 2
        squareTwoVertices[0] = squareTwo.getPosition() + sf::Vector2f(cos(squareTwoRotation), sin(squareTwoRotation)) * (float)(squareTwo.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(squareTwoRotationPerp), sin(squareTwoRotationPerp)) * (float)(squareTwo.getLocalBounds().width / 2.0f);
        squareTwoVertices[1] = squareTwo.getPosition() - sf::Vector2f(cos(squareTwoRotation), sin(squareTwoRotation)) * (float)(squareTwo.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(squareTwoRotationPerp), sin(squareTwoRotationPerp)) * (float)(squareTwo.getLocalBounds().width / 2.0f);
        squareTwoVertices[2] = squareTwo.getPosition() + sf::Vector2f(cos(squareTwoRotation), sin(squareTwoRotation)) * (float)(squareTwo.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(squareTwoRotationPerp), sin(squareTwoRotationPerp)) * (float)(squareTwo.getLocalBounds().width / 2.0f);
        squareTwoVertices[3] = squareTwo.getPosition() - sf::Vector2f(cos(squareTwoRotation), sin(squareTwoRotation)) * (float)(squareTwo.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(squareTwoRotationPerp), sin(squareTwoRotationPerp)) * (float)(squareTwo.getLocalBounds().width / 2.0f);
        
        
        //Gets the projection for each square
        //Square 1
        float squareOneMax = squareOneVertices[0].x * axis[numAxis].x + squareOneVertices[0].y * axis[numAxis].y;
        float squareOneMin = squareOneMax;
        for(size_t vertice = 1; vertice < 4; ++vertice)
        {
            float dotProduct = squareOneVertices[vertice].x * axis[numAxis].x + squareOneVertices[vertice].y * axis[numAxis].y;
            if(dotProduct > squareOneMax)
                squareOneMax = dotProduct;
            else if(dotProduct < squareOneMin)
                squareOneMin = dotProduct;
        }
        
        //Square 2
        float squareTwoMax = squareTwoVertices[0].x * axis[numAxis].x + squareTwoVertices[0].y * axis[numAxis].y;
        float squareTwoMin = squareTwoMax;
        for(size_t vertice = 1; vertice < 4; ++vertice)
        {
            float dotProduct = squareTwoVertices[vertice].x * axis[numAxis].x + squareTwoVertices[vertice].y * axis[numAxis].y;
            if(dotProduct > squareTwoMax)
                squareTwoMax = dotProduct;
            else if(dotProduct < squareTwoMin)
                squareTwoMin = dotProduct;
        }
      
        //Checks if the two projections are overlapping
        if(squareOneMax <= squareTwoMax && squareOneMax >= squareTwoMin)
            continue;
        else if(squareOneMin <= squareTwoMax && squareOneMin >= squareTwoMin)
            continue;
        else if(squareTwoMax <= squareOneMax && squareTwoMax >= squareOneMin)
            continue;
        else if(squareTwoMin <= squareOneMax && squareTwoMin >= squareOneMin)
            continue;
        else
            return false;
        
    }
    return true;
}

//Checks to see if a bullet has collided
bool isColliding(const sf::Sprite& squareOne, const sf::RectangleShape& bulletNew, const sf::Vector2f& bulletOldPosition)
{
    sf::Vector2f axis[4];
    
    //Gets all the axis
    //Axis 1
    float squareOneRotation = squareOne.getRotation() * 3.14159265358f / 180.0f;
    axis[0] = sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation));
   
    //Axis 2
    float squareOneRotationPerp = squareOneRotation + 3.14159265358f / 2.0f;
    axis[1] = sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp));
     
    //Axis 3
    float bulletNewRotation = bulletNew.getRotation()  * 3.14159265358f / 180.0f;
    axis[2] = sf::Vector2f(cos(bulletNewRotation), sin(bulletNewRotation));
    
    //Axis 4 
    float bulletNewRotationPerp = bulletNewRotation + 3.14159265358f / 2.0f;
    axis[3] = sf::Vector2f(cos(bulletNewRotationPerp), sin(bulletNewRotationPerp));
    
    //Tests all of them
    for(size_t numAxis = 0; numAxis < 4; ++numAxis)
    {
        sf::Vector2f squareOneVertices[4];
        sf::Vector2f bulletNewVertices[4];
        //Gets the four vertices for both squares
        //Square 1
        squareOneVertices[0] = squareOne.getPosition() + sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        squareOneVertices[1] = squareOne.getPosition() - sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        squareOneVertices[2] = squareOne.getPosition() + sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        squareOneVertices[3] = squareOne.getPosition() - sf::Vector2f(cos(squareOneRotation), sin(squareOneRotation)) * (float)(squareOne.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(squareOneRotationPerp), sin(squareOneRotationPerp)) * (float)(squareOne.getLocalBounds().width / 2.0f);
        
        //Bullet, takes into account the previous bullet location
        bulletNewVertices[0] = bulletNew.getPosition() + sf::Vector2f(cos(bulletNewRotation), sin(bulletNewRotation)) * (float)(bulletNew.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(bulletNewRotationPerp), sin(bulletNewRotationPerp)) * (float)(bulletNew.getLocalBounds().width / 2.0f);
        bulletNewVertices[1] = bulletNew.getPosition() + sf::Vector2f(cos(bulletNewRotation), sin(bulletNewRotation)) * (float)(bulletNew.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(bulletNewRotationPerp), sin(bulletNewRotationPerp)) * (float)(bulletNew.getLocalBounds().width / 2.0f);
        bulletNewVertices[2] = bulletNew.getPosition() - sf::Vector2f(cos(bulletNewRotation), sin(bulletNewRotation)) * (float)(bulletNew.getLocalBounds().width / 2.0f) + sf::Vector2f(cos(bulletNewRotationPerp), sin(bulletNewRotationPerp)) * (float)(bulletNew.getLocalBounds().width / 2.0f);
        bulletNewVertices[3] = bulletNew.getPosition() - sf::Vector2f(cos(bulletNewRotation), sin(bulletNewRotation)) * (float)(bulletNew.getLocalBounds().width / 2.0f) - sf::Vector2f(cos(bulletNewRotationPerp), sin(bulletNewRotationPerp)) * (float)(bulletNew.getLocalBounds().width / 2.0f);
        
        
        //Gets the projection for each square
        //Square 1
        float squareOneMax = squareOneVertices[0].x * axis[numAxis].x + squareOneVertices[0].y * axis[numAxis].y;
        float squareOneMin = squareOneMax;
        for(size_t vertice = 1; vertice < 4; ++vertice)
        {
            float dotProduct = squareOneVertices[vertice].x * axis[numAxis].x + squareOneVertices[vertice].y * axis[numAxis].y;
            if(dotProduct > squareOneMax)
                squareOneMax = dotProduct;
            else if(dotProduct < squareOneMin)
                squareOneMin = dotProduct;
        }
        
        //Square 2
        float bulletNewMax = bulletNewVertices[0].x * axis[numAxis].x + bulletNewVertices[0].y * axis[numAxis].y;
        float bulletNewMin = bulletNewMax;
        for(size_t vertice = 1; vertice < 4; ++vertice)
        {
            float dotProduct = bulletNewVertices[vertice].x * axis[numAxis].x + bulletNewVertices[vertice].y * axis[numAxis].y;
            if(dotProduct > bulletNewMax)
                bulletNewMax = dotProduct;
            else if(dotProduct < bulletNewMin)
                bulletNewMin = dotProduct;
        }
      
        //Checks if the two projections are overlapping
        if(squareOneMax <= bulletNewMax && squareOneMax >= bulletNewMin)
            continue;
        else if(squareOneMin <= bulletNewMax && squareOneMin >= bulletNewMin)
            continue;
        else if(bulletNewMax <= squareOneMax && bulletNewMax >= squareOneMin)
            continue;
        else if(bulletNewMin <= squareOneMax && bulletNewMin >= squareOneMin)
            continue;
        else
            return false;
        
    }
    return true;
}



