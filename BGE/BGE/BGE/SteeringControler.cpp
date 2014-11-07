#include "SteeringControler.h"
#include "Game.h"
#include <gtx/norm.hpp>
#include <string>
#include "Utils.h"
#include "Params.h"
#include "Sphere.h"
#include <limits>

using namespace BGE;

bool SteeringController::counted = false;
vector<shared_ptr<GameComponent>> SteeringController::obstacles;
vector<shared_ptr<GameComponent>> SteeringController::steerables;


SteeringController::SteeringController(void)
{
	tag = "steeringController";
	force = glm::vec3(0);
	acceleration = glm::vec3(0);
	mass = 1.0f;
	Time::deltaTime = 0.0f;
	calculationMethod = CalculationMethods::WeightedTruncatedRunningSumWithPrioritisation;
	maxSpeed = Params::GetFloat("max_speed");
	maxForce = Params::GetFloat("max_force");
	wanderTarget = RandomPosition(1.0f);
	wanderTarget = glm::normalize(wanderTarget);
	wanderTarget *= Params::GetFloat("wander_radius");
	counted = false;

	route = make_shared<Route>();

	// Start with all behaviours turned off
	TurnOffAll();
}

bool SteeringController::Initialise()
{
	Attach(route);
	return GameComponent::Initialise();
}


SteeringController::~SteeringController(void)
{
}

void SteeringController::Update()
{
	float smoothRate;

	if (!SteeringController::counted)
	{
		SteeringController::obstacles = Game::Instance()->FindComponentsByTag("obstacle");
		SteeringController::steerables = Game::Instance()->FindComponentsByTag("steerable");
		SteeringController::counted = true;
	}


	force = Calculate();
	CheckNaN(force);
	glm::vec3 newAcceleration = force / mass;

	if (Time::deltaTime > 0)
	{
		smoothRate = Clip(9 * Time::deltaTime, 0.15f, 0.4f) / 2.0f;
		BlendIntoAccumulator(smoothRate, newAcceleration, acceleration);
	}

	transform->velocity += acceleration * Time::deltaTime;
	float speed = glm::length(transform->velocity);
	if (speed > maxSpeed)
	{

		transform->velocity = glm::normalize(transform->velocity);
		transform->velocity *= maxSpeed;
	}
	transform->position += transform->velocity * Time::deltaTime;

	// the length of this global-upward-pointing vector controls the vehicle's
	// tendency to right itself as it is rolled over from turning acceleration
	glm::vec3 globalUp = glm::vec3(0, 0.2f, 0);
	// acceleration points toward the center of local path curvature, the
	// length determines how much the vehicle will roll while turning
	glm::vec3 accelUp = acceleration * 0.05f;
	// combined banking, sum of transform->up due to turning and global transform->up
	glm::vec3 bankUp = accelUp + globalUp;
	// blend bankUp into vehicle's transform->up basis vector
	smoothRate = Time::deltaTime * 3;
	glm::vec3 tempUp = transform->up;
	BlendIntoAccumulator(smoothRate, bankUp, tempUp);
	transform->up = tempUp;
	transform->up = glm::normalize(transform->up);

	if (speed > 0.0001f)
	{
		transform->look = transform->velocity;
		transform->look = glm::normalize(transform->look);
		if (transform->look == transform->right)
		{
			transform->right = Transform::basisRight;
		}
		else
		{
			transform->right = glm::cross(transform->look, transform->up);
			glm::normalize(transform->right);

			CheckNaN(transform->right, Transform::basisRight);
			transform->up = glm::cross(transform->right, transform->look);
			transform->up = glm::normalize(transform->up);
			CheckNaN(transform->up, Transform::basisUp);
		}
		// Apply damping
		transform->velocity *= 0.99f;
	}

	if (transform->look != Transform::basisLook)
	{
		// Make a quaternion from the vectors
		/*glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0), transform->look, transform->up);
		rotationMatrix = glm::transpose(rotationMatrix);
		transform->orientation = glm::quat(rotationMatrix);*/

		// Alternatively
		glm::vec3 axis = glm::cross(Transform::basisLook, transform->look);
		axis = glm::normalize(axis);
		float angle = glm::acos(glm::dot(Transform::basisLook, transform->look));
		transform->orientation = glm::angleAxis(glm::degrees(angle), axis);
	}

	GameComponent::Update();
}

