#ifndef SOUNDMANAGER_H
#define	SOUNDMANAGER_H
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <list>
class SoundManager 
{
public:
    SoundManager();
    void playSound(const std::string&, const sf::Vector2f&, const sf::Vector2f&);
    void update(const sf::Time&);
private:
    
    //------------------Buffers-----------------------
    //Pistol
    sf::SoundBuffer bufferPistolSelect_;
    sf::SoundBuffer bufferPistolOne_;
    sf::SoundBuffer bufferPistolTwo_;
    sf::SoundBuffer bufferPistolThree_;

    //Magnum
    sf::SoundBuffer bufferMagnumSelect_;
    sf::SoundBuffer bufferMagnumOne_;

    //Shotgun
    sf::SoundBuffer bufferShotgunSelect_;
    sf::SoundBuffer bufferShotgunOne_;
    
    //Rifle
    sf::SoundBuffer bufferRifleSelect_;
    sf::SoundBuffer bufferRifleOne_;
    sf::SoundBuffer bufferRifleTwo_;
    sf::SoundBuffer bufferRifleThree_;
    
	//Explosion
	sf::SoundBuffer bufferExplosion_;
	sf::SoundBuffer bufferRocketLaunch_;

	//Zombie
	sf::SoundBuffer bufferZombieOne_;
	sf::SoundBuffer bufferZombieTwo_;
	sf::SoundBuffer bufferZombieThree_;
	sf::SoundBuffer bufferZombieFour_;
	sf::SoundBuffer bufferZombieFive_;

	//Ranged zombie gun
	sf::SoundBuffer bufferRangedZombie_;

    //Grass footsteps
    sf::SoundBuffer bufferGrassOne_;
    sf::SoundBuffer bufferGrassTwo_;
    sf::SoundBuffer bufferGrassThree_;
    sf::SoundBuffer bufferGrassFour_;
    sf::SoundBuffer bufferGrassFive_;
    sf::SoundBuffer bufferGrassSix_;
    sf::SoundBuffer bufferGrassSeven_;
    
    //Hits
    sf::SoundBuffer bufferHitOne_;
	sf::SoundBuffer bufferHitTwo_;
	sf::SoundBuffer bufferHitThree_;
	sf::SoundBuffer bufferHitFour_;
    sf::SoundBuffer bufferCrunchySplat_;
	
	//Punches
	sf::SoundBuffer bufferPunchOne_;
	sf::SoundBuffer bufferPunchTwo_;

	//Cracks
	sf::SoundBuffer bufferCrackOne_;
	sf::SoundBuffer bufferCrackTwo_;

	//Flesh
	sf::SoundBuffer bufferFleshOne_;
	sf::SoundBuffer bufferFleshTwo_;

	//UI
	sf::SoundBuffer bufferHover_;
	sf::SoundBuffer bufferClick_;
	sf::SoundBuffer bufferBuy_;

	//Misc
	sf::SoundBuffer bufferHealth_;
	sf::SoundBuffer bufferImpact_;

    std::list<sf::Sound> lSounds_;
    
    //---------------Music----------------------------
    sf::Music musicBackgroundNature_;    
};

#endif