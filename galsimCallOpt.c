#include "graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/* Simulate galaxies */
/*Sources: https://stackoverflow.com/questions/46615620/c-structures-initializing-using-for-loop,   */

/*Main*/
int N;
char * infile;
int stp;
float delta;
int graph;
int quit;
double G;

typedef struct part{		/*Define class*/
    double Posx, Posy, Mass, Velx, Vely, Bright;
}particle;


/*Graph*/
const double epsil = 1.0/1000.0;
const float circleRadius=0.0025, circleColor=0;
const int windowWidth=1000;
float L=1, W=1;

/*Force calc*/
typedef struct fxy{
    double fx, fy;
}F;

struct fxy f;
double Fx;
double Fy;
double distx;
double disty;
double rad; 
double sumx, sumy;
struct fxy ftot;

/*Pos update*/
double ax, ay, Vx, Vy, px, py;









void force_calc(struct part *particles, double *fx, double *fy){           /*Takes two particles as input and returns the force in x and y direction*/ 
                  

    /*printf("Rad %f\n", rad);
    printf("Disty %f\n", disty);*/
    /*printf("Pow %f\n", pow((rad+epsil),3));
    printf("Pow %f\n", (rad+epsil)*(rad+epsil)*(rad+epsil));*/

    for (int i = 0; i < N; i++){    
        for (int j = i+1; j < N; j++){
            if (i != j){          
                distx = (particles + i)->Posx -(particles + j)->Posx;                                   /*Force calculations for i, j*/
                disty = (particles + i)->Posy -(particles + j)->Posy;
                rad = sqrt(distx*distx + disty*disty);
                Fx = -G*(particles + i)->Mass * (particles + j)->Mass/pow((rad+epsil),3) * distx;
                Fy = -G*(particles + i)->Mass * (particles + j)->Mass/pow((rad+epsil),3) * disty;
                                        /*Make Fx and Fy into 2D-matrix sum later*/
                *(fx + i) += Fx;
                *(fy + i) += Fy;
                *(fx + j) += -Fx;
                *(fy + j) += -Fy;
            }
        }
    }
}


void pos_update(struct part *particles, double *fx, double *fy){             /*Takes pointer to particle and updates its position given acceleration in x and y plane*/
    for (int i = 0; i < N; i++){
        ax = *(fx + i)/(particles + i)->Mass;
        ay = *(fy + i)/(particles + i)->Mass;
        Vx = (particles + i)->Velx;
        Vy = (particles + i)->Vely;

        particles[i].Velx = Vx + delta*ax;
        particles[i].Vely = Vy + delta*ay;


        px = (particles + i)->Posx + delta*Vx;
        py = (particles + i)->Posy + delta*Vy;            /*If particles fly out they get put on the edge of the 1 x 1 box*/
        if (px >= 1){
            particles[i].Posx = 1;
        }
        else if(px <= 0){
            particles[i].Posx = 0;
        }
        else{
            particles[i].Posx = px;
        }

        if (py >= 1){
            particles[i].Posy = 1;
        }
        else if(py <= 0){
            particles[i].Posy = 0;
        }
        else{
            particles[i].Posy = py;
        } 
        *(fx + i) = 0; 
        *(fy + i) = 0;
        /*printf("Posupdate\nPosx = %f Posy = %f\n", particles->Posx, particles->Posy);*/
    }
}


void read1(char *infile, particle *particles){
    FILE *file;
    file = fopen(infile, "rb");                
    for (int i = 0; i < N; i++) {
        fread(&particles[i].Posx, sizeof(double), 1, file);	
        fread(&particles[i].Posy, sizeof(double), 1, file);	
        fread(&particles[i].Mass, sizeof(double), 1, file);	
        fread(&particles[i].Velx, sizeof(double), 1, file);	
        fread(&particles[i].Vely, sizeof(double), 1, file);	
        fread(&particles[i].Bright, sizeof(double), 1, file);
    }
    fclose(file);
}

void write1(particle *particles){
    FILE *outfile;
    outfile = fopen("result.gal", "wb");
    for (int i = 0; i < N; i++) {
        fwrite(&particles[i].Posx, sizeof(double), 1, outfile);
        fwrite(&particles[i].Posy, sizeof(double), 1, outfile);	
        fwrite(&particles[i].Mass, sizeof(double), 1, outfile);	
        fwrite(&particles[i].Velx, sizeof(double), 1, outfile);	
        fwrite(&particles[i].Vely, sizeof(double), 1, outfile);	
        fwrite(&particles[i].Bright, sizeof(double), 1, outfile);
    }
    fclose(outfile);
}