int SteeringController::TagNeighboursSimple(float inRange)
{
	tagged.clear();
	static float inRange2 = inRange * inRange;
	for (int i = 0; i < SteeringController::steerables.size(); i++)
	{
		if (SteeringController::steerables[i] != parent)
		{
			shared_ptr<GameComponent> neighbour = steerables[i];
			float distance = glm::length2(transform->position - neighbour->transform->position);
			if (distance < inRange2)
			{
				tagged.push_back(neighbour);
			}
		}
	}
	return tagged.size();
}

bool SteeringController::IsOn(behaviour_type behaviour)
{
	return ((flags & (int)behaviour) == (int)behaviour);
}

void SteeringController::TurnOn(behaviour_type behaviour)
{
	flags |= ((int)behaviour);
}

void SteeringController::TurnOffAll()
{
	flags = (int)SteeringController::behaviour_type::none;
}

glm::vec3 SteeringController::Cohesion()
{
	glm::vec3 steeringForce = glm::vec3(0);
	glm::vec3 centreOfMass = glm::vec3(0);
	int taggedCount = 0;

	for (int i = 0; i < tagged.size(); i++)
	{
		shared_ptr<GameComponent> entity = tagged[i];
		if (entity != parent)
		{
			centreOfMass += entity->transform->position;
			taggedCount++;
		}
	}
	if (taggedCount > 0)
	{
		centreOfMass /= (float)taggedCount;
		if (glm::length2(centreOfMass) == 0)
		{
			steeringForce = glm::vec3(0);
		}
		else
		{
			steeringForce = glm::normalize(Seek(centreOfMass));
		}
	}
	CheckNaN(steeringForce);
	return steeringForce;
}

glm::vec3 SteeringController::Alignment()
{
	glm::vec3 steeringForce = glm::vec3(0);
	int taggedCount = 0;

	for (int i = 0; i < tagged.size(); i++)
	{
		shared_ptr<GameComponent> entity = tagged[i];
		if (entity != parent)
		{
			steeringForce += entity->transform->look;
			taggedCount++;
		}
	}

	if (taggedCount > 0)
	{
		steeringForce /= (float)taggedCount;
		steeringForce -= transform->look;
	}
	return steeringForce;

}

glm::vec3 SteeringController::SphereConstrain(float radius)
{
	float distance = glm::length(transform->position);
	glm::vec3 steeringForce = glm::vec3(0);
	if (distance > radius)
	{
		steeringForce = glm::normalize(transform->position) * (radius - distance);
	}
	return steeringForce;
}


glm::vec3 SteeringController::Separation()
{
	glm::vec3 steeringForce = glm::vec3(0);

	for (int i = 0; i < tagged.size(); i++)
	{
		shared_ptr<GameComponent> entity = tagged[i];
		if (entity != parent)
		{
			glm::vec3 toEntity = transform->position - entity->transform->position;
			steeringForce += (glm::normalize(toEntity) / glm::length(toEntity));
		}
	}

	return steeringForce;
}

glm::vec3  SteeringController::Evade()
{
	float dist = glm::length(target->transform->position - transform->position);
	float lookAhead = (dist / maxSpeed);

	glm::vec3 targetPos = target->transform->position + (lookAhead * target->transform->velocity);
	return Flee(targetPos);
}


