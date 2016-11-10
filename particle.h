#ifndef _particle_h
#define _particle_h

#include "Vector2.h"
#include "Image.h"

typedef struct Particle_tag
{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
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