void graphics(particle *particles, float L, float W, float circleRadius, float circleColor){
    ClearScreen();
    for (int i = 0; i < N; i++){
        DrawCircle(particles[i].Posx, particles[i].Posy, L, W, circleRadius, circleColor);
    }
    Refresh();
    usleep(3000);
}




int main(int argc, char** argv){
    if (6 != argc){
        printf("Needs five input variables!\n");
    }
    else{
        N = atoi(argv[1]);
        char * infile = argv[2];
        stp = atoi(argv[3]);
        delta = atof(argv[4]);
        graph = atoi(argv[5]);
        G = 100.0/N;

        printf("\n\nN = %d\n", N);
        printf("Input file = %s\n",infile);
        printf("Number of timesteps = %d\n",stp);
        printf("Timestep = %f\n",delta);
        printf("Graphics = %d\n\n",graph);


        particle *particles = NULL; 
        particles = calloc(N,sizeof(particle));

        read1(infile, particles);


        /*for (int i = 0; i < N; i++) {
            printf("Start\nParticle %d\nPosx %f\nPosy %f\nMass %f\nVelx %f\nVely %f\nBrightness %f\n\n",i+1,particles[i].Posx,particles[i].Posy,particles[i].Mass,particles[i].Velx,particles[i].Vely,particles[i].Bright);
        }*/     



        /*initialize acc som pointer pointer i,j?       https://www.tutorialspoint.com/how-to-dynamically-allocate-a-2d-array-in-c*/
        double *ftotx = (double *)malloc(N * sizeof(double));                 /*Malloc memory for N size array ax */
        double *ftoty = (double *)malloc(N * sizeof(double));                 /*Malloc memory for N size array ay */


        
        
        
        /*struct fxy a;
        struct fxy b;
        a = force_calc(particles[2], particles[0]);*/                         /*Test specific particles*//*
        b = force_calc(particles[2], particles[1]);
        printf("Force on particle %d x: %f y: %f\n", 1, a.fx, a.fy);
        printf("Force on particle %d x: %f y: %f\n\n", 1, b.fx, b.fy);*/

        if (graph == 1){
            InitializeGraphics(argv[0],windowWidth,windowWidth);
            SetCAxes(0,1);
            printf("Hit q to quit.\n");
        }


        
        for (int n = 0; n < stp; n++){
            force_calc(particles, ftotx, ftoty);                /*Force calc to get force in x and y direction for particle i*/  /*If for a matrix: (*arr is first value, *arr+i*N is ith column(or row idk), *arr+i*N+j is jth element of ith row*/


                                                                    /*printf("Sum of x forces %f Sum of y forces %f\n\n", sumx, sumy);*/
                                                                /*for (int i = 0; i < N; i++){
                                                                    printf("Force on particle %d x: %f y: %f\n", i, accx[i]*particles[i].Mass, accy[i]*particles[i].Mass);
                                                                }*/
                                                                /*printf("Before pos update particle %d\n Posx %f Posy %f\n Velx %f Vely %f\n Accx %f Accy %f\n\n", i, particles[i].Posx, particles[i].Posy, particles[i].Velx, particles[i].Vely, accix[i], acciy[i]);*/
            /* step all particles*/
            pos_update(particles, ftotx, ftoty);

            /*Update Graphics*/
            if (graph == 1 && quit != 1){
                graphics(particles, L, W, circleRadius, circleColor);
                quit = CheckForQuit();
                if (quit == 1){
                    FlushDisplay();
                    CloseDisplay();
                }
            }
            /*printf("After pos update particle %d\n Posx %f Posy %f\n Velx %f Vely %f\n Accx %f Accy %f\n\n", i, particles[i].Posx, particles[i].Posy, particles[i].Velx, particles[i].Vely, accix[i], acciy[i]);*/
        }



        write1(particles);



        /*for (int i = 0; i < N; i++) {
            printf("Finish\nParticle %d\nPosx %f\nPosy %f\nMass %f\nVelx %f\nVely %f\nBrightness %f\n\n",i+1,particles[i].Posx,particles[i].Posy,particles[i].Mass,particles[i].Velx,particles[i].Vely,particles[i].Bright);
        }*/

        free(ftotx);
        free(ftoty);
        free(particles);
    }
   	return(0);
}