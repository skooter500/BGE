#include "FountainEffect.h"
#include "Utils.h"

using namespace BGE;

FountainEffect::FountainEffect(bool hasTransform) :numParticles(1000), ParticleEffect(hasTransform)
{
	transform->diffuse = glm::vec3(1, 1, 1);
}

FountainEffect::FountainEffect(int numParticles, bool hasTransform) : ParticleEffect(hasTransform)
{
	this->numParticles = numParticles;	
}

FountainEffect::~FountainEffect(void)
{
}

bool FountainEffect::Initialise()
{
	for (int i = 0 ; i < numParticles ; i ++)
	{
		Particle p;
		InitParticle(p);
		particles.push_back(p);
	}
	return ParticleEffect::Initialise();
}

void FountainEffect::InitParticle(Particle & p)
{
	float radius = 0.25f;
	p.position = transform->position; // Start the particle at the ParticleSystem position
	// Give the particle a clamped random velocity
	p.velocity = glm::vec3(RandomClamped(-radius, radius), RandomClamped(), RandomClamped(-radius, radius)) * 10.0f;
	
	// Make the velocity always point upwards
	p.velocity.y = glm::abs<float>(p.velocity.y);
	
	// Give the particle a random colour
	float colourScale = RandomClamped(0, 1);
	p.diffuse.r = transform->diffuse.r * colourScale;
	p.diffuse.g = transform->diffuse.g * colourScale;
	p.diffuse.b = transform->diffuse.b * colourScale;
	p.age = 0;
	p.alive = true;
	// Random sizes also
	p.size = RandomClamped(10, 50);
}

void FountainEffect::UpdateParticle(Particle & p)
{
	static glm::vec3 gravity = glm::vec3(0, -9.8, 0);
	
	p.velocity += gravity * Time::deltaTime;
	p.position += p.velocity * Time::deltaTime;

	// Fade the alpha as we approach the ground
	float fadeHeight = 5;
	
	p.diffuse.a = glm::clamp<float>(p.position.y / fadeHeight, 0.0f, 1.0f);
	if (p.position.y < 0)
	{
		InitParticle(p);
	}
}

void FountainEffect::Update()
{
	ParticleEffect::Update();
}

