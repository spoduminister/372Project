#include "physics.h"
#include <math.h>
//should move by one physics step

float vec_dot(vec_t a, vec_t b)
{
    float product_sum = 0.0f;

    for(int d = 0; d < dims; d++)
    {
        product_sum += a.v[d] * b.v[d];
    }
    return product_sum;
}
vec_t vec_sub(vec_t a, vec_t b)
{
    vec_t diff;

    for(int d = 0; d < dims; d++)
    {
        diff.v[d] = a.v[d] - b.v[d];
    }
    return diff;
}
vec_t vec_add(vec_t a, vec_t b)
{
    vec_t sum;

    for(int d = 0; d < dims; d++)
    {
        sum.v[d] = a.v[d] + b.v[d];
    }
    return sum;
}

void physics_step(Particle * particle_list, int numParticles, Box * Boxes, float timestep)
{
    while(timestep > 0)
    {      
        //TODO parallel find collision time step
        float collision_step = timestep;
        
        float tempcoll = 0;
        float dist; 
        int index[2];


	// UNCOMMENT FOR PARALLEL	
	#pragma acc parallel loop
        for (int i = 0; i < numParticles; i++) {
            int pnum = 0;
            vec_t min;
            vec_t max;

            for(int j = 0; j < dims; j++)
		    {
			    max.v[j] = fmax(particle_list[i].pos.v[j], particle_list[i].pos.v[j] + (particle_list[i].vel.v[j]*timestep)) + particle_list[i].radius;
			    min.v[j] = fmin(particle_list[i].pos.v[j], particle_list[i].pos.v[j] + (particle_list[i].vel.v[j]*timestep)) - particle_list[i].radius;
		    }
            int* colls = get_within_bounds(Boxes, 0, min, max, &pnum);
            if (colls != NULL) {

                for (int j = 0; j < pnum; j++) {
                    if (colls[j] >= i) continue;
                    
                    float squareR = pow((particle_list[i].radius + particle_list[colls[j]].radius),2); // Sum of particle radius' squared
                    vec_t posdiff = vec_sub(particle_list[i].pos, particle_list[colls[j]].pos); // vector difference in particle positions
                    vec_t veldiff = vec_sub(particle_list[i].vel, particle_list[colls[j]].vel); // vector difference in particle velocities
                    
                    float pos_dot = vec_dot(posdiff,posdiff);
                    float pos_vel_dot = vec_dot(posdiff,veldiff);
                    float vel_dot = vec_dot(veldiff,veldiff);
                    
                    float t = pow((pos_vel_dot/2.0),2.0) - (vel_dot * (pos_dot - squareR));
                    
                    if (t < 0)continue;
                    
                    t -=  pos_vel_dot;
                    t /= vel_dot;
                    if (t < collision_step) {
                        index[0] = i;
                        index[1] = colls[j];
                        collision_step = t;
                    }
                    
                }
            }
            
        }

        //step collision timestep
        //TODO parallel step collision time step
	// UNCOMMENT FOR PARALLEL
	#pragma acc parallel loop
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
