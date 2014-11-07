Game Engines 1 - C++ & git
==========================

## Questions we will answer today
- C++ Classes
- Inheritance
- Memory management
- stl & iterators
- git concepts
- git gotchas
- git workflow
- Branching & merging
- Forking

## Classes in C++
- .h file for the interface (API) to the class
- .cpp file for the implementation of the class
- Different syntax to Java/C# for public/private/protected
- C++ has namespaces like C#
- Methods are called member functions
- Fields are called member variables
- No standard for accessors (I don’t use them in BGE anyway)
- Use the :: (scope resultion operator) to indicate that a function is scoped to a class or namespace.

Example .h file:

```C++
#pragma once
#include <glm/glm.hpp>
namespace Game1
{
	class GameComponent
	{
	public:
		GameComponent(void);
		~GameComponent(void);

		glm::vec2 position;
		glm::vec2 look;
		float rotation;
		float speed;

		virtual bool Initialise();
		virtual void Update(float timeDelta);
		virtual void Draw();
		virtual void Cleanup();

	};
}
```

Example .cpp file:

```C++
#include "GameComponent.h"
#include <iostream>
using namespace Game1;

GameComponent::GameComponent(void)
{
	std::cout << "Base class constructor" << std::endl;
	position = glm::vec2(0.0f);
	look = glm::vec2(0.0f, 1.0f);
	rotation = 0.0f;
	speed = 0.0f;
}


GameComponent::~GameComponent(void)
{
}

bool GameComponent::Initialise()
{
	return true;
}

void GameComponent::Update(float timeDelta)
{

}

void GameComponent::Draw()
{

}

void GameComponent::Cleanup()
{

}

```
## Inheritance
- C++ has no concept of interfaces like in Java/C#
- It does have multiple inheritance though
- The syntax for inheritance is similar to that of C#. I.e. Use a :
- You can have public, private and protected inheritance, but I recommend you always use public inheritance.

Example:

```C++
#pragma once
#include "GameComponent.h"
#include <SDL.h>

namespace Game1
{
	class Player :
		public GameComponent
	{
	public:
		Player(void);
		~Player(void);

		SDL_Texture * texture; 

		void Update(float timeDelta);
		bool Initialise();
		void Update();
		void Draw();

	};
}
```
- Member functions are statically bound like in C++ unless you use the keyword ```virtual``` in the base class member function signature
- In BGE, when you extend a class you should make sure to call the appropriate member function in the base class. For example:

```C++
void Player::Update(float timeDelta)
{
	const Uint8 * keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_UP])
	{
		position += look * speed * timeDelta; 
	}

	if (keyState[SDL_SCANCODE_DOWN])
	{
		position -= look * speed * timeDelta; 
	}

	// How to call Update in the base class...
	GameComponent::Update(timeDelta);
}
```

## Memory Management
* C++ has pointers like in C
* To instantiate a new object on the heap you can use ```new```. However you need to remember to call ```delete``` on the pointer.
* The objects destructor member function will get called automatically when the object is deleted
* C++ also has pass by reference like in C#

Example of a pointer that is passed by reference:

```C++
void VRGame2::GravityGun(PhysicsController * & pickedUp, KinectHand * hand);
```
* Managing pointers like this is error prone and cumbesome.
* C++ 11 now has really cool *smart pointers*.
* These are *reference counted pointers*. The object will be automatically destructed when the reference count goes to 0.
* BGE uses these as much as possible
* Use of reference counted pointers is not teh same as garbage collection in C#, Java
* It is in fact much faster as it does not rely on a garbage collection *thread* like C#/Java
* Instead it just uses the C++ scope rules.

Example:

```C++
#include <iostream>
#include "Player.h"
#include <memory>
using namespace std;

void doSomething(shared_ptr<Player> player) // ref count is 2
{
	shared_ptr<Player> p = player; // ref count is 3
	p->position = glm::vec2(100, 100);
} // ref count goes back to 1


void main()
{
	shared_ptr<Player> ppp = make_shared<Player>(); // ref count is 1
	cout << "Something" << endl;
} // ref count goes to 0 and the object is destructed
```

