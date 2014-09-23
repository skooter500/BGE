#pragma once
#include "ParticleEffect.h"
#include "Particle.h"
namespace BGE
{
	class SnowEffect :
		public ParticleEffect
	{
	public:
		SnowEffect(bool hasTransform = false);
		~SnowEffect(void);

		bool Initialise();
		void InitParticle(Particle & particle);
		void UpdateParticle(float timeDelta, Particle & particle);
	};
}

