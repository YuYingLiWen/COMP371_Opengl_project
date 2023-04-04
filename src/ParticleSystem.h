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

extern std::vector<unsigned int> cube_indexes;
extern std::vector<glm::vec3> cube_pos;

static glm::vec3 quad_normal = glm::vec3(0.0f, 0.0f, 1.0f);

extern Camera camera;

struct Particle 
{
public:
	bool enabled = false;

	Transform transform;
	float life_time_left = 5.0f;
	float life_time = 5.0f;
	glm::vec3 direction = glm::vec3(CustomRandom::GetInstance().RandomSphere());

	glm::vec4 color_begin = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
	glm::vec4 color_end = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green

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

class ParticleSystem : public SceneObject
{
public:
	ParticleSystem()
		:SceneObject(&cube_pos, &cube_indexes)
	{
		program.Attach("res\\shaders\\color_vs.shader", "res\\shaders\\color_fs.shader");

		int to_make = 500;
		for (unsigned int i = 0; i < to_make; i++) Make();
	}

	~ParticleSystem()
	{
		for (auto& p : active_particles) delete p;
		while (!inactive_particles.empty())
		{
			delete inactive_particles.front();
			inactive_particles.pop();
		}
	}

	void Update()
	{
		if (paused) return;

		to_be_deactivated_buffer.clear();

		program.Bind();
		
		SceneObject::Bind();
		
		for (auto& p : active_particles)
		{
			p->life_time_left -= AppTime::DeltaTime();

			if (p->life_time_left <= 0.0f) to_be_deactivated_buffer.push_back(p);
			else Draw(p);
		}

		for (auto& p : to_be_deactivated_buffer) Enqueue(p);
	}

	void Reset() 
	{
		for (Particle* p : active_particles)
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
		inactive_particles.push(new Particle());
	}

	// Becomes inactive
	void Enqueue(Particle* p)
	{
		p->enabled = false;

		inactive_particles.push(p);
		active_particles.remove(p);
	}

	// Becomes active
	Particle* Dequeu()
	{
		if (inactive_particles.empty())
		{
			Make();
		}

		auto& p = inactive_particles.front();
		active_particles.push_back(p);
		inactive_particles.pop();

		ResetParticle(p);

		return p;
	}

	void Draw(Particle* p)
	{
		//Transforms stuff
		//p->transform.SetScale(glm::vec3(1.0f));

		//Shader stuff
		//Color stuff
		program.SetUniform4f("u_color_begin", p->color_begin);
		program.SetUniform4f("u_color_end", p->color_end);
		program.SetUniformFloat("u_t", p->life_time_left / p->life_time);

		glm::mat4 lookat = p->transform.LookAt(camera.GetTransform().Position());

		//p->direction = p->direction * gravity;

		p->transform.Translate(p->direction * gravity * AppTime::DeltaTime());


		program.SetPVM(camera.GetProjection(), camera.GetView(), p->GetModel());



		
		glDrawElements(GL_TRIANGLES, cube_indexes.size(), GL_UNSIGNED_INT, nullptr);
	}

	void ResetParticle(Particle* p) 
	{
		p->enabled = true;
		p->life_time_left = p->life_time;
		p->transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//p->transform.SetPosition(glm::vec3(CustomRandom::GetInstance().Generate(2.0f) * 100.0f, 0.0f, CustomRandom::GetInstance().Generate(2.0f) * 100.0f));
	}

private:
	std::vector<Particle*> to_be_deactivated_buffer;

	std::list<Particle*> active_particles;
	std::queue<Particle*> inactive_particles;

	ShaderProgram program;

	double elapsed_time = 0.0f;
	bool paused = false;

	const float gravity = 9.8f;
};

