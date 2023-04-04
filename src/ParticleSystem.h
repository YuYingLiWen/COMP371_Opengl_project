#pragma once
#include <list>
#include <queue>
#include <vector>
#include <GLEW/glew.h>

#include "Transform.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "AppTime.h"
#include "CustomRandom.h"
#include "YuMath.h"


class Particle 
{
public:
	bool enabled = false;

	Transform transform;
	float life_time_left = 5.0f;
	glm::vec3 direction = glm::vec3(CustomRandom::GetInstance().RandomSphere());

	glm::vec4 color_begin = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
	glm::vec4 color_end = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green

	Particle(glm::vec4 color_begin, glm::vec4 color_end, glm::vec3 direction);

	glm::mat4 GetModel();
};

struct ParticleData
{
public:
	float life_time = 5.0f;
	glm::vec3 scale;
	glm::vec3 direction;

	glm::vec4 color_begin = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
	glm::vec4 color_end = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
};

class ParticleSystem : public SceneObject
{
public:
	ParticleSystem(ParticleData data);

	ParticleSystem(float rate, ParticleData data);

	ParticleSystem(glm::vec3 position, ParticleData data);

	ParticleSystem(int emit_count, float rate, glm::vec3 position, glm::vec3 spawn_area, ParticleData data);

	~ParticleSystem();

	void Update();

	void Reset();



	void Pause();
	void Play();
	unsigned int ParticlesCount();
	unsigned int ActiveParticlesCount();
	unsigned int InactiveParticlesCount();


private:
	void Emit();

	void Awake();
	void Make();

	// Becomes inactive
	void Enqueue(Particle* p);

	// Becomes active
	Particle* Dequeu();

	void Draw(Particle* p);

	void ResetParticle(Particle* p);

private:
	ParticleData data;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 spawn_area = glm::vec3(0.0f);


	std::vector<Particle*> to_be_deactivated_buffer;

	std::list<Particle*> active_particles;
	std::queue<Particle*> inactive_particles;

	ShaderProgram program;

	bool paused = true;

	const float speed = 9.8f;

	float rate = 1;
	int emit_count = 1;

	float time_elapsed = 0.0f;

	CustomRandom rand = CustomRandom::GetInstance();
};

