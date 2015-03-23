#include "SoundManager.h"
#include <iostream>
#include <algorithm>
SoundManager::SoundManager()
{
	//----------------Loads all of the sounds--------------
	//Pistol
	if (!bufferPistolSelect_.loadFromFile("assets/sounds/gun/pistol/pistol_select.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/pistol/pistol_select.ogg" << std::endl;

	if (!bufferPistolOne_.loadFromFile("assets/sounds/gun/pistol/pistol_1.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/pistol/pistol_1.ogg" << std::endl;

	if (!bufferPistolTwo_.loadFromFile("assets/sounds/gun/pistol/pistol_2.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/pistol/pistol_2.ogg" << std::endl;

	if (!bufferPistolThree_.loadFromFile("assets/sounds/gun/pistol/pistol_3.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/pistol/pistol_3.ogg" << std::endl;

	//Magnum
	if (!bufferMagnumSelect_.loadFromFile("assets/sounds/gun/magnum/magnum_select.ogg"))
		std::cout << "Failed to load magnum select" << std::endl;
	if (!bufferMagnumOne_.loadFromFile("assets/sounds/gun/magnum/magnum_1.ogg"))
		std::cout << "Failed to load magnum 1" << std::endl;

	//Shotgun
	if (!bufferShotgunSelect_.loadFromFile("assets/sounds/gun/shotgun/shotgun_select.ogg"))
		std::cout << "Failed to load shotgun select" << std::endl;

	if (!bufferShotgunOne_.loadFromFile("assets/sounds/gun/shotgun/shotgun_1.ogg"))
		std::cout << "Failed to load shotgun 1" << std::endl;

	//Rifle
	if (!bufferRifleSelect_.loadFromFile("assets/sounds/gun/rifle/rifle_select.ogg"))
		std::cout << "Failed to load" << std::endl;

	if (!bufferRifleOne_.loadFromFile("assets/sounds/gun/rifle/rifle_1.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/rifle/rifle_1.ogg" << std::endl;

	if (!bufferRifleTwo_.loadFromFile("assets/sounds/gun/rifle/rifle_2.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/rifle/rifle_2.ogg" << std::endl;

	if (!bufferRifleThree_.loadFromFile("assets/sounds/gun/rifle/rifle_3.ogg"))
		std::cout << "Failed to load: sounds/gun/rifle/rifle_3.ogg" << std::endl;

	//Explosion
	if (!bufferExplosion_.loadFromFile("assets/sounds/gun/explosion/explosion.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/explosion/explosion.ogg" << std::endl;

	//Zombies

	if (!bufferZombieOne_.loadFromFile("assets/sounds/zombie/moan_1.ogg"))
		std::cout << "Failed to load: assets/sounds/zombie/moan_1.ogg" << std::endl;

	if (!bufferZombieTwo_.loadFromFile("assets/sounds/zombie/moan_2.ogg"))
		std::cout << "Failed to load: assets/sounds/zombie/moan_2.ogg" << std::endl;
	
	if (!bufferZombieThree_.loadFromFile("assets/sounds/zombie/moan_3.ogg"))
		std::cout << "Failed to load: assets/sounds/zombie/moan_3.ogg" << std::endl;
	
	if (!bufferZombieFour_.loadFromFile("assets/sounds/zombie/moan_4.ogg"))
		std::cout << "Failed to load: assets/sounds/zombie/moan_4.ogg" << std::endl;
	
	if (!bufferZombieFive_.loadFromFile("assets/sounds/zombie/moan_5.ogg"))
		std::cout << "Failed to load: assets/sounds/zombie/moan_5.ogg" << std::endl;

	//Ranged Zombie
	if (!bufferRangedZombie_.loadFromFile("assets/sounds/gun/zombie_ranged/zombie_ranged.ogg"))
		std::cout << "Failed to load: assets/sounds/gun/zombie_ranged/zombie_ranged.ogg" << std::endl;

	//Grass
	if (!bufferGrassOne_.loadFromFile("assets/sounds/footstep/grass/grass_1.ogg"))
		std::cout << "Failed to load: assets/sounds/footstep/grass/grass_1.ogg" << std::endl;

	if (!bufferGrassTwo_.loadFromFile("assets/sounds/footstep/grass/grass_2.ogg"))
		std::cout << "Failed to load: assets/sounds/footstep/grass/grass_2.ogg" << std::endl;

	if (!bufferGrassThree_.loadFromFile("assets/sounds/footstep/grass/grass_3.ogg"))
		std::cout << "Failed to load: assets/sounds/footstep/grass/grass_3.ogg" << std::endl;

	if (!bufferGrassFour_.loadFromFile("assets/sounds/footstep/grass/grass_4.ogg"))
		std::cout << "Failed to load: assets/sounds/footstep/grass/grass_4.ogg" << std::endl;

	if (!bufferGrassFive_.loadFromFile("assets/sounds/footstep/grass/grass_5.ogg"))
		std::cout << "Failed to load: assets/sounds/footstep/grass/grass_5.ogg" << std::endl;

	if (!bufferGrassSix_.loadFromFile("assets/sounds/footstep/grass/grass_6.ogg"))
		std::cout << "Failed to load: sounds/footstep/grass/grass_6.ogg" << std::endl;

	if (!bufferGrassSeven_.loadFromFile("assets/sounds/footstep/grass/grass_7.ogg"))
		std::cout << "Failed to load: assets/sounds/footstep/grass/grass_7.ogg" << std::endl;


	//Hits
	if (!bufferHitOne_.loadFromFile("assets/sounds/hit/hit_1.ogg"))
		std::cout << "Failed to load: assets/sounds/hit/hit_1.ogg" << std::endl;

	if (!bufferHitTwo_.loadFromFile("assets/sounds/hit/hit_2.ogg"))
		std::cout << "Failed to load: assets/sounds/hit/hit_2.ogg" << std::endl;

	if (!bufferHitThree_.loadFromFile("assets/sounds/hit/hit_3.ogg"))
		std::cout << "Failed to load: assets/sounds/hit/hit_3.ogg" << std::endl;

	if (!bufferHitFour_.loadFromFile("assets/sounds/hit/hit_4.ogg"))
		std::cout << "Failed to load: assets/sounds/hit/hit_4.ogg" << std::endl;

	if (!bufferCrunchySplat_.loadFromFile("assets/sounds/hit/crunchy_splat.ogg"))
		std::cout << "Failed to load: assets/sounds/hit/crunchy_splat.ogg" << std::endl;

	//UI
	if (!bufferHover_.loadFromFile("assets/sounds/ui/hover.wav"))
		std::cout << "Failed to load: assets/sounds/ui/hover.wav";
	if (!bufferClick_.loadFromFile("assets/sounds/ui/click.wav"))
		std::cout << "Failed to load: assets/sounds/ui/click.wav";

	//--------------Music--------------
	if (!musicBackgroundNature_.openFromFile("assets/sounds/background/nature.ogg"))
		std::cout << "Failed to load: assets/sounds/background/nature.ogg" << std::endl;

	musicBackgroundNature_.setLoop(true);
	musicBackgroundNature_.play();


}
void SoundManager::update(const sf::Time& dT)
{
	//auto iEraseAt = std::remove_if(lSounds_.begin(), lSounds_.end(), [](const sf::Sound& sound) {return sound.getStatus() == sf::Sound::Stopped;}); 
	//lSounds_.erase(iEraseAt, lSounds_.end());
	for (auto iSound = lSounds_.begin(); iSound != lSounds_.end();)
	{
		if (iSound->getStatus() == sf::Sound::Stopped)
			iSound = lSounds_.erase(iSound);
		else
			++iSound;
	}
	//std::cout << lSounds_.size() << std::endl ;
	//std::cout << lSounds_.back().getStatus() << std::endl;;


}
void SoundManager::playSound(const std::string& type, const sf::Vector2f& position, const sf::Vector2f& playerPosition)
{
	if (lSounds_.size() < 255)
	{
		sf::Vector2f distanceVector = position - playerPosition;
		float distance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
		if (distance < 800)
		{
			if (type == "pistol")
			{
				sf::Sound sound;
				int random = std::rand() % 3;
				if (random == 0)
					sound = sf::Sound(bufferPistolOne_);
				else if (random == 1)
					sound = sf::Sound(bufferPistolTwo_);
				else
					sound = sf::Sound(bufferPistolThree_);

				lSounds_.push_back(sound);
			}
			else if (type == "pistol_select")
			{
				sf::Sound sound(bufferPistolSelect_);
				lSounds_.push_back(sound);
			}
			else if (type == "magnum")
			{
				sf::Sound sound = sf::Sound(bufferMagnumOne_);

				lSounds_.push_back(sound);
			}
			else if (type == "magnum_select")
			{
				sf::Sound sound(bufferMagnumSelect_);
				lSounds_.push_back(sound);
			}
			else if (type == "shotgun")
			{
				sf::Sound sound(bufferShotgunOne_);
				lSounds_.push_back(sound);
			}
			else if (type == "shotgun_select")
			{
				sf::Sound sound(bufferShotgunSelect_);
				lSounds_.push_back(sound);
			}
			else if (type == "rifle")
			{
				sf::Sound sound;
				int random = std::rand() % 4;
				if (random == 0)
					sound = sf::Sound(bufferRifleOne_);
				else if (random == 1)
					sound = sf::Sound(bufferRifleTwo_);
				else
					sound = sf::Sound(bufferRifleThree_);

				lSounds_.push_back(sound);
			}
			else if (type == "rifle_select")
			{
				sf::Sound sound(bufferRifleSelect_);
				lSounds_.push_back(sound);
			}
			else if (type == "explosion")
			{
				sf::Sound sound = sf::Sound(bufferExplosion_);
				lSounds_.push_back(sound);
			}
			else if (type == "zombie")
			{
				sf::Sound sound;
				int random = 0;

				if (random == 0)
					sound = sf::Sound(bufferZombieOne_);
				else if (random == 1)
					sound = sf::Sound(bufferZombieTwo_);
				else if (random == 2)
					sound = sf::Sound(bufferZombieThree_);
				else if (random == 3)
					sound = sf::Sound(bufferGrassFour_);
				else
					sound = sf::Sound(bufferGrassFive_);
				lSounds_.push_back(sound);
			}
			else if (type == "zombie_ranged")
			{
				sf::Sound sound(bufferRangedZombie_);
				lSounds_.push_back(sound);
			}
			else if (type == "grass")
			{

				sf::Sound sound;
				int random = std::rand() % 7;
				if (random == 0)
					sound = sf::Sound(bufferGrassOne_);
				else if (random == 1)
					sound = sf::Sound(bufferGrassTwo_);
				else if (random == 2)
					sound = sf::Sound(bufferGrassThree_);
				else if (random == 3)
					sound = sf::Sound(bufferGrassFour_);
				else if (random == 4)
					sound = sf::Sound(bufferGrassFive_);
				else if (random == 5)
					sound = sf::Sound(bufferGrassSix_);
				else
					sound = sf::Sound(bufferGrassSeven_);


				lSounds_.push_back(sound);
			}
			else if (type == "hit")
			{

				sf::Sound sound;
				int random = std::rand() % 4;
				if (random == 0)
					sound = sf::Sound(bufferHitOne_);
				else if (random == 1)
					sound = sf::Sound(bufferHitTwo_);
				else if (random == 2)
					sound = sf::Sound(bufferHitThree_);
				else if (random == 3)
					sound = sf::Sound(bufferHitFour_);

				lSounds_.push_back(sound);
			}
			else if (type == "zombie_death")
			{
				sf::Sound sound = sf::Sound(bufferCrunchySplat_);
				lSounds_.push_back(sound);
			}
			else if (type == "hover")
			{
				lSounds_.push_back(sf::Sound(bufferHover_));
			}
			else if (type == "click")
			{
				lSounds_.push_back(sf::Sound(bufferClick_));
			}
			lSounds_.back().setVolume((1.0f - (distance / 800.0f)) * 200.0f);
			lSounds_.back().play();
		}
	}
}
