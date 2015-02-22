#include "Emitter.h"

Emitter::Emitter(const bool hasSpawnLimit,
		 const int particlesToSpawn,
		 const int particlesPerSecond,
		 const sf::Vector2f& startingParticleSize,
		 const sf::Vector2f& endingParticleSize,
		 const float minDirection,
		 const float maxDirection,
		 const float minSpeed,
		 const float maxSpeed,
		 const float speedAcceleration,
		 const float minLife,
		 const float maxLife,
		 const sf::Color& startingColor,
		 const sf::Color& endingColor)
    :hasSpawnLimit_(hasSpawnLimit),
     particlesToSpawn_(particlesToSpawn),
     particlesPerSecond_(particlesPerSecond),
     startingParticleSize_(startingParticleSize),
     endingParticleSize_(endingParticleSize),
     minDirection_(minDirection),
     maxDirection_(maxDirection),
     minSpeed_(minSpeed),
     maxSpeed_(maxSpeed),
     speedAcceleration_(speedAcceleration),
     minLife_(minLife),
     maxLife_(maxLife),
     startingColor_(startingColor),
     endingColor_(endingColor)
{}
    
void Emitter::update(const sf::Time& dT)
{
    if(spawnClock_.getElapsedTime().asSeconds() >= 1.0f / particlesPerSecond_)
    {
	spawnClock_.restart();
	float direction = minDirection_ + std::rand() % (maxDirection_ - minDirection_);
	float speed = minSpeed_ + std::rand() % (maxSpeed_ - minSpeed_);
	float life = minLife_ + std::rand() % (maxLife_ - minLife_);
	
	lParticles_.pushBack(Particle(startingParticleSize_,
				      endingParticleSize_,
				      direction,
				      speed,
				      speedAcceleration_,
				      life,
				      startingColor_,
				      endingColor_));
    }
				      
    for(auto iParticle = lParticles_.begin(); iParticle != lParticles.end();)
    {
	//Updates the particle
	iParticle->update(dT);

	//Checks if it is dead, erases if so, iterates otherwise
	if(iParticle->isDead())
	    iParticle = lParticles.erase(iParticle);
	else
	    ++iParticle;
    }
}

std::list<Particle> Emitter::getParticles() const { return lParticles_; }
