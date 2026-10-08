#ifndef DISPLAY_H
#define DISPLAY_H

// Change to correct ones
extern volatile unsigned char *buffer1;
extern volatile unsigned char *buffer2;

void displayPrint(void);
void printUnit(volatile unsigned char*bufferArea, int pBlueprint[][10], int row, int col);
void printMoving(volatile unsigned char*bufferArea, int pBlueprint[][10], int row, int col);
void printMap(volatile unsigned char*bufferArea, int map[][32]);
void testSwapBuffer(void);
void switchBuffer(volatile unsigned char *bufferArea);
void clearBuffer(volatile unsigned char *bufferArea);

#endif