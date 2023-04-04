#pragma once
#include <list>
#include <queue>
#include <GLEW/glew.h>

#include "Transform.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "AppTime.h"
#include "CustomRandom.h"

#include <memory>

static std::vector<glm::vec3> quad_pos = {
	// Front face
	glm::vec3(-0.5f, -0.5f, 0.0f),   // 0 Bot Left
	glm::vec3(0.5f, -0.5f, 0.0f),   // 1 Bot Right
	glm::vec3(0.5f,  0.5f, 0.0f),   // 2 Top Right
	glm::vec3(-0.5f,  0.5f, 0.0f),   // 3 Top Left
};

static glm::vec3 quad_normal = glm::vec3(0.0f, 0.0f, 1.0f);

extern Camera camera;

struct Particle
{

public:
	bool enabled = false;

	Transform transform;
	float life_time = 5.0f;
	glm::vec3 velocity = glm::vec3(CustomRandom::GetInstance().RandomSphere());

	//glm::vec4 color_begin;
	glm::vec4 color_end = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta

	glm::mat4 GetModel()
	{
		glm::mat4 mat = glm::mat4(1.0f);

		mat = glm::translate(mat, transform.Position());

		mat = glm::rotate(mat, glm::radians(transform.Rotation().x), RIGHT);
		mat = glm::rotate(mat, glm::radians(transform.Rotation().y), UP);
		mat = glm::rotate(mat, glm::radians(transform.Rotation().z), FORWARD);

		mat = glm::scale(mat, transform.Scale());

		return mat;
	}
};

class ParticleSystem
{
public:
	ParticleSystem()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &quad_pos, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glGenBuffers(1, &normal_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(glm::vec3), &quad_normal, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		int to_make = 500;
		for (unsigned int i = 0; i < to_make; i++) Make();

	}

	~ParticleSystem()
	{

	}

	void Update()
	{
		if (paused) return;

		//program.Bind();

		std::list<std::shared_ptr<Particle>> to_be_deactivated;
		
		for (auto& p : active_particles)
		{
			p->life_time -= AppTime::DeltaTime();

			PRINT_LOG(p->life_time);
			if (p->life_time <= 0.0f) to_be_deactivated.push_front(p);
			else Draw(p);
		}

		for (auto& p : to_be_deactivated) Enqueue(p);
	}

	void Reset() 
	{
		for (std::shared_ptr<Particle> p : active_particles)
		{
			Enqueue(p);
		}
	}

	void Emit()
	{
		elapsed_time += AppTime::DeltaTime();

		//if ((unsigned int)elapsed_time % 3 == 0)
		{
			Dequeu();
		}
	}

	void Pause() { paused = true; }
	void Play() { paused = false; }
	unsigned int ParticlesCount() { return ActiveParticlesCount() + InactiveParticlesCount(); }
	unsigned int ActiveParticlesCount() { return active_particles.size(); }
	unsigned int InactiveParticlesCount() { return inactive_particles.size(); }


private:
	void Make()
	{
		inactive_particles.push(std::make_shared<Particle>());
	}

	// Becomes inactive
	void Enqueue(std::shared_ptr<Particle> p)
	{
		p->enabled = false;

		inactive_particles.push(p);
		active_particles.remove(p);
	}

	// Becomes active
	std::shared_ptr<Particle> Dequeu()
	{
		if (inactive_particles.empty())
		{
			Make();
		}

		auto& p = inactive_particles.front();
		active_particles.push_back(p);
		inactive_particles.pop();

		p->enabled = true;
		p->life_time = 5.0f;
		return p;
	}

	void Draw(std::shared_ptr<Particle> p)
	{
		p->transform.Translate(p->velocity * gravity * AppTime::DeltaTime());

		//Shader stuff
		//program.SetPVM(camera.GetProjection(), camera.GetView(), p->GetModel());

		

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	std::list<std::shared_ptr<Particle>> active_particles;
	std::queue<std::shared_ptr<Particle>> inactive_particles;

	ShaderProgram program;

	double elapsed_time = 0.0f;
	bool paused = false;

	unsigned int vao = -1;
	unsigned int vbo = -1;
	unsigned int normal_vbo = -1;

	const float gravity = 9.8f;
};

