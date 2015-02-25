#ifndef LIGHTINGPOLYGON_H
#define LIGHTINGPOLYGON_H
#include <set>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Ray.h"

class LightingPolygon
{
public:
	LightingPolygon(GameObject*);

	//Getters
	std::vector<sf::ConvexShape> getTriangles() const;

	//Pushers
	void pushSprite(const sf::Sprite&);

	//Helpers
	void clearSprites();
	void createPolygon();
private:
	//Used to get center position of the light
	GameObject* pParentObject_ = nullptr;

	//Used to store all the sprites that are used for lighting
	std::vector<sf::Sprite> vSprites_;

	//The rays to check
	std::set<Ray> sRays_;

	//The final (intersected) points to draw triangles with
	std::vector<sf::Vector2f> vTrianglePoints_;
	
	//Used to store all the triangles that make up the 
	std::vector<sf::ConvexShape> vTriangles_;

	//Used to draw all those rectangles
	//sf::RenderTexture renderTexture_;

};

#endif
