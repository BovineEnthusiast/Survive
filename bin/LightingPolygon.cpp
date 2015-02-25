#include "LightingPolygon.h"
#include <iostream>
#include <array>
#include <math.h>

LightingPolygon::LightingPolygon(GameObject* pParentObject)
    :pParentObject_(pParentObject)
{
}

//Getters
std::vector<sf::ConvexShape> LightingPolygon::getTriangles() const { return vTriangles_; }

//Pushers
void LightingPolygon::pushSprite(const sf::Sprite& sprite) { vSprites_.push_back(sprite); }
//Helpers
void LightingPolygon::clearSprites() { vSprites_.clear(); }
void LightingPolygon::createPolygon()
{
	//Clears old data
	sRays_.clear();
	vTrianglePoints_.clear();
	vTriangles_.clear();
	//renderTexture_.clear();

    //The origin of the light
    sf::Vector2f rayStart = pParentObject_->getPositionGlobal();
    
    //Inserts rays into the set and points them towards the verticies
	for (auto& sprite : vSprites_)
	{
		//Gets the unit vectors that point forward and right. Used to get vertex positions
		float rotation = sprite.getRotation() * 3.14159265358f / 180.0f;
		sf::Vector2f forwardVector(cos(rotation), sin(rotation));
		sf::Vector2f rightVector(cos(rotation + 90), sin(rotation + 90));

		//Gets the magnitude for the vectors.
		float vectorMagnitude = sprite.getLocalBounds().height / 2;

		//Position of the sprite at the center origin
		sf::Vector2f spritePosition = sprite.getPosition();

		//Assigns the rays. Goes CW after top-left
		sRays_.insert(Ray(spritePosition + forwardVector - rightVector, rayStart));
		sRays_.insert(Ray(spritePosition + forwardVector + rightVector, rayStart));
		sRays_.insert(Ray(spritePosition - forwardVector + rightVector, rayStart));
		sRays_.insert(Ray(spritePosition - forwardVector - rightVector, rayStart));
	}

	int numPoint = 0;

	//Adds rays that point to the four corners of the map
	sRays_.insert(Ray(-rayStart, rayStart)); //Top left
	sRays_.insert(Ray(sf::Vector2f(8160, 0) - rayStart, rayStart)); //Top right
	sRays_.insert(Ray(sf::Vector2f(8160, 8160) - rayStart, rayStart)); //Bottom right
	sRays_.insert(Ray(sf::Vector2f(0, 8160) - rayStart, rayStart)); //Bottom left

	//Adds slightly offset rays to light up past the corner of a "wall"
	std::set<Ray> tempSet = sRays_;
	for (auto& point : tempSet)
	{
		sRays_.insert(Ray(rayStart, point.getAngle() + 0.0001f));
		sRays_.insert(Ray(rayStart, point.getAngle() - 0.0001f));
	}

	std::cout << "Ray count: " << sRays_.size() << std::endl;
	//Calculates all the triangle points
	for (auto& ray : sRays_)
	{
		//Gets the ray's direction
		sf::Vector2f rayDirection = ray.getDirection();

		//Keep track of the smallest intersection
		sf::Vector2f smallestIntersectionVector;
		float smallestRayScalar = std::numeric_limits<float>::max();

		for (auto& sprite : vSprites_)
		{
			std::array<sf::Vector2f, 4> aVerticies;

			//Gets the unit vectors that point forward and right. Used to get vertex positions
			float rotation = sprite.getRotation() * 3.14159265358f / 180.0f;
			sf::Vector2f forwardVector(cos(rotation), sin(rotation));
			sf::Vector2f rightVector(cos(rotation + 90), sin(rotation + 90));

			//Gets the magnitude for the vectors.
			float vectorMagnitude = sprite.getLocalBounds().height / 2;

			//Position of the sprite at the center origin
			sf::Vector2f spritePosition = sprite.getPosition();

			aVerticies[0] = spritePosition + forwardVector - rightVector;
			aVerticies[1] = spritePosition + forwardVector + rightVector;
			aVerticies[2] = spritePosition - forwardVector + rightVector;
			aVerticies[3] = spritePosition - forwardVector - rightVector;

			//For those four verticies
			for (int i = 0; i < 4; ++i)
			{
				sf::Vector2f lineStart;
				sf::Vector2f lineDirection;

				if (i != 3)
				{
					lineStart = aVerticies[i];

					//Gets the direction of the line(to the other point)
					lineDirection = aVerticies[i + 1] - lineStart;
				}
				else
				{
					lineStart = aVerticies[3];

					//Gets the direction of the line(to the other point)
					lineDirection = aVerticies[0] - lineStart;
				}

				//The two scalars for the parametric form of the ray/line equations
				float lineScalar = (rayDirection.x * (lineStart.y - rayStart.y) + rayDirection.y * (rayStart.x - lineStart.x)) / (lineDirection.x * rayDirection.y - lineDirection.y * rayDirection.x);
				float rayScalar = (lineStart.x + lineDirection.x * lineScalar - rayStart.x) / rayDirection.x;

				//Make sure the intersection exists
				if (rayScalar <= 0 || (lineScalar <= 0 || lineScalar >= 1))
					continue;

				if (rayScalar < smallestRayScalar)
				{
					smallestRayScalar = rayScalar;
					smallestIntersectionVector = sf::Vector2f(rayStart.x + rayDirection.x * rayScalar, rayStart.y + rayDirection.y * rayScalar);
				}

			}
		}
		++numPoint;
		vTrianglePoints_.push_back(smallestIntersectionVector);

		//If there are two points(plus the light position)
		if (numPoint >= 2)
		{
			sf::Vector2f currentPoint = vTrianglePoints_.back();
			sf::Vector2f lastPoint = vTrianglePoints_.at(vTrianglePoints_.size() - 1);

			sf::ConvexShape triangle;
			triangle.setFillColor(sf::Color::Black);
			triangle.setPointCount(3);

			triangle.setPoint(0, lastPoint);
			triangle.setPoint(1, rayStart);
			triangle.setPoint(2, currentPoint);
			std::cout << "pointPos1: " << lastPoint.x << ", " << lastPoint.y << std::endl;
			std::cout << "pointPos2: " << currentPoint.x << ", " << currentPoint.y << std::endl;
			vTriangles_.push_back(triangle);
		}
		//renderTexture_.display();
	}

	//Draw all those triangles onto the render texture
	//for (auto& triangle : vTriangles_)
	//{
	//	renderTexture_.draw(triangle);
	//}
}
