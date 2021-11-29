#include "particle.h"
#include <string.h>

Particle * init_particles(int numParticles)
{
	Particle * particle_list = (Particle *) malloc(sizeof(Particle) * numParticles);
	for(int i = 0; i < numParticles; i++)
	{
		for(int j = 0; j < dims; j++)
		{
			particle_list[i].pos[j] = (rand() % 100000)/ 100.0f;
			particle_list[i].vel[j] = (rand() % 100000)/ 100.0f;
		}
		particle_list[i].radius = (rand() % 100000)/ 100.0f;
	}
	return particle_list;
}

Box * build_boxes(Particle * particle_list, int numParticles, int sub_particles[numParticles])
{
	Box * parent = (Box *) malloc(sizeof(Box));
	parent->numBoxes = 1;
	
	if(numParticles <1)
	{
		parent->P_index = -1;
		parent->num_Particles = numParticles;
		free(parent);
		return NULL;
		
	}
	else if(numParticles == 1)
	{
		parent->P_index = -1;
		parent->num_Particles = numParticles;
		
		for(int i = 0; i < numParticles; i++)
		{
			if(sub_particles[i])
				parent->P_index = i;
		}
		if (parent->P_index == -1)return NULL;
		
		for(int j = 0; j < dims; j++)
		{
			parent->max_pos[j] = fmax(particle_list[parent->P_index].pos[j], particle_list[parent->P_index].pos[j] + particle_list[parent->P_index].vel[j]);
			parent->min_pos[j] = fmin(particle_list[parent->P_index].pos[j], particle_list[parent->P_index].pos[j] + particle_list[parent->P_index].vel[j]);
		}
		return parent;
	}
	
	int sub_particle_list[(1<<dims)][numParticles];
	int sub_particle_count[(1<<dims)];
	
	for(int i = 0; i < numParticles; i++)
	{
		for(int j = 0; j < dims; j++)
		{
			parent->mid_pos[j] += particle_list[i].pos[j]/numParticles;
			parent->max_pos[j] += fmax(particle_list[i].pos[j],parent->max_pos[j]);
			parent->min_pos[j] += fmin(particle_list[i].pos[j],parent->min_pos[j]);
		}
	}
	
	for(int i = 0; i < numParticles; i++)
	{
		int region = 0;
		for(int j = 0; j < dims; j++)
		{
			if (particle_list[i].pos[j] > parent->mid_pos[j])region +=(1<<j);
		}
		sub_particle_list[region][i]=1;
		sub_particle_count[region]+=1;
	}
	
	for(int i = 0; i < (1<<dims); i++)
	{
		Box * child = build_boxes(particle_list, sub_particle_count[i], sub_particle_list[i]);
		parent = (Box *) realloc(parent, sizeof(Box) * (parent->numBoxes + child->numBoxes));
		memcpy(parent + parent->numBoxes, child, (sizeof(Box)*child->numBoxes));
		parent->numBoxes += child->numBoxes;
		free(child);
	}
	
	return parent;
}

void physics_step(Particle * particle_list, int numParticles)
{
	int sub_particles[numParticles];
	for(int i = 0; i < numParticles; i++)
	{
		sub_particles[i] = 1;
	}
	Box * Boxes = build_boxes(particle_list, numParticles, sub_particles);
	for(int i = 0; i < numParticles; i++)
	{
		for(int j = 0; j < dims; j++)
		{
			particle_list[i].pos[j] += particle_list[i].vel[j];
		}
	}
}