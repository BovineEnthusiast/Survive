#ifndef BLOODSPLAT_H
#define	BLOODSPLAT_H
#include "GameObject.h"
class BloodSplat : public GameObject {
public:
    BloodSplat(sf::Texture*);
    void update(const sf::Time&);
    bool isFaded() const;
    
    //Getters
    sf::Sprite getSprite() const;
private:

    sf::Sprite bloodSplatSprite_;
    sf::Texture* pBloodSplatTexture_;
    sf::Color color_ = sf::Color(200 + std::rand() % 56, 0, 0, 150);
    //Fade properties
    bool faded_ = false;
    float fadeAfter_ = 10000;
    float fadeFor_ = 5000;
    sf::Clock lifeClock_;
    
};

#endif	

