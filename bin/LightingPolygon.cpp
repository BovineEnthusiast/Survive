#include "LightingPolygon.h"
#include <iostream>
#include <array>
#include <math.h>

LightingPolygon::LightingPolygon()

{
}

//Getters
std::vector<sf::ConvexShape> LightingPolygon::getTriangles() const { return vTriangles_; }

//Setters
void LightingPolygon::setPosition(const sf::Vector2f& position) { rayStart_ = position; }
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

	//std::cout << "Richard is a kant: " << rayStart_.x << ", " << rayStart_.y << std::endl;
	//Inserts rays into the set and points them towards the verticies
	for (auto& sprite : vSprites_)
	{
		//Gets the unit vectors that point forward and right. Used to get vertex positions
		float rotation = sprite.getRotation() * 3.14159265358f / 180.0f;
		sf::Vector2f forwardVector(cos(rotation), sin(rotation));
		sf::Vector2f rightVector(cos(rotation + (3.14159265358f / 2)), sin(rotation + (3.14159265358f / 2)));

		//Gets the magnitude for the vectors.
		float vectorMagnitude = sprite.getGlobalBounds().height / 2;
		forwardVector *= vectorMagnitude;
		rightVector *= vectorMagnitude;
		//Position of the sprite at the center origin
		sf::Vector2f spritePosition = sprite.getPosition();

		//Assigns the rays. Goes CW after top-left
		sRays_.insert(Ray((spritePosition + forwardVector - rightVector) - rayStart_, rayStart_));
		sRays_.insert(Ray((spritePosition + forwardVector + rightVector) - rayStart_, rayStart_));
		sRays_.insert(Ray((spritePosition - forwardVector + rightVector) - rayStart_, rayStart_));
		sRays_.insert(Ray((spritePosition - forwardVector - rightVector) - rayStart_, rayStart_));
	}

	int numPoint = 0;

	//Adds rays that point to the four corners of the map
	sRays_.insert(Ray(sf::Vector2f(0.0f, 0.0f) - rayStart_, rayStart_)); //Top left
	sRays_.insert(Ray(sf::Vector2f(8160.0f, 0.0f) - rayStart_, rayStart_)); //Top right
	sRays_.insert(Ray(sf::Vector2f(8160.0f, 8160.0f) - rayStart_, rayStart_)); //Bottom right
	sRays_.insert(Ray(sf::Vector2f(0.0f, 8160.0f) - rayStart_, rayStart_)); //Bottom left

	//Adds slightly offset rays to light up past the corner of a "wall"
	std::set<Ray> tempSet = sRays_;
	for (auto& point : tempSet)
	{
		sRays_.insert(Ray(rayStart_, point.getAngle() + 0.00001f));
		sRays_.insert(Ray(rayStart_, point.getAngle() - 0.00001f));
	}

	//Calculates all the triangle points
	for (auto& ray : sRays_)
	{
		//Gets the ray's direction
		sf::Vector2f rayDirection = ray.getDirection();
		//Keep track of the smallest intersection
		sf::Vector2f smallestIntersectionVector;
		float smallestRayScalar = std::numeric_limits<float>::max();
		float lineScalarTest;

		//Test against world boundries
		std::array<sf::Vector2f, 4> worldVerticies;

		worldVerticies[0] = sf::Vector2f(0.0f, 0.0f);
		worldVerticies[1] = sf::Vector2f(8160.0f, 0.0f);
		worldVerticies[2] = sf::Vector2f(8160.0f, 8160.0f);
		worldVerticies[3] = sf::Vector2f(0.0f, 8160.0f);

		//For those four verticies
		for (int i = 0; i < 4; ++i)
		{
			sf::Vector2f lineStart;
			sf::Vector2f lineDirection;

			if (i != 3)
			{
				lineStart = worldVerticies[i];

				//Gets the direction of the line(to the other point)
				lineDirection = worldVerticies[i + 1] - lineStart;
			}
			else
			{
				lineStart = worldVerticies[3];

				//Gets the direction of the line(to the other point)
				lineDirection = worldVerticies[0] - lineStart;
			}

			//The two scalars for the parametric form of the ray/line equations
			float lineScalar = (rayDirection.x * (lineStart.y - rayStart_.y) + rayDirection.y * (rayStart_.x - lineStart.x)) / (lineDirection.x * rayDirection.y - lineDirection.y * rayDirection.x);
			float rayScalar = (lineStart.x + lineDirection.x * lineScalar - rayStart_.x) / rayDirection.x;

			


			if (rayScalar < smallestRayScalar && rayScalar > 0 && lineScalar >= 0 && lineScalar <= 1)
			{
				smallestRayScalar = rayScalar;
				lineScalarTest = lineScalar;
				smallestIntersectionVector = sf::Vector2f(rayStart_.x + rayDirection.x * rayScalar, rayStart_.y + rayDirection.y * rayScalar);
			}

		}
		//Test against sprites
		for (auto& sprite : vSprites_)
		{
			std::array<sf::Vector2f, 4> aVerticies;

			//Gets the unit vectors that point forward and right. Used to get vertex positions
			float rotation = sprite.getRotation() * 3.14159265358f / 180.0f;
			sf::Vector2f forwardVector(cos(rotation), sin(rotation));
			sf::Vector2f rightVector(cos(rotation + (3.14159265358f / 2)), sin(rotation + (3.14159265358f / 2)));

			//Gets the magnitude for the vectors.
			float vectorMagnitude = sprite.getGlobalBounds().height / 2;
			forwardVector *= vectorMagnitude;
			rightVector *= vectorMagnitude;

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
				float lineScalar = (rayDirection.x * (lineStart.y - rayStart_.y) + rayDirection.y * (rayStart_.x - lineStart.x)) / (lineDirection.x * rayDirection.y - lineDirection.y * rayDirection.x);
				float rayScalar = (lineStart.x + lineDirection.x * lineScalar - rayStart_.x) / rayDirection.x;



				if (rayScalar < smallestRayScalar && rayScalar > 0 && lineScalar >= 0 && lineScalar <= 1)
				{
					smallestRayScalar = rayScalar;
					lineScalarTest = lineScalar;
					smallestIntersectionVector = sf::Vector2f(rayStart_.x + rayDirection.x * rayScalar, rayStart_.y + rayDirection.y * rayScalar);
				}

			}
		}
		//if (smallestRayScalar != std::numeric_limits<float>::max())
		//{
		float angleTest = atan2(smallestIntersectionVector.x - rayStart_.x, smallestIntersectionVector.y - rayStart_.y) * 180 / 3.14159265358f;
		
			++numPoint;
			vTrianglePoints_.push_back(smallestIntersectionVector);
		

		//If there are two points(plus the light position)
		if (numPoint >= 2)
		{
			sf::Vector2f currentPoint = vTrianglePoints_.back();
			sf::Vector2f lastPoint = vTrianglePoints_.at(vTrianglePoints_.size() - 2);

			sf::ConvexShape triangle;
			triangle.setFillColor(sf::Color::Black);
			triangle.setPointCount(3);

			triangle.setPoint(0, lastPoint);
			triangle.setPoint(1, rayStart_);
			triangle.setPoint(2, currentPoint);
			vTriangles_.push_back(triangle);
		}
		//}



		//renderTexture_.display();
	}
	//Adds the last triangle
	sf::ConvexShape triangle;
	triangle.setFillColor(sf::Color::Black);
	triangle.setPointCount(3);

	triangle.setPoint(0, vTrianglePoints_.back());
	triangle.setPoint(1, rayStart_);
	triangle.setPoint(2, vTrianglePoints_.front());

	vTriangles_.push_back(triangle);

	//Draw all those triangles onto the render texture
	//for (auto& triangle : vTriangles_)
	//{
	//	renderTexture_.draw(triangle);
	//}
}
