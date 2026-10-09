#include "blueprints.h"
#include "display.h"
#include "entity_logic.h"
#include <stdio.h>

struct entity pacman;
struct entity blinky;
struct entity blinky2;
struct entity blinky3;

void handle_interrupt(){

}

int pow(int p, int e){
    if(e == 0) return 1;

    if( e == 1) return p;

    int ret = p;
    for(int i = 1; i<e; i++){
        ret *= p;
    }
    return ret;
}

int abs(int x){
    if(x < 0){
        return -x;
    }
    return x;
}

int entityOverlap(struct entity *entity1, struct entity *entity2){
    if(abs(entity1->row - entity2->row) < 5 && abs(entity1->col - entity2->col) < 5){
        return 1;
    }
    return 0;
}

int separateNumbers(int number, char *out){
    int count = 0;

    if(number == 0){
        out[0] = 0;
        return 1;
    }

    for(int i = 0; 1; i++){
        int p = pow(10, i);

        if(number < p) break;

        out[i] = (number/p)%10;
        count++;
    }
    return count;
}



// Written in collab by Erik/Milton
int main(){
    labinit();
    volatile unsigned char *drawBuffer = buffer2;

    setup(&pacman, 10, 10, 0);
    setup(&blinky, 220, 10, 1);
    setup(&blinky2, 10, 300, 2);
    setup(&blinky3, 220, 220, 3);

    int gameover = 0;

    while(gameover != 1){
        clearBuffer(drawBuffer);
        printMap(drawBuffer, map);

        printMoving(drawBuffer, pacBlpt, pacman.row, pacman.col);
        printMoving(drawBuffer, blinkyBlpt, blinky.row, blinky.col);
        printMoving(drawBuffer, blinkyBlpt, blinky2.row, blinky2.col);
        printMoving(drawBuffer, blinkyBlpt, blinky3.row, blinky3.col);

        move_pacman(&pacman, map);
        ghost_movement(&blinky, pacman.currentSqCol, pacman.currentSqRow, map);
        ghost_movement(&blinky2, pacman.currentSqCol, pacman.currentSqRow, map);
        ghost_movement(&blinky3, pacman.currentSqCol, pacman.currentSqRow, map);

        if(entityOverlap(&pacman, &blinky) || entityOverlap(&pacman, &blinky2) || entityOverlap(&pacman, &blinky3)){
            gameover = 1;
        }

        switchBuffer(drawBuffer);
        if(drawBuffer == buffer1){
            drawBuffer = buffer2;
        }
        else{
            drawBuffer = buffer1;
        }
        for(volatile int i = 0; i<20000; i++){

        }
    }
    char numbers[8];
    int count = separateNumbers(pacman.score, numbers); 

    for(int i = 0; i<count; i++){
        switch(numbers[i]){
            case 0:
                printMoving(drawBuffer, zeroBlpt, 220, (300 - 10*i));
                break;
            case 1:
                printMoving(drawBuffer, oneBlpt, 220, (300 - 10*i));
                break;
            case 2:
                printMoving(drawBuffer, twoBlpt, 220, (300 - 10*i));
                break;
            case 3:
                printMoving(drawBuffer, threeBlpt, 220, (300 - 10*i));
                break;
            case 4:
                printMoving(drawBuffer, fourBlpt, 220, (300 - 10*i));
                break;
            case 5:
                printMoving(drawBuffer, fiveBlpt, 220, (300 - 10*i));
                break;
            case 6:
                printMoving(drawBuffer, sixBlpt, 220, (300 - 10*i));
                break;
            case 7:
                printMoving(drawBuffer, sevenBlpt, 220, (300 - 10*i));
                break;
            case 8:
                printMoving(drawBuffer, eightBlpt, 220, (300 - 10*i));
                break;
            case 9:
                printMoving(drawBuffer, nineBlpt, 220, (300 - 10*i));
                break;
        }
    }
    switchBuffer(drawBuffer);
}