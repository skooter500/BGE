Game Engines 1 Assignment
=========================

## Interm Assignment
Please subit through [this Google Docs link](https://docs.google.com/forms/d/13J_laob3a-Eixw2u1QRjK4eHsgKQ5hJ9F9smu0n-wdw/viewform) the following:

A gitHub link to a fork the BGE repository and the name of a branch that has your code for the assignment. 

In the *code* please make:
- A subclass of Game for your assignnmemt
- Have a look at ```PhysicsGame1``` for an example
- Some code to procedurally place some rigid bodies in the scene. For example use a for loop to make a wall

In the *readme.md* file at the root of your project, you should include:
- Your full name & student number
- A description of what you plan to make
- Some links to png's or whatever with some diagrams of what you plan to make
	- Try isometric, top down and side on diagrams
	- Include the rigid body types & joint types
	- Include the power source
	- You can draw these on paper and take a picture with your phone
	- If you are planning o contribute to BGE, then please describe what your contribution will be and how you plan to do it

This is worth 10% (out of a total of 100%) and is due on Friday 7 November.

## Main Assignment
Use BGE (or your own engine if you like) to make an articulated animat or robot simulation that walks, crawls, flys, rolls or drags itself around under its own power. Control via an input device is also acceptable. Like this dragon controllable using the Kinect:

[![Video](http://img.youtube.com/vi/fipi6hplcn4/0.jpg)](http://www.youtube.com/watch?v=fipi6hplcn4)

Feel free to use the rift, Kinect, Leap Motion or WiiMote in your solution.

Here is a [YouTube playlist you can use for inspiration](https://www.youtube.com/playlist?list=PL1n0B6z4e_E5ybHULPRLErccV8BeHvH5V). 

In your solution you must make use of:
- Rigid bodies
- Joints
- Motors/controllers

You should also make an effort to make your creation look awesome. The marking scheme is below:

| Complexity | OO | Visuals | Jazz |
|------------|----|---------|------|
|30%         |20% |20%      |30%   |

You should spend around 16 hours on the assignment. 
Here is a rubric I will use to grade the assignments. 

| Grade | Description |
| ------|-------------|
| First | Your animat works and moves realistically. You make a pull request to BGE and it's accepted meaning you your code is good enough to be used by next years students. You have lots of different joint types and have figured out how to put appropriate limits on the joints so that they are realistic. The problem you solved was challenging and required a lot of figuring out. You have created new classes to abstract the complexity of your assignment. Your assignment has a lot of novelty and is impressive looking |
| 2.1 | Your animat works and moves with some minor issues. You have used a 2-3 joint types with no limits put on the joints. You have used just 1-2 rigid body shapes. The animat/thing you make is about as complicated as a [Watt beam engine](http://www.animatedengines.com/watt.html). You dont use any other controllers or tried to use a controller but couldnt get it working. Code is in functions and no new classes required.|
| 2.2 | Your animat doesnt move by itself. Your assignment is some rigid bodies connected by hinge joints or one joint type. Your assignmnet is about as complicated as a 4 wheeled cart. Everything is in one or two functions. |
| Pass | No animat. There are no joints, just some rigid bodies. Simple changes to to the PhysicsGame or SceneGraphGame code. |
| Fail | Project won't compile or major issues. Little or no code changes to the supplied code. |

This part of the assignment is worth 90% and is due on Wednesday 10th December.



