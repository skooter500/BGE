#pragma once
#include <string>
#include "SteeringControler.h"

using namespace std;

namespace BGE
{
	class Scenario
	{
	public:
		Scenario(void);

		virtual string Description() = 0;
		virtual void Initialise() = 0;
		virtual void Update();

		shared_ptr<SteeringController> leaderController;
	};
}
