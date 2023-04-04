#include "ParticleSystem.h"

extern std::vector<unsigned int> cube_indexes;
extern std::vector<glm::vec3> cube_pos;

static glm::vec3 quad_normal = glm::vec3(0.0f, 0.0f, 1.0f);

extern Camera camera;

// Particle

Particle::Particle(glm::vec4 color_begin, glm::vec4 color_end, glm::vec3 direction)
	:color_begin(color_begin), color_end(color_end), direction(direction) {}

glm::mat4 Particle::GetModel()
{
	glm::mat4 mat = glm::mat4(1.0f);

	mat = glm::translate(mat, transform.Position());

	mat = glm::rotate(mat, glm::radians(transform.Rotation().x), RIGHT);
	mat = glm::rotate(mat, glm::radians(transform.Rotation().y), UP);
	mat = glm::rotate(mat, glm::radians(transform.Rotation().z), FORWARD);

	mat = glm::scale(mat, transform.Scale());
	return mat;
}



///Particle System

ParticleSystem::ParticleSystem(ParticleData data)
	: rate(1), data(data),
	SceneObject(&cube_pos, &cube_indexes)
{
	Awake();
}

ParticleSystem::ParticleSystem(float rate, ParticleData data)
	: rate(rate), data(data),
	SceneObject(&cube_pos, &cube_indexes)
{
	Awake();
}


ParticleSystem::ParticleSystem(glm::vec3 position, ParticleData data)
	:rate(1), position(position), data(data),
	SceneObject(&cube_pos, &cube_indexes)
{
	Awake();
}

ParticleSystem::ParticleSystem(int emit_count, float rate, glm::vec3 position, glm::vec3 spawn_area, ParticleData data)
	:emit_count(emit_count), rate(rate), position(position), spawn_area(spawn_area), data(data),
	SceneObject(&cube_pos, &cube_indexes)
{
	Awake();
}

ParticleSystem::~ParticleSystem()
{
	for (auto& p : active_particles) delete p;
	while (!inactive_particles.empty())
	{
		delete inactive_particles.front();
		inactive_particles.pop();
	}
}

void ParticleSystem::Update()
{
	Emit();

	to_be_deactivated_buffer.clear();

	program.Bind();
	SceneObject::Bind();

	for (auto& p : active_particles)
	{
		if(!paused) p->life_time_left -= AppTime::DeltaTime();

		if (p->life_time_left <= 0.0f) to_be_deactivated_buffer.push_back(p);
		else
		{
			Draw(p);
		}
	}

	for (auto& p : to_be_deactivated_buffer) Enqueue(p);
}

void ParticleSystem::Reset()
{
	for (Particle* p : active_particles)
	{
		Enqueue(p);
	}
}

void ParticleSystem::Pause() { PRINT_LOG("Pause"); paused = true; }

void ParticleSystem::Play() { PRINT_LOG("Play"); paused = false; }

unsigned int ParticleSystem::ParticlesCount() { return ActiveParticlesCount() + InactiveParticlesCount(); }

unsigned int ParticleSystem::ActiveParticlesCount() { return active_particles.size(); }

unsigned int ParticleSystem::InactiveParticlesCount() { return inactive_particles.size(); }

void ParticleSystem::Emit()
{
	if (paused) return;

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

void ParticleSystem::Awake()
{
	program.Attach("res\\shaders\\color_vs.shader", "res\\shaders\\color_fs.shader");

	int to_make = 500;
	for (unsigned int i = 0; i < to_make; i++) Make();
}

void ParticleSystem::Make()
{
	inactive_particles.push(new Particle(data.color_begin, data.color_end, data.direction));
}

void ParticleSystem::Enqueue(Particle* p)
{
	p->enabled = false;

	inactive_particles.push(p);
	active_particles.remove(p);
}

Particle* ParticleSystem::Dequeu()
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

void ParticleSystem::Draw(Particle* p)
{
	//Transforms stuff
	//p->transform.SetScale(glm::vec3(1.0f, 3.0f, 0.5f));

	//Shader stuff
	//Color stuff
	program.SetUniform4f("u_color_begin", p->color_begin);
	program.SetUniform4f("u_color_end", p->color_end);
	program.SetUniformFloat("u_t", p->life_time_left / data.life_time);

	glm::mat4 lookat = p->transform.LookAt(camera.GetTransform().Position());


	if(!paused) p->transform.Translate(p->direction * speed * AppTime::DeltaTime());


	program.SetPVM(camera.GetProjection(), camera.GetView(), p->GetModel());

	glDrawElements(GL_TRIANGLES, cube_indexes.size(), GL_UNSIGNED_INT, nullptr);
}

void ParticleSystem::ResetParticle(Particle* p)
{
	glm::vec3 spawn_position = glm::vec3(
		spawn_area.x * rand.Generate(2.0f),
		spawn_area.y * rand.Generate(2.0f),
		spawn_area.z * rand.Generate(2.0f));
	p->transform.SetPosition(position + spawn_position);

	p->enabled = true;
	p->life_time_left = data.life_time;
	p->transform.SetScale(data.scale);
	//p->transform.SetPosition(glm::vec3(CustomRandom::GetInstance().Generate(2.0f) * 100.0f, 0.0f, CustomRandom::GetInstance().Generate(2.0f) * 100.0f));
}
