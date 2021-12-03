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
        if (collision_step < timestep) {
            float v1 = (float)sqrt(pow((double)particle_list[index[0]].vel.v[0], 2) + pow((double)particle_list[index[0]].vel.v[1], 2));
            float v2 = (float)sqrt(pow((double)particle_list[index[1]].vel.v[0], 2) + pow((double)particle_list[index[1]].vel.v[1], 2));
            //final x direction of particle 1; taking a random direction and making it a unit vector
            float scalar = (float)rand() / (float)(RAND_MAX);
            float fvx1_dir = (scalar * -1) * particle_list[index[0]].vel.v[0] / (particle_list[index[0]].vel.v[0] + particle_list[index[0]].vel.v[1]);
            float fvy1_dir = ((1-scalar) * -1) * particle_list[index[0]].vel.v[1] / (particle_list[index[0]].vel.v[0] + particle_list[index[0]].vel.v[1]);
            float fvx2_dir = (scalar * -1) * particle_list[index[1]].vel.v[0] / (particle_list[index[0]].vel.v[0] + particle_list[index[0]].vel.v[1]);
            float fvy2_dir = ((1 - scalar) * -1) * particle_list[index[1]].vel.v[1] / (particle_list[index[0]].vel.v[0] + particle_list[index[0]].vel.v[1]);
            particle_list[index[0]].vel.v[0] = fvx1_dir * v1;
            particle_list[index[0]].vel.v[1] = fvy1_dir * v1;
            particle_list[index[1]].vel.v[0] = fvx2_dir * v2;
            particle_list[index[1]].vel.v[1] = fvy2_dir * v2;
        }
        
        timestep -= collision_step;
    }
	
    return;
}
