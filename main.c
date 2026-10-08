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
    //int score = 0;

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