#pragma once
#include "GameComponent.h"

namespace BGE
{
	class FPSController:public GameComponent
	{
	public: 
		FPSController(void);
		~FPSController(void);
		void Update();
		void Jump(float height, float duration);
		bool FPSController::Initialise();

		float jumpHeight;
		float jumpY;
		float jumpDuration;
		float jumpTheta;
		glm::vec3 gravity;
		bool jumping;
	};
}
