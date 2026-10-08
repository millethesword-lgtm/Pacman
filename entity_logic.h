#ifndef ENTITY_LOGIC_H
#define ENTITY_LOGIC_H

struct entity{
    int col, row;
    int drCol, drRow;
    int scared;
    int eaten;
    int ghosttype;
    int currentSqRow, currentSqCol;
    int wantedDrRow, wantedDrCol;
} entity;

void labinit();

void move_pacman(struct entity *pacman, int map[][32]);

void ghost_movement(struct entity *ghost, int targetX, int targetY, int map[][32]);

void setup(struct entity *entity, int row, int col, int ghosttype);

int collisionDetection(struct entity *entity20, int map[][32]);

#endif