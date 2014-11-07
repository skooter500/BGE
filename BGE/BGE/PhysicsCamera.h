#pragma once
#include "Camera.h"
#include "PhysicsController.h"
#include <btBulletDynamicsCommon.h>
#include "PhysicsFactory.h"
#include "Utils.h"

namespace BGE

{
	class PhysicsCamera :
		public PhysicsController, public btMotionState 
	{
	private:
		PhysicsController * pickedUp;
	public:
		PhysicsCamera(PhysicsFactory * physicsFactory);
		~PhysicsCamera(void);

		//bool Initialise();
		void PhysicsCamera::Update();

		void getWorldTransform(btTransform &worldTrans) const;
		void setWorldTransform(const btTransform &worldTrans);
		void GravityGun(RayGeom ray, bool isPhys);
		PhysicsFactory * physicsFactory;
		float elapsed;
		float fireRate;
	};
}
