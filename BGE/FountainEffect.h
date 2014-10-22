#pragma once
#include "ParticleEffect.h"

namespace BGE
{
	class FountainEffect :
		public ParticleEffect
	{
	public:
		FountainEffect(bool hasTransform = false);
		FountainEffect(int numParticles, bool hasTransform = false);
		~FountainEffect(void);

		bool Initialise();
		void Update();
		void InitParticle(Particle & particle);
		void UpdateParticle(Particle & particle);

		int numParticles;
	};
}

