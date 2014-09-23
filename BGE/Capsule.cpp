#include "Capsule.h"
#include "Model.h"
#include "Content.h"

using namespace BGE;

Capsule::Capsule(float radius, float height):GameComponent(true)
{	
	transform->diffuse = glm::vec3(RandomFloat(),RandomFloat(),RandomFloat());
	transform->ambient = glm::vec3(0, 0, 0);
	transform->specular = glm::vec3(1, 1, 1);
	transform->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	tag = "capsule";
	
	transform->scale = glm::vec3(radius, height, radius);
}

bool Capsule::Initialise()
{
	if (!initialised)
	{
		std::shared_ptr<Model> model(Content::LoadModel("capsule"));
		model->drawMode = Model::draw_modes::single_material;
		Attach(model);
		initialised = true;
		return GameComponent::Initialise();
	}
	else
	{
		return true;
	}
}

void Capsule::Draw()
{
	GameComponent::Draw();
}

Capsule::~Capsule(void)
{
}
