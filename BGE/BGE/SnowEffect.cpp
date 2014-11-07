#include "SnowEffect.h"
#include "Utils.h"

using namespace BGE;

SnowEffect::SnowEffect(bool hasTransform) :ParticleEffect(hasTransform)
{
	textureName = "snowflake";
}


SnowEffect::~SnowEffect(void)
{
}

bool SnowEffect::Initialise()
{
	for (int i = 0 ; i < 1000 ; i ++)
	{
		Particle p;
		InitParticle(p);
		p.position.y = RandomClamped(0, 1000);
		particles.push_back(p);
	}
	return ParticleEffect::Initialise();
}

void SnowEffect::InitParticle(Particle & p)
{
	float range = 1000.0f;
	float radius = 0.1f;
	// Give the particle a random X and Z, but fixed Y
	p.position = glm::vec3(RandomClamped(-range, range), range, RandomClamped(-range, range));
	// Give the particle a random down velocity
	p.velocity = glm::vec3(RandomClamped(-radius, radius), RandomClamped(- radius, -radius * 5.0f), RandomClamped(-radius, radius));
	
	// White colour
	p.diffuse = glm::vec4(1,1,1,1);
	p.age = 0;
	p.alive = true;
	p.size = RandomClamped(500, 600);
}

void SnowEffect::UpdateParticle(Particle & p)
{
	static glm::vec3 gravity = glm::vec3(0, -9.8, 0);

	p.velocity += gravity * Time::deltaTime;
	p.position += p.velocity * Time::deltaTime;

	// Fade the alpha as we approach the ground
	float fadeHeight = 20;

	p.diffuse.a = glm::clamp<float>(p.position.y / fadeHeight, 0.0f, 1.0f);
	if (p.position.y < 0)
	{
		InitParticle(p);
	}
}
