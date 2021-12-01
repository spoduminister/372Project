#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "particle.h"
#include "BoundBox.h"
#include "physics.h"
#include "draw2D.h"
#include "bitmap.h"

int width = 1000;
int height = 1000;
int running = 1;

int main(){
    srand(time(NULL));
    int numParticles = 30;
    clock_t tick;
    clock_t tock;
    float timestep = 1.0f;
    //TESTING LOOP
    unsigned int * PixelBuffer = (unsigned int *) malloc(sizeof(unsigned int) * width * height);
    char title[64];
    
    
    tick = clock();
    
    Particle * particle_list = init_particles(numParticles,(vec_t){0,0},(vec_t){width,height});
    int * sub_particles = (int *) malloc(sizeof(int) * numParticles);
    for(int i = 0; i < numParticles; i++)sub_particles[i] = i;
    Box * Boxes = build_boxes(particle_list, numParticles, sub_particles,0,timestep);
    tock = clock();
    printf("BUILD TIME:%ld\n", tock - tick);
    tick = clock();
    //physics_step(particle_list, numParticles,Boxes, timestep);
    tock = clock();
    printf("PHYSICS TIME:%ld\n", tock - tick);
    tick = clock();
    draw_image(PixelBuffer, width, height, particle_list, numParticles, Boxes);
    sprintf(title, "./Output/Frame_%d.bmp", numParticles);
    save_bmp((unsigned char *)PixelBuffer, width, height, title);
	tock = clock();
    free(Boxes);
    free(particle_list);
    
    printf("DrawTime %ld\n", tock - tick);
    
    free(PixelBuffer);
    /*
    Particle * particle_list = init_particles(numParticles);
    //end condition will be a stop button in the gui 
    while (running == 1) {
        int * sub_particles = (int *) malloc(sizeof(int) * numParticles);
        for(int i = 0; i < numParticles; i++)sub_particles[i] = i;
        Box * Boxes = build_boxes(particle_list, numParticles, sub_particles,0,timestep);
        
        //render();
        
        physics_step(particle_list, i,Boxes, timestep);
        
        free(Boxes);
        
    }
    */

}
