#pragma once
#include "Game.h"

namespace BGE
{
	class ScriptingExample :
		public Game
	{
	private:

	public:
		ScriptingExample(void);
		~ScriptingExample(void);
		bool Initialise();
		void Update();
		void Cleanup();
		void CreateWall();
	};
}