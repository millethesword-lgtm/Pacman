#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "entity_logic.h"

volatile int* dataGPIO = (volatile int *)0x40000e0;
volatile int* dataDirection = (volatile int *)0x40000e4;
volatile int *led = (volatile int*)0x04000000;


void setup(struct entity *entity20, int row1, int col1, int ghostnumber){
    entity20->col = col1;
    entity20->row = row1;
    entity20->eaten = 0;
    entity20->scared = 0;
    entity20->drCol = 0;
    entity20->drRow = 0;
    entity20->ghosttype = ghostnumber; // pacman is not a ghost
    entity20->currentSqRow = row1/10;
    entity20->currentSqCol = col1/10;
    entity20->wantedDrRow = 0;
    entity20->wantedDrCol = 0;
    entity20->score = 0;
};

void labinit(void){
    *dataDirection = 0x0;
}

int readinput(){
    return *dataGPIO;
}

// Rework this function and incorporate collision details from move_pacman
void collisionDetection(struct entity *entity20, int map[][32]){
    if((entity20->row)%10 == 0 && (entity20->col)%10 == 0){
            // add points to score depending on what is at the square you empty
            
            //map[pacman->currentSqRow][pacman->currentSqCol] = -1;
            entity20->currentSqRow = entity20->row/10;
            entity20->currentSqCol = entity20->col/10;

            int nextRow = entity20->currentSqRow + entity20->drRow;
            int nextCol = entity20->currentSqCol + entity20->drCol;

            if(map[entity20->currentSqRow + entity20->wantedDrRow][entity20->currentSqCol + entity20->wantedDrCol] != 1){
                entity20->drRow = entity20->wantedDrRow;
                entity20->drCol = entity20->wantedDrCol;
            }
            if(map[nextRow][nextCol] == 1){
                entity20->drRow = 0;
                entity20->drCol = 0;
            }
    }
}

void move_pacman(struct entity *pacman, int map[][32]){
    int pins = readinput();
    *led = pins;
    if(pins & 0x10000){
        pacman->wantedDrRow = 0;
        pacman->wantedDrCol = 1;
    }
    else if(pins & 0x1000){
        pacman->wantedDrRow = 1;
        pacman->wantedDrCol = 0;
    }
    else if(pins & 0x10){
        pacman->wantedDrRow = 0;
        pacman->wantedDrCol = -1;
    }
    else if(pins & 0x1){
        pacman->wantedDrRow = -1;
        pacman->wantedDrCol = 0;
    }

    pacman->row = pacman->row + pacman->drRow;
    pacman->col = pacman->col + pacman->drCol;

    collisionDetection(pacman, map);
    
    switch(map[pacman->currentSqRow][pacman->currentSqCol]){
        case 0:
            pacman->score += 10;
            break;
        case 3:
            pacman->score += 200;
            break;
        case 9:
            pacman->score += 50;
            break;
    }
    map[pacman->currentSqRow][pacman->currentSqCol] = -1;
}


void ghost_movement(struct entity *ghost, int targetX, int targetY, int map[][32]){
    if(ghost->row % 10 == 0 && ghost->col % 10 == 0){
    int drX [4] = {-1, 0, 1, 0};
    int drY [4] = {0, -1, 0, 1};

    int bestdist = 1000000;
    int alldist[4] = {};

    for (int d = 0; d < 4; d++){
        int newY =  ghost->currentSqRow + drY[d];
        int newX = ghost->currentSqCol + drX[d];

        if(ghost->drRow != 0 || ghost->drCol != 0){
            if(drX[d] == -ghost->drRow && drY[d] == -ghost->drCol) continue;
        }

        int distRow = newY - targetY;
        int distCol = newX - targetX;

        //sqrt is unrecognized
        int dist = distRow * distRow + distCol * distCol;

        alldist[d] = dist;
    }

    for(int d = 0; d < 4; d++){
        ghost->wantedDrRow = drY[d];
        ghost->wantedDrCol = drX[d];
        collisionDetection(ghost, map); 

        if(ghost->drRow == 0 && ghost->drCol == 0){
            continue;
        } else if(alldist[d] < bestdist){
            bestdist = alldist[d];
            ghost->drRow = drY[d];
            ghost->drCol = drX[d];
        }
    }

    }

    ghost->row += ghost->drRow;
    ghost->col += ghost->drCol;
}