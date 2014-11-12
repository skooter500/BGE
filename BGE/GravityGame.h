#pragma once
#include "Game.h"

namespace BGE
{
	class GravityGame :
		public Game
	{
	public:
		GravityGame();
		~GravityGame();

		bool Initialise();

		void Update();

	};
}
