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

static std::vector<glm::vec3> quad_pos = {
		// Front face
		glm::vec3(-0.5f, -0.5f, 0.5f),   // 0 Bot Left
		glm::vec3(0.5f, -0.5f, 0.5f),   // 1 Bot Right
		glm::vec3(0.5f,  0.5f, 0.5f),   // 2 Top Right
		glm::vec3(-0.5f,  0.5f, 0.5f),   // 3 Top Left

		// Back face
		glm::vec3(-0.5f, -0.5f, -0.5f),  // 4 Bot Left
		glm::vec3(0.5f, -0.5f, -0.5f),  // 5 Bot Right
		glm::vec3(0.5f,  0.5f, -0.5f),  // 6 Top Right
		glm::vec3(-0.5f,  0.5f, -0.5f),  // 7 Top Left
};

static std::vector<unsigned int> indexes = {
	// front face
	0, 1, 2,
	0, 2, 3,

	//Back face
	4,5,6,
	4,6,7,

	// Top face
	3,2,6,
	3,6,7,

	// Bottom face
	0,1,5,
	0,5,4,

	//Left face
	1,5,6,
	1,6,2,

	//Right face
	4,0,3,
	4,3,7
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
		Awake();

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

		glDeleteBuffers(1, &quad_vbo);
		glDeleteBuffers(1, &normal_vbo);
		glDeleteBuffers(1, &ibo);

		glDeleteVertexArrays(1, &vao);
	}

	void Update()
	{
		if (paused) return;

		to_be_deactivated_buffer.clear();

		program.Bind();
		//SceneObject::Bind();
		glBindVertexArray(vao);
		
		for (auto& p : active_particles)
		{
			p->life_time -= AppTime::DeltaTime();

			if (p->life_time <= 0.0f) to_be_deactivated_buffer.push_back(p);
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
		//Transforms
		//p->transform.SetScale(glm::vec3(1.0f));

		p->transform.Translate(p->velocity * gravity * AppTime::DeltaTime());

		//Shader stuff

		program.SetPVM(camera.GetProjection(), camera.GetView(), p->GetModel());
		program.SetUniform4f("u_color", p->color_end);

		
		glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, nullptr);
	}


	void Awake()
	{
		glGenVertexArrays(1, &vao); 
		glBindVertexArray(vao); 

		glGenBuffers( 1, &quad_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
		glBufferData(GL_ARRAY_BUFFER, quad_pos.size() * sizeof(glm::vec3), quad_pos.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glGenBuffers(1, &normal_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(glm::vec3), &quad_normal, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned int), indexes.data(), GL_STATIC_DRAW);
	}

	void ResetParticle(Particle* p) 
	{
		p->enabled = true;
		p->life_time = 5.0f;
		p->transform.SetPosition(glm::vec3(0.0f));
	}

private:
	std::vector<Particle*> to_be_deactivated_buffer;

	std::list<Particle*> active_particles;
	std::queue<Particle*> inactive_particles;

	ShaderProgram program;

	double elapsed_time = 0.0f;
	bool paused = false;

	const float gravity = 9.8f;

	unsigned int vao = -1;
	unsigned int quad_vbo = -1;
	unsigned int normal_vbo = -1;
	unsigned int ibo = -1;
};