glm::vec3 SteeringController::ObstacleAvoidance()
{
	glm::vec3 force = glm::vec3(0);
	makeFeelers();

	vector<shared_ptr<GameComponent>> tagged;

	float minBoxLength = 50.0f;
	float boxLength = minBoxLength + ((glm::length(transform->velocity) / maxSpeed) * minBoxLength * 2.0f);

	if (glm::isnan(boxLength))
	{
		BGE::LogMessage("NAN");
	}
	// Matt Bucklands Obstacle avoidance
	// First tag obstacles in range

	for (int i = 0; i < obstacles.size(); ++i)
	{
		glm::vec3 toCentre = transform->position - obstacles[i]->transform->position;
		float dist = glm::length(toCentre);
		if (dist < boxLength)
		{
			tagged.push_back(obstacles[i]);
		}
	}
	float distToClosestIP = numeric_limits<float>::max();
	shared_ptr<GameComponent> closestIntersectingObstacle = nullptr;
	glm::vec3 localPosOfClosestObstacle = glm::vec3(0);
	glm::vec3 intersection = glm::vec3(0);

	for (int i = 0; i < tagged.size(); i++)
	{
		glm::vec3 localPos = transform->InverseTransformPosition(tagged[i]->transform->position);

		// If the local transform->position has a positive Z value then it must lay
		// behind the agent. (in which case it can be ignored)
		if (localPos.z <= 0)
		{
			// If the distance from the x axis to the object's transform->position is less
			// than its radius + half the width of the detection box then there
			// is a potential intersection.
			float expandedRadius = transform->scale.z + tagged[i]->transform->scale.z;
			if ((glm::abs(localPos.y) < expandedRadius) && (glm::abs(localPos.x) < expandedRadius))
			{
				// Now to do a ray/sphere intersection test. The center of the				
				SphereGeom tempSphere;
				tempSphere.radius = expandedRadius;
				tempSphere.pos = localPos;

				// Create a ray
				RayGeom ray;
				ray.pos = glm::vec3(0, 0, 0);
				ray.look = Transform::basisLook;

				// Find the point of intersection
				if (ClosestRayIntersectsSphere(ray, tempSphere, glm::vec3(0), intersection))
				{
					// Now see if its the closest, there may be other intersecting spheres
					float dist = glm::length(intersection);
					if (dist < distToClosestIP)
					{
						dist = distToClosestIP;
						closestIntersectingObstacle = tagged[i];
						localPosOfClosestObstacle = localPos;
					}
				}
			}
		}
		if (closestIntersectingObstacle != nullptr)
		{
			float multiplier = 1.0f + (boxLength - localPosOfClosestObstacle.z) / boxLength;

			//calculate the lateral force
			float obstacleRadius = closestIntersectingObstacle->transform->scale.x; 
			float expandedRadius = transform->scale.x + obstacleRadius;
			force.x = (expandedRadius - glm::abs(localPosOfClosestObstacle.x)) * multiplier;
			force.y = (expandedRadius - glm::abs(localPosOfClosestObstacle.y)) * multiplier;

			// Generate positive or negative direction so we steer around!
			// Not always in the same direction as in Matt Bucklands book
			if (localPosOfClosestObstacle.x > 0)
			{
				force.x = -force.x;
			}

			// If the obstacle is above, steer down
			if (localPosOfClosestObstacle.y > 0)
			{
				force.y = -force.y;
			}
			//apply a braking force proportional to the obstacle's distance from
			//the vehicle.
			const float brakingWeight = 0.01f;
			force.z = (expandedRadius -
				localPosOfClosestObstacle.z) *
				brakingWeight;

			//finally, convert the steering vector from local to world space
			// Dont include position!                    
			force = transform->TransformNormal(force);
		}
	}
	return force;
}

glm::vec3 SteeringController::OffsetPursuit(glm::vec3 offset)
{
	glm::vec3 target = glm::vec3(0);

	target = leader->transform->TransformPosition(offset);

	float dist = glm::length(target - transform->position);

	float lookAhead = (dist / maxSpeed);

	target = target + (lookAhead * leader->transform->velocity);

	CheckNaN(target);
	return Arrive(target);
}

glm::vec3 SteeringController::Pursue()
{
	float dist = glm::length(target->transform->position - transform->position);

	float lookAhead = (dist / maxSpeed);

	glm::vec3 targetPos = target->transform->position + (lookAhead * target->transform->velocity);
	return Seek(targetPos);
}

