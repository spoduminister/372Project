#include "physics.h"
#include <math.h>
//should move by one physics step
void physics_step(Particle * particle_list, int numParticles, Box * Boxes, float timestep)
{
    while(timestep > 0)
    {      
        //TODO parallel find collision time step
        float collision_step = timestep;
        
        float tempcoll = 0;
        float dist; 
        int index[2];

        for (int i = 0; i < numParticles; i++) {
            int pnum = 0;
            vec_t dest;
            dest = particle_list[i].pos;
            dest[0] += particle_list[i].vel.v[0];
            dest[1] += particle_list[i].vel.v[1];
            int* colls = get_within_bounds(Boxes, 0, particle_list[i].pos, dest, *pnum);
            if (colls != NULL) {
                for (int j = 0; j < pnum; j++) {
                    float tempdist = (float)sqrt(((pow((double)(particle_list[colls[0]].pos.v[0] - particle_list[i].pos.v[0]), 2.0)) + (pow((double)(particle_list[colls[0]].pos.v[1] - particle_list[i].pos.v[1]), 2.0))));
                    float vel = (float)sqrt(pow((double)particle_list[i].vel.v[0], 2) + pow((double)particle_list[i].vel.v[1], 2));
                    tempcoll = tempdist / vel;
                    if (tempcoll < collision_step) {
                        dist = tempdist;
                        index[0] = i;
                        index[1] = colls[0];
                        collision_step = tempcoll;
                    }
                }
            }
            
        }

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