STL
---
- The STL is the Standard Template Library
- A collection of generic data structures
- BGE uses the following from the STL:
- <code>vector</code> - Like a dynamic array. Random access but slow insertions & deletions.
- <code>list</code> - A linked list. No random access but fast insertions and deletions. Iterate using an iterator
- <code>map</code> - A key value pair. Fast lookups.
- <code>multimap</code> - A key value pair. Fast lookups. Multiple identical keys.
- <code>shared_ptr</code>'s work really well with the stl.
- Some data structors can be iterated using [] however some require the use of an iterator

Some simple examples of the stl in action:

```C++
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <deque>
#include <time.h>
#include <string>
#include <algorithm>


using namespace std;

long lastMillis = 0;

long millis()
{
	clock_t cl = clock();
	return ((float) cl * (float) CLOCKS_PER_SEC) / 1000.0f; 
}

void elapsed()
{
	long milli = millis();
	cout << "Elapsed: " << milli - lastMillis << endl;
	lastMillis = milli;
}

struct ArraySorter
{
	bool operator()(int a, int b)
	{
		return (a > b);
	}
};

void main()
{
	lastMillis = millis();
	// First an example of the vector template
	vector<int> v;
	const int NUM_ELEMENTS = 10000;

	elapsed();
	srand(time(NULL));

	for (int i = 0 ; i < 10 ; i ++)
	{
		v.push_back(rand());
	}
	cout << "Now print them out:" << endl;
	for (int i = 0 ; i < v.size() ; i ++)
	{
		cout << i << ": " << v[i] << endl;
	}

	vector<int>::iterator vit = v.begin();
	while (vit != v.end())
	{
		cout << * vit << endl;
		vit ++;
	}


	v.push_back(8);
	vector<int>::iterator found = find(v.begin(), v.end(), 8);
	if (found == v.end())
	{
		cout << "I couldnt find the number 8 in the vector" << endl;
	}
	else
	{
		cout << "I found the number 8 in the vector" << endl;
	}

	cout << "Now print them out using an iterator:" << endl;
	vector<int>::iterator it = v.begin();

	int i = 0;
	while (it != v.end())
	{
		cout << (i++) << ": " << (* it) << endl;
		it ++;
	}
	elapsed();
	cout << "Now sort them:" << endl;
	sort(v.begin(), v.end());
	it = v.begin();
	while (it != v.end())
	{
		cout << (i++) << ": " << (* it) << endl;
		
	}
	elapsed();
	cout << "Now sort them using a functor:" << endl;
	sort(v.begin(), v.end(), ArraySorter());
	it = v.begin();
	while (it != v.end())
	{
		cout << (i++) << ": " << (* it) << endl;
		it ++;
	}
	elapsed();

	cout << "Copy them into a list:" << endl;
	list<int> l(v.size());
	copy(v.begin(), v.end(), l.begin());
	
	cout << "Print the list:" << endl;
	list<int>::iterator lit = l.begin();
	i = 0;
	while (lit != l.end())
	{
		cout << (i++) << ": " << (* lit) << endl;
		lit ++;
	}
	
	cout << "Now test a map:" << endl;
	
	map<string, int> ages;

	ages["bryan"] = 35;
	ages["stephen"] = 21;
	ages["oroti"] = 22;
	ages["thomas"] = 17;
	ages["lyons"] = 23;

	map<string, int>::iterator mit = ages.begin();
	while(mit != ages.end())
	{
		cout << mit->first << "\t" << mit->second << endl;
		mit ++;
	}

	// Now test the map find
	map<string, int>::iterator foundMap = ages.find("oroti");
	if (foundMap == ages.end())
	{
		cout << "I couldnt find the oroti in the map" << endl;
	}
	else
	{
		cout << "Oroti's age is " << foundMap->second;
	}

	cout << "Now a test of a priority q" << endl;
	priority_queue<int, vector<int>> pq;
	elapsed();
	for (int i = 0 ; i < NUM_ELEMENTS ; i ++)
	{
		pq.push(rand());
	}
	elapsed();
	while (! pq.empty())
	{
		cout << pq.top() << endl;
		pq.pop();
	}
	elapsed();
}
```