glm::vec3 SteeringController::Flee(glm::vec3 targetPos)
{
	float panicDistance = 100.0f;
	glm::vec3 desiredVelocity;
	desiredVelocity = transform->position - targetPos;
	if (glm::length(desiredVelocity)  > panicDistance)
	{
		return glm::vec3(0);
	}

	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= maxSpeed;

	return (desiredVelocity - transform->velocity);
}

glm::vec3 SteeringController::RandomWalk()
{
	float dist = glm::length(transform->position - randomWalkTarget);
	if (dist < 50)
	{
		randomWalkTarget.x = RandomClamped() * Params::GetFloat("world_range");
		randomWalkTarget.y = RandomClamped() * Params::GetFloat("world_range");
		randomWalkTarget.z = RandomClamped() * Params::GetFloat("world_range");
	}
	return Seek(randomWalkTarget);
}

glm::vec3 SteeringController::Seek(glm::vec3 targetPos)
{
	glm::vec3 desiredVelocity;

	desiredVelocity = targetPos - transform->position;

	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= maxSpeed;

	return (desiredVelocity - transform->velocity);
}


glm::vec3 SteeringController::Wander()
{
	float jitterTimeSlice = Params::GetFloat("wander_jitter") * Time::deltaTime;

	glm::vec3 toAdd = RandomInsideUnitSphere() * jitterTimeSlice;
	wanderTarget += toAdd;
	wanderTarget = glm::normalize(wanderTarget);
	wanderTarget *= Params::GetFloat("wander_radius");

	glm::vec3 localTarget = wanderTarget + (Transform::basisLook * Params::GetFloat("wander_distance"));
	glm::vec3 worldTarget = transform->TransformPosition(localTarget);
	
	return (worldTarget - transform->position);
}

glm::vec3 SteeringController::WallAvoidance()
{
	makeFeelers();


	glm::vec3 force = glm::vec3(0);

	/*
	Plane worldPlane = new Plane(new glm::vec3(0, 1, 0), 0);

	foreach (glm::vec3 feeler in fighter.Feelers)
	{
	float dot = worldPlane.DotCoordinate(feeler);
	if (dot < 0)
	{
	float distance = Math.Abs(dot - worldPlane.D);
	force += worldPlane.Normal * distance;
	}
	}

	if (force.Length() > 0.0)
	{
	fighter.DrawFeelers = true;
	}
	else
	{
	fighter.DrawFeelers = false;
	}
	fighter.DrawAxis = false;
	*/
	return force;
}

void SteeringController::makeFeelers()
{
	feelers.clear();
	float feelerDistance = 20.0f;
	// Make the forward feeler
	glm::vec3 newFeeler = Transform::basisLook * feelerDistance;
	newFeeler = transform->TransformPosition(newFeeler);
	feelers.push_back(newFeeler);

	newFeeler = Transform::basisLook * feelerDistance;
	newFeeler = glm::vec3(glm::rotate(glm::mat4(1), 45.0f, Transform::basisUp) * glm::vec4(newFeeler, 1));
	newFeeler = transform->TransformPosition(newFeeler); 
	feelers.push_back(newFeeler);

	newFeeler = Transform::basisLook * feelerDistance;
	newFeeler = glm::vec3(glm::rotate(glm::mat4(1), -45.0f, Transform::basisUp) * glm::vec4(newFeeler, 1));
	newFeeler = transform->TransformPosition(newFeeler); 
	feelers.push_back(newFeeler);

	newFeeler = Transform::basisLook * feelerDistance;
	newFeeler = glm::vec3(glm::rotate(glm::mat4(1), 45.0f, Transform::basisRight) * glm::vec4(newFeeler, 1));
	newFeeler = transform->TransformPosition(newFeeler); 
	feelers.push_back(newFeeler);

	newFeeler = Transform::basisLook * feelerDistance;
	newFeeler = glm::vec3(glm::rotate(glm::mat4(1), -45.0f, Transform::basisRight) * glm::vec4(newFeeler, 1));
	newFeeler = transform->TransformPosition(newFeeler); 
	feelers.push_back(newFeeler);
}


