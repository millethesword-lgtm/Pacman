#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "entity_logic.h"
#include <limits.h>

volatile int* dataGPIO = (volatile int *)0x40000e0;
volatile int* dataDirection = (volatile int *)0x40000e4;
volatile int *led = (volatile int*)0x04000000;

// Erik
void setup(struct entity *entity20, int row1, int col1, int ghostnumber){
    entity20->col = col1;
    entity20->row = row1;
    entity20->eaten = 0;
    entity20->scared = 0;
    entity20->drCol = 0;
    entity20->drRow = 0;
    entity20->ghosttype = ghostnumber; 
    entity20->currentSqRow = row1/10;
    entity20->currentSqCol = col1/10;
    entity20->wantedDrRow = 0;
    entity20->wantedDrCol = 0;
    entity20->score = 0;
};

// Erik
void labinit(void){
    *dataDirection = 0x0;
}

// Erik
int readinput(){
    return *dataGPIO;
}

// Made by Erik - Small inputs from Milton
int collisionDetection(struct entity *entity20, int map[][32]){
    entity20->currentSqRow = entity20->row/10;
    entity20->currentSqCol = entity20->col/10;

    int nextRow = entity20->currentSqRow + entity20->drRow;
    int nextCol = entity20->currentSqCol + entity20->drCol;

    if(map[nextRow][nextCol] == 1){
        return 1;
    }
    return 0;
}

// Made in collab (Erik/Milton)
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

    if((pacman->row)%10 == 0 && (pacman->col)%10 == 0){
        int oldDrRow = pacman->drRow;
        int oldDrCol = pacman->drCol;
        pacman->drRow = pacman->wantedDrRow;
        pacman->drCol = pacman->wantedDrCol;
        if(collisionDetection(pacman, map)){
            pacman->drRow = oldDrRow;
            pacman->drCol = oldDrCol;
            if(collisionDetection(pacman, map)){
                pacman->drRow = 0;
                pacman->drCol = 0;
            }
        }
    }
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

// Written by Erik - small inputs from Milton
void ghost_movement(struct entity *ghost, int targetX, int targetY, int map[][32]){
    if(ghost->row % 10 == 0 && ghost->col % 10 == 0){
    int drX [4] = {-1, 0, 1, 0};
    int drY [4] = {0, -1, 0, 1};

    int alldist[4] = {};

    for (int d = 0; d < 4; d++){
        int newY =  ghost->currentSqRow + drY[d];
        int newX = ghost->currentSqCol + drX[d];

        if(ghost->drRow != 0 || ghost->drCol != 0){
            if(drX[d] == -ghost->drRow && drY[d] == -ghost->drCol) continue;
        }

        int distRow = newY - targetY;
        int distCol = newX - targetX;

        int dist = distRow * distRow + distCol * distCol;

        alldist[d] = dist;
    }   

    int shortestDist = 2147483647;
    int idx = -1;

    int oldDrRow = ghost->drRow;
    int oldDrCol = ghost->drCol;

    for(int d = 0; d < 4; d++){
        if(-oldDrRow == drY[d] && -oldDrCol == drX[d]) continue;

        ghost->drRow = drY[d];
        ghost->drCol = drX[d];

        if(!collisionDetection(ghost, map) && alldist[d] < shortestDist){
            shortestDist = alldist[d];
            idx = d;
        }
    }
    if(idx != -1){
        ghost->drRow = drY[idx];
        ghost->drCol = drX[idx];
    } else {
        ghost->drRow = -oldDrRow;
        ghost->drCol = -oldDrCol;
    } 
    }
    ghost->row += ghost->drRow;
    ghost->col += ghost->drCol;
}