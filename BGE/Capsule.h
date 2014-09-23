#pragma once
#include "GameComponent.h"
#include <iostream> 


namespace BGE
{
	class Capsule :
		public GameComponent
	{
	public:
		Capsule(float radius, float height);
		~Capsule(void);

		bool Initialise();
		void Draw();
	};
}