glm::vec3 SteeringController::Arrive(glm::vec3 target)
{
	glm::vec3 toTarget = target - transform->position;

	float slowingDistance = 8.0f;
	float distance = glm::length(toTarget);
	if (distance == 0.0f)
	{
		return glm::vec3(0);
	}
	const float DecelerationTweaker = 10.3f;
	float ramped = maxSpeed * (distance / (slowingDistance * DecelerationTweaker));

	float clamped = glm::min<float>(ramped, maxSpeed);
	glm::vec3 desired = clamped * (toTarget / distance);

	CheckNaN(desired);

	return desired - transform->velocity;
}

glm::vec3 SteeringController::Calculate()
{
	if (calculationMethod == CalculationMethods::WeightedTruncatedRunningSumWithPrioritisation)
	{
		return CalculateWeightedPrioritised();
	}

	return glm::vec3(0);
}

glm::vec3 SteeringController::CalculateWeightedPrioritised()
{
	glm::vec3 force = glm::vec3(0);
	glm::vec3 steeringForce = glm::vec3(0);

	if (IsOn(behaviour_type::obstacle_avoidance))
	{
		force = ObstacleAvoidance() * Params::GetWeight("obstacle_avoidance_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}
	CheckNaN(force);
	if (IsOn(behaviour_type::wall_avoidance))
	{
		force = WallAvoidance() * Params::GetWeight("wall_avoidance_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::sphere_constrain))
	{
		force = SphereConstrain(Params::GetFloat("world_range")) * Params::GetWeight("sphere_constrain_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::evade))
	{
		force = Evade() * Params::GetWeight("evade_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	int tagged = 0;
	if (IsOn(behaviour_type::separation) || IsOn(behaviour_type::cohesion) || IsOn(behaviour_type::alignment))
	{
		tagged = TagNeighboursSimple(Params::GetFloat("tag_range"));
	}
	

	if (IsOn(behaviour_type::separation) && (tagged > 0))
	{
		force = Separation() * Params::GetWeight("separation_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::alignment) && (tagged > 0))
	{
		force = Alignment() * Params::GetWeight("alignment_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::cohesion) && (tagged > 0))
	{
		force = Cohesion() * Params::GetWeight("cohesion_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::seek))
	{
		force = Seek(targetPos) * Params::GetWeight("seek_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::arrive))
	{
		force = Arrive(targetPos) * Params::GetWeight("arrive_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::wander))
	{
		force = Wander() * Params::GetWeight("wander_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::pursuit))
	{
		force = Pursue() * Params::GetWeight("pursuit_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::offset_pursuit))
	{
		force = OffsetPursuit(offset) * Params::GetWeight("offset_pursuit_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::follow_path))
	{
		force = FollowPath() * Params::GetWeight("follow_path_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	if (IsOn(behaviour_type::random_walk))
	{
		force = RandomWalk() * Params::GetWeight("random_walk_weight");
		if (!AccumulateForce(steeringForce, force))
		{
			return steeringForce;
		}
	}

	return steeringForce;
}

glm::vec3 SteeringController::FollowPath()
{
	float epsilon = 5.0f;
	float dist = glm::length(transform->position - route->NextWaypoint());
	if (dist < epsilon)
	{
		route->AdvanceToNext();
	}
	if ((!route->looped) && route->IsLast())
	{
		return Arrive(route->NextWaypoint());
	}
	else
	{
		return Seek(route->NextWaypoint());
	}
}

bool SteeringController::AccumulateForce(glm::vec3 & runningTotal, glm::vec3 force)
{
	float soFar = glm::length(runningTotal);

	float remaining = maxForce - soFar;
	if (remaining <= 0)
	{
		return false;
	}

	float toAdd = glm::length(force);


	if (toAdd < remaining)
	{
		runningTotal += force;
	}
	else
	{
		runningTotal += glm::normalize(force) * remaining;
	}
	return true;
}