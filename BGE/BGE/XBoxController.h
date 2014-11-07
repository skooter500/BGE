#pragma once
#include "GameComponent.h"

namespace BGE
{
	class XBoxController :
		public GameComponent
	{
	public:
		XBoxController(bool hasTransform = false);
		~XBoxController(void);

		void Update();
		bool Initialise();
		bool disablePitch;
	};
}
