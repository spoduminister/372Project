#include "particle.h"

Particle * init_particles(int numParticles, vec_t min_pos, vec_t max_pos)
{
	Particle * particle_list = (Particle *) malloc(sizeof(Particle) * numParticles);
	for(int i = 0; i < numParticles; i++)
	{
		for(int j = 0; j < dims; j++)
		{
			particle_list[i].pos.v[j] = (float)rand()/(float)(RAND_MAX) * (max_pos.v[j] - min_pos.v[j]) + min_pos.v[j];
			particle_list[i].vel.v[j] = 0.0f;//(float)rand()/(float)(RAND_MAX) * 20.0f - 10.0f;
		}
		particle_list[i].radius = (float)rand()/(float)(RAND_MAX) * 50.0f + 25.0f;
	}
	return particle_list;
}



