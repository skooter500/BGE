#pragma once
#include "Game.h"

namespace BGE
{
	class EmptyGame :
		public Game
	{
	public:
		EmptyGame();
		~EmptyGame();

		bool Initialise();
		void Update();
	};
}
