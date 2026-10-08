#include "blueprints.h"
#include "display.h"
#include "entity_logic.h"
#include <stdio.h>

struct entity pacman;
struct entity blinky;
struct entity pacman2;

int main(){
    labinit();
    volatile unsigned char *drawBuffer = buffer2;
    setup(&pacman, 10, 10, 0);
    setup(&blinky, 20, 50, 1);
    int gameover = 0;
    int score = 0;

    // Thoughts for collision systems: do row/col modulo 10, if rest is 0, move in the current movement direction
    // in the 2d map, and once you are fully in one square, you consume what is in it, and when it comes to pathing,
    // check in movement direction if the next square indicator on map is 1, then continue in direction that is currently
    // used. Update map when fully in a square and remove consumable that was in there (change to a number that represents
    // an empty square, no current one so make a new one).
    // I think the collision function can be in entity logic, and should work for all entities :)
    while(gameover != 1){
        clearBuffer(drawBuffer);
        printMap(drawBuffer, map);

        // draws moving objects, such as pacman/ghosts
        printMoving(drawBuffer, pacBlpt, pacman.row, pacman.col);
        printMoving(drawBuffer, blinkyBlpt, blinky.row, blinky.col);

        // For some reason makes pacman dissapear (col/row was left undefined and pac wsa sent to shadow realm)
        move_pacman(&pacman, map);
        ghost_movement(&blinky, pacman.currentSqRow, pacman.currentSqCol, map);

        switchBuffer(drawBuffer);
        if(drawBuffer == buffer1){
            drawBuffer = buffer2;
        }
        else{
            drawBuffer = buffer1;
        }
        for(volatile int i = 0; i<30000; i++){

        }
    }
}