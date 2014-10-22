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
		bool FPSController::Initialise();
	};
}