Some of the iterator code can be difficult to figure out! Consider the following actual code from BGE:

```C++
std::vector<std::shared_ptr<GameComponent>> GameComponent::FindComponentsByTag(std::string tag)
{
	// This looks ugly, but hey thats C++ for ya
	pair<std::multimap<std::string, std::shared_ptr<GameComponent>>::iterator
		, std::multimap<std::string, std::shared_ptr<GameComponent>>::iterator> range = childrenMap.equal_range(tag);
	
	std::multimap<std::string, std::shared_ptr<GameComponent>>::iterator it = range.first;
	std::vector<std::shared_ptr<GameComponent>> components;
	while (it != range.second)
	{
		components.push_back((*it).second);		
		it++;
	}
	return components;
}
```

## git concepts
* git is an absolutely awesome source code control system
* git almost never deletes *anything*
* gis is very efficient at *diffing* in other words figuring out what has changed between revisions and only storing the changes
* GitHub is where you can host projects in git
* You can host unlimited free repositories on GitHub
* Youc an register for git academic with your student number and get free private repositories
* You NEED to do the following:
* Install [git for Windows](Git-1.9.4-preview20140815.exe)
* Read the first three chapters of the [git manual](http://git-scm.com/book/en/)
* Use git to manage a project
* Then you will learn to love it just like I do!

## git gotchas
* git is for source code *mostly*.
* Only your source code and projects files should be in the repo.
* Content such as .png's and 3D models are ok, so long as they dont change (because git cant diff binary files very well).
* Use a .gitignore file with your repo to tell git to not check in the outputs of your build. USe GitHub to make one for your project type.
* Do not put dependencies in your git repo. Provide a link to install them instead.

## git workflow for a new project
* Create the project in Visual Studio or whatever
* Navigate to the project folder and start a bash shell
* Type:

	```bash
	git init
	```
* Go up to GitHub and create a new repo for the project. Make sure to tell GitHub to create a .gitignore file
* I always use the MIT Licence for my Open Source projects
* Copy the URL of the project
* Now go back to the bash shell and type:

	```bash
	git remote add origin THE_URL_THAT_YOU_COPIED
	git pull origin master
	```
* This sets up the remote alias for origin and pulls down the changes from the server into the master branch. The next time you pull, you wont have to type <code>origin master</code>
* Type:
 
	```bash
	git add . --all
	git commit -m "Type your commit message here"
	git push
	```
* This will:

	1. Add your files to the git staging area
	2. Commit your files to the local git repository
	3. Push your changes to the remote server

* You can get the commit history by typing:

	```bash
	git log
	```
* You can move the HEAD back to a particular commit by typing:

	```bash
	git checkout THE_40_DIGIT_ID_OF_THE_COMMIT
	```

## git branching

* You can create branches to work on new features and then merge them back into the master branch when you are done:
	```bash
	git checkout -b BRANCH_NAME
	```
* Your subsequent commits will be added to this new branch. To push this branch to the server type:

	```bash
	git push origin BRANCH_NAME
	```
* To merge a branch, navigate to the *branch you want to merge into* and type:

	```bash
	git merge BRANCH_YOU_WANT_TO_MERGE_FROM
	```
* You can delete a branch by typing:

	```bash
	git branch -d BRANCH_NAME
	```

## git forking
* You can fork a git repository to get a version of the project you can use as a starter for a new project
* Read [this guide](https://help.github.com/articles/fork-a-repo) to forking repository and fork BGE!
* At some point, if you do something cool, please submit a pull request and submit your code back into BGE for next years students.
