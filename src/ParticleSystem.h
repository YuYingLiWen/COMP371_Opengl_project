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

class Particle 
{
public:
	bool enabled = false;

	Transform transform;
	float life_time_left = 5.0f;
	glm::vec3 direction = glm::vec3(CustomRandom::GetInstance().RandomSphere());

	glm::vec4 color_begin = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
	glm::vec4 color_end = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green

	Particle(glm::vec4 color_begin, glm::vec4 color_end, glm::vec3 direction)
		:color_begin(color_begin), color_end(color_end), direction(direction) {}

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
	ParticleSystem(ParticleData data)
		: rate(1), data(data),
		SceneObject(&cube_pos, &cube_indexes)
	{
		Awake();
	}

	ParticleSystem(float rate, ParticleData data)
		: rate(rate), data(data),
		SceneObject(&cube_pos, &cube_indexes)
	{
		Awake();
	}

	ParticleSystem(glm::vec3 position, ParticleData data)
		:rate(1), position(position), data(data),
		SceneObject(&cube_pos, &cube_indexes)
	{
		Awake();
	}

	ParticleSystem(int emit_count, float rate, glm::vec3 position, glm::vec3 spawn_area, ParticleData data)
		:emit_count(emit_count), rate(rate), position(position), spawn_area(spawn_area), data(data),
		SceneObject(&cube_pos, &cube_indexes)
	{
		Awake();
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
		
		Emit();

		to_be_deactivated_buffer.clear();

		program.Bind();
		SceneObject::Bind();
		
		for (auto& p : active_particles)
		{
			p->life_time_left -= AppTime::DeltaTime();

			if (p->life_time_left <= 0.0f) to_be_deactivated_buffer.push_back(p);
			else
			{	
				Draw(p);
			}
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



	void Pause() { paused = true; }
	void Play() { paused = false; }
	unsigned int ParticlesCount() { return ActiveParticlesCount() + InactiveParticlesCount(); }
	unsigned int ActiveParticlesCount() { return active_particles.size(); }
	unsigned int InactiveParticlesCount() { return inactive_particles.size(); }


private:
	void Emit()
	{
		time_elapsed -= AppTime::DeltaTime();

		if (time_elapsed <= 0.0f)
		{
			for (float i = 0; i < emit_count; i++)
			{
				Dequeu();
			}

			time_elapsed = rate;
		}
	}

	void Awake()
	{
		program.Attach("res\\shaders\\color_vs.shader", "res\\shaders\\color_fs.shader");

		int to_make = 500;
		for (unsigned int i = 0; i < to_make; i++) Make();
	}
	void Make()
	{
		inactive_particles.push(new Particle(data.color_begin, data.color_end, data.direction));
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
		//p->transform.SetScale(glm::vec3(1.0f, 3.0f, 0.5f));

		//Shader stuff
		//Color stuff
		program.SetUniform4f("u_color_begin", p->color_begin);
		program.SetUniform4f("u_color_end", p->color_end);
		program.SetUniformFloat("u_t", p->life_time_left / data.life_time);

		glm::mat4 lookat = p->transform.LookAt(camera.GetTransform().Position());

		//p->direction = p->direction * gravity;

		p->transform.Translate(p->direction * gravity * AppTime::DeltaTime());


		program.SetPVM(camera.GetProjection(), camera.GetView(), p->GetModel());

		glDrawElements(GL_TRIANGLES, cube_indexes.size(), GL_UNSIGNED_INT, nullptr);
	}

	void ResetParticle(Particle* p) 
	{
		p->enabled = true;
		p->life_time_left = data.life_time;
		p->transform.SetPosition(position + spawn_area * CustomRandom::GetInstance().Generate(2.0f));
		p->transform.SetScale(data.scale);
		//p->transform.SetPosition(glm::vec3(CustomRandom::GetInstance().Generate(2.0f) * 100.0f, 0.0f, CustomRandom::GetInstance().Generate(2.0f) * 100.0f));
	}

private:
	ParticleData data;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 spawn_area = glm::vec3(0.0f);


	std::vector<Particle*> to_be_deactivated_buffer;

	std::list<Particle*> active_particles;
	std::queue<Particle*> inactive_particles;

	ShaderProgram program;

	bool paused = true;

	const float gravity = 9.8f;

	float rate = 1;
	int emit_count = 1;

	float time_elapsed = 0.0f;
};

