#include "physics.h"

//should move by one physics step
void physics_step(Particle * particle_list, int numParticles, Box * Boxes, float timestep)
{
    while(timestep > 0)
    {      
        //TODO find time untill next collision event
        //TODO parallel find collision time step
        float collision_step = timestep;
        
        //step collision timestep
        //TODO parallel step collision time step
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
