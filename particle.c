#include "particle.h"
Particle * init_particles(int numParticles)
{
	Particle * particle_list = (Particle *) malloc(sizeof(Particle) * numParticles);
	for(int i = 0; i < numParticles; i++)
	{
		for(int j = 0; j < dims; j++)
		{
			particle_list[i].pos[j] = (float)rand()/(float)(RAND_MAX) - 0.5f;
			particle_list[i].vel[j] = (float)rand()/(float)(RAND_MAX) - 0.5f;
		}
		particle_list[i].radius = (float)rand()/(float)(RAND_MAX) - 0.5f;
	}
	return particle_list;
}

Box * build_boxes(Particle * particle_list, int numParticles, int sub_particles[],int BoxOffset, float timestep)
{
	Box * parent = (Box *) malloc(sizeof(Box));
	parent->numBoxes = 1;
	for(int i = 0; i < dims; i++)
	{
		parent->max_pos[i] = NAN;
		parent->min_pos[i] = NAN;
		parent->mid_pos[i] = 0.0;
	}
	
	if(numParticles <1)
	{
		free(sub_particles);
		free(parent);
		return NULL;
		
	}
	else if(numParticles == 1)
	{
		parent->num_Particles = numParticles;
		parent->P_index = sub_particles[0];
		
		for(int j = 0; j < dims; j++)
		{
			parent->max_pos[j] = fmax(particle_list[parent->P_index].pos[j], particle_list[parent->P_index].pos[j] + (particle_list[parent->P_index].vel[j]*timestep)) + particle_list[parent->P_index].radius;
			parent->min_pos[j] = fmin(particle_list[parent->P_index].pos[j], particle_list[parent->P_index].pos[j] + (particle_list[parent->P_index].vel[j]*timestep)) - particle_list[parent->P_index].radius;
		}
		free(sub_particles);
		return parent;
	}
	
	int  * sub_particle_list[(1<<dims)];
	int sub_particle_count[(1<<dims)];
	for(int i = 0; i < (1<<dims); i++)
	{
		sub_particle_list[i] = (int *) malloc(sizeof(int));
		sub_particle_count[i] = 0;
	}
	
	for(int i = 0; i < numParticles; i++)
	{
		int index = sub_particles[i];
		for(int j = 0; j < dims; j++)
		{
			parent->mid_pos[j] = (particle_list[index].pos[j]/ ((float)numParticles)) + parent->mid_pos[j];
		}
	}
	
	for(int i = 0; i < numParticles; i++)
	{
		
		int region = 0;
		int index = sub_particles[i];
		for(int j = 0; j < dims; j++)
		{
			if (particle_list[index].pos[j] > parent->mid_pos[j])region +=(1<<j);
		}
		sub_particle_count[region]+=1;
		sub_particle_list[region] = realloc(sub_particle_list[region], sub_particle_count[region] * sizeof(int));
		sub_particle_list[region][sub_particle_count[region] - 1] = index;
	}
	
	for(int i = 0; i < (1<<dims); i++)
	{
		Box * child = build_boxes(particle_list, sub_particle_count[i], sub_particle_list[i],parent->numBoxes + BoxOffset,timestep);
		if (child != NULL)
		{
			parent = (Box *) realloc(parent, sizeof(Box) * (parent->numBoxes + child->numBoxes));
			for(int j = 0; j < dims; j++)
			{
				parent->min_pos[j] = fmin(parent->min_pos[j],child->min_pos[j]);
				parent->max_pos[j] = fmax(parent->max_pos[j],child->max_pos[j]);
			}
			memcpy(parent + parent->numBoxes, child, (sizeof(Box)*child->numBoxes));
			parent->children[i] = parent->numBoxes + BoxOffset;
			parent->numBoxes += child->numBoxes;
			free(child);
		}
		else
		{
			parent->children[i] = -1;
		}
	}
	free(sub_particles);
	return parent;
}

void physics_step(Particle * particle_list, int numParticles, float timestep)
{
	int * sub_particles = (int *) malloc(sizeof(int) * numParticles);
	for(int i = 0; i < numParticles; i++)
	{
		sub_particles[i] = i;
	}
	Box * Boxes = build_boxes(particle_list, numParticles, sub_particles,0,timestep);
	/*
	for(int i = 0; i < numParticles; i++)
	{
		for(int j = 0; j < dims; j++)
		{
			particle_list[i].pos[j] += particle_list[i].vel[j];
		}
	}
	*/
	free(Boxes);
}