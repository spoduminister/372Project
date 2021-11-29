#include <math.h>
#include <stdlib.h>

#define dims 2
enum AXIS {X, Y};


typedef struct{
    float pos[dims];
    float vel[dims];
	float radius;
}Particle;

typedef struct{
    int numBoxes;
    int P_index;
    int num_Particles;
    int children[(1<<dims)];
    float max_pos[dims];
    float min_pos[dims];
    float mid_pos[dims];
}Box;


Particle * init_particles(int numParticles);

Box * build_boxes(Particle * particle_list, int numParticles, int sub_particles[numParticles]);

void physics_step(Particle * particle_list, int numParticles);