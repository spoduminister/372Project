#include "physics.h"

void physics_step(Particle * particle_list, int numParticles, Box * Boxes, float timestep)
{
    while(timestep > 0)
    {      
        //find new timestep
        //TODO parallel find time step
        float collision_step = timestep;
        
        //step new timestep
        //TODO parallel step time step
        for(int i = 0; i < numParticles; i++)
        {
            for(int j = 0; j < dims; j++)
            {
                particle_list[i].pos.v[j] += particle_list[i].vel.v[j]* collision_step;
            }
        }
        
        //TODO actually resolve collisions
        //resolve collisions
        
        
        timestep -= collision_step;
    }
	
    return;
}
