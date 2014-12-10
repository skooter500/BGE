#pragma once
#include "GameComponent.h"

namespace BGE
{
	class GravityController :public GameComponent
	{
	public:
		GravityController();
		~GravityController();

		glm::vec3 gravity;

		void Update();
	};
}
