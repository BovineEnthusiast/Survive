#ifndef TILE_H
#define	TILE_H
#include <SFML/Graphics.hpp>
class Tile 
{
public:
    Tile();
    Tile(sf::Texture*, const sf::IntRect&, const std::string&);

    //Getters
    sf::Sprite getSprite();
    std::string getType();
    bool hasItem() const;
    
    //Setters
    void setSpritePos(const sf::Vector2f&);
	void setType(const std::string&);
    void setHasItem(bool);
    void setTileTexture();
    void setTileTextureRect();
private:
    sf::Texture* pTexture_;
    sf::Sprite sprite_;
    std::string type_ = "";
    bool hasItem_ = false;
};

#endif

