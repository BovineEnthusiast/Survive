#include "SoundManager.h"
#include <iostream>
#include <algorithm>
SoundManager::SoundManager() 
{
    //----------------Loads all of the sounds--------------
    //Pistol
    if(!bufferPistolOne_.loadFromFile("sounds/gun/pistol/pistol_1.ogg"))
        std::cout << "Failed to load: sounds/gun/pistol/pistol_1.ogg" << std::endl;
    
    if(!bufferPistolTwo_.loadFromFile("sounds/gun/pistol/pistol_2.ogg"))
        std::cout << "Failed to load: sounds/gun/pistol/pistol_2.ogg" << std::endl;

    if(!bufferPistolThree_.loadFromFile("sounds/gun/pistol/pistol_3.ogg"))
        std::cout << "Failed to load: sounds/gun/pistol/pistol_3.ogg" << std::endl;

    //Rifle
    if(!bufferRifleOne_.loadFromFile("sounds/gun/rifle/rifle_1.ogg"))
        std::cout << "Failed to load: sounds/gun/rifle/rifle_1.ogg" << std::endl;

    if(!bufferRifleTwo_.loadFromFile("sounds/gun/rifle/rifle_2.ogg"))
        std::cout << "Failed to load: sounds/gun/rifle/rifle_2.ogg" << std::endl;
    
    if(!bufferRifleThree_.loadFromFile("sounds/gun/rifle/rifle_3.ogg"))
        std::cout << "Failed to load: sounds/gun/rifle/rifle_3.ogg" << std::endl;

    if(!bufferRifleFour_.loadFromFile("sounds/gun/rifle/rifle_4.ogg"))
        std::cout << "Failed to load: sounds/gun/rifle/rifle_4.ogg" << std::endl;

    
    //Grass
    if(!bufferGrassOne_.loadFromFile("sounds/footstep/grass/grass_1.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_1.ogg" << std::endl;

    if(!bufferGrassTwo_.loadFromFile("sounds/footstep/grass/grass_2.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_2.ogg" << std::endl;

    if(!bufferGrassThree_.loadFromFile("sounds/footstep/grass/grass_3.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_3.ogg" << std::endl;

    if(!bufferGrassFour_.loadFromFile("sounds/footstep/grass/grass_4.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_4.ogg" << std::endl;

    if(!bufferGrassFive_.loadFromFile("sounds/footstep/grass/grass_5.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_5.ogg" << std::endl;

    if(!bufferGrassSix_.loadFromFile("sounds/footstep/grass/grass_6.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_6.ogg" << std::endl;

    if(!bufferGrassSeven_.loadFromFile("sounds/footstep/grass/grass_7.ogg"))
        std::cout << "Failed to load: sounds/footstep/grass/grass_7.ogg" << std::endl;
    
    
    //Hits
    if(!bufferCrunch_.loadFromFile("sounds/hit/crunch.ogg"))
        std::cout << "Failed to load: sounds/hit/crunch.ogg" << std::endl;
    
    if(!bufferCrunchySplat_.loadFromFile("sounds/hit/crunchy_splat.ogg"))
        std::cout << "Failed to load: sounds/hit/crunchy_splat.ogg" << std::endl;
    
    //--------------Music--------------
    if(!musicBackgroundNature_.openFromFile("sounds/background/nature.ogg"))
        std::cout << "Failed to load: sounds/background/nature.ogg" << std::endl;
    
    musicBackgroundNature_.setLoop(true);
    musicBackgroundNature_.play();


}
void SoundManager::update(const sf::Time& dT)
{
    //auto iEraseAt = std::remove_if(lSounds_.begin(), lSounds_.end(), [](const sf::Sound& sound) {return sound.getStatus() == sf::Sound::Stopped;}); 
    //lSounds_.erase(iEraseAt, lSounds_.end());
    for(auto iSound = lSounds_.begin(); iSound != lSounds_.end();)
    {
        if(iSound->getStatus() == sf::Sound::Stopped)
            iSound = lSounds_.erase(iSound);
        else
            ++iSound;
    }
    std::cout << lSounds_.size() << std::endl ;
    //std::cout << lSounds_.back().getStatus() << std::endl;;

    
}
void SoundManager::playSound(const std::string& type)
{
    if(type == "pistol")
    {
        sf::Sound sound;
        int random = std::rand() % 3;
        if(random == 0)
            sound = sf::Sound(bufferPistolOne_);
        else if(random == 1)
            sound = sf::Sound(bufferPistolTwo_);
        else
            sound = sf::Sound(bufferPistolThree_);
        
        lSounds_.push_back(sound);
        lSounds_.back().play();
    }
    else if(type == "rifle")
    {
        sf::Sound sound;
        int random = std::rand() % 4;
        if(random == 0)
            sound = sf::Sound(bufferRifleOne_);
        else if(random == 1)
            sound = sf::Sound(bufferRifleTwo_);
        else if(random == 2)
            sound = sf::Sound(bufferRifleThree_);
        else
            sound = sf::Sound(bufferRifleFour_);
        
        lSounds_.push_back(sound);
        lSounds_.back().play();
    }
    else if(type == "grass")
    {

        sf::Sound sound;
        int random = std::rand() % 7;
        if(random == 0)
            sound = sf::Sound(bufferGrassOne_);
        else if(random == 1)
            sound = sf::Sound(bufferGrassTwo_);
        else if(random == 2)
            sound = sf::Sound(bufferGrassThree_);
        else if(random == 3)
            sound = sf::Sound(bufferGrassFour_);
        else if(random == 4)
            sound = sf::Sound(bufferGrassFive_);
        else if(random == 5)
            sound = sf::Sound(bufferGrassSix_);
        else
            sound = sf::Sound(bufferGrassSeven_);
   
        
        lSounds_.push_back(sound);
        lSounds_.back().play();
    }
    else if(type == "hit")
    {
   
        sf::Sound sound = sf::Sound(bufferCrunch_);
        lSounds_.push_back(sound);
        lSounds_.back().play();
    }
    else if(type == "zombie_death")
    {
        sf::Sound sound = sf::Sound(bufferCrunchySplat_);
        lSounds_.push_back(sound);
        lSounds_.back().play();
    }
}
