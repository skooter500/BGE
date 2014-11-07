#pragma once
#include "GameComponent.h"


namespace BGE
{
	class Sphere :
		public GameComponent
	{
	public:
		Sphere(float radius);
		~Sphere(void);
		void Update();
		bool Initialise();
	};
}
