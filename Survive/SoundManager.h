#ifndef SOUNDMANAGER_H
#define	SOUNDMANAGER_H
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <list>
class SoundManager 
{
public:
    SoundManager();
    void playSound(const std::string&);
    void update(const sf::Time&);
private:
    
    //------------------Buffers-----------------------
    //Pistol
    sf::SoundBuffer bufferPistolOne_;
    sf::SoundBuffer bufferPistolTwo_;
    sf::SoundBuffer bufferPistolThree_;
    
    //Rifle
    sf::SoundBuffer bufferRifleOne_;
    sf::SoundBuffer bufferRifleTwo_;
    sf::SoundBuffer bufferRifleThree_;
    sf::SoundBuffer bufferRifleFour_;
    
    //Grass footsteps
    sf::SoundBuffer bufferGrassOne_;
    sf::SoundBuffer bufferGrassTwo_;
    sf::SoundBuffer bufferGrassThree_;
    sf::SoundBuffer bufferGrassFour_;
    sf::SoundBuffer bufferGrassFive_;
    sf::SoundBuffer bufferGrassSix_;
    sf::SoundBuffer bufferGrassSeven_;
    
    //Hits
    sf::SoundBuffer bufferCrunch_;
    sf::SoundBuffer bufferCrunchySplat_;
    
	//UI
	sf::SoundBuffer bufferHover_;
	sf::SoundBuffer bufferClick_;

    std::list<sf::Sound> lSounds_;
    
    //---------------Music----------------------------
    sf::Music musicBackgroundNature_;    
};

#endif

