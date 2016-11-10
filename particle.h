#ifndef _particle_h
#define _particle_h

#include "Vector3.h"
#include "Image.h"

typedef struct Particle_tag
{
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float life;
    Image* image;
} Particle;

typedef struct ParticleSystem_tag
{
    int maxParticles;
    Particle* particles;
} ParticleSystem;

ParticleSystem* ps_create(int maxParticles);
void ps_updateAndDraw(ParticleSystem* ps, BYTE* buffer, float dt);

#endif
