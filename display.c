#include "blueprints.h"

#define YELLOW 0xFC

#define DefWall = 170
#define Red = 0xFF

volatile unsigned int *bufferRegister = (volatile unsigned int*)0x4000100;
volatile unsigned int *backBufferRegister = (volatile unsigned int*)0x4000104;
volatile unsigned int *statContRegister = (volatile unsigned int*)0x400010C;

volatile unsigned char *buffer1 = (volatile unsigned char*)0x08000000;
volatile unsigned char *buffer2 = (volatile unsigned char*)0x08012C00;

// Milton
void printUnit(volatile unsigned char*bufferArea, int pBlueprint[][10], int row, int col){
  for(int k = 0; k < 10; k++){
    for(int i = 0; i < 10; i++){
      switch(pBlueprint[k][i]){
      case 0:
        // Nothing
        break;
      case 1:
        bufferArea[(row * 10 + k) * 320 + (col * 10 + i)] = 170;
        break;
      case 2:
        bufferArea[(row * 10 + k) * 320 + (col * 10 + i)] = 54;
        break;
      case 3:
        bufferArea[(row * 10 + k) * 320 + (col * 10 + i)] = 192;
        break;
      case 4:
        bufferArea[(row * 10 + k) * 320 + (col * 10 + i)] = YELLOW;
        break;
      case 8:
        bufferArea[(row * 10 + k) * 320 + (col * 10 + i)] = 0x14;
        break;
      case 9:
        bufferArea[(row * 10 + k) * 320 + (col * 10 + i)] = 255;
        break;  
      }
    }
  }
}

// Milton
void printMoving(volatile unsigned char*bufferArea, int pBlueprint[][10], int currentRow, int currentCol){
  for(int k = 0; k < 10; k++){
    for(int i = 0; i < 10; i++){
      switch(pBlueprint[k][i]){
      case 0:
        // Nothing
        break;
      case 1:
        bufferArea[(currentRow + k) * 320 + (currentCol + i)] = 170;
        break;
      case 2:
        bufferArea[(currentRow + k) * 320 + (currentCol + i)] = 54;      // dont want to use for pellets!!
        break;
      case 3:
        bufferArea[(currentRow + k) * 320 + (currentCol + i)] = 192;    // Should be red - Apple
        break;
      case 4:
        bufferArea[(currentRow + k) * 320 + (currentCol + i)] = YELLOW;
        break;
      case 8:
        bufferArea[(currentRow + k) * 320 + (currentCol + i)] = 0x14; // Is lime-green
        break;
      case 9:
        bufferArea[(currentRow + k) * 320 + (currentCol + i)] = 255;
        break;  
      }
    }
  }
}

// Milton
void drawPellet(volatile unsigned char*bufferArea, int row, int col){
  int x = col * 10 + 4;
  int y = row * 10 + 4;
  for(int i = 0; i < 2; i++){
    for(int k = 0; k < 2; k++){
      bufferArea[(y + i) * 320 + (x + k)] = 54;
    }
  }
}

// Milton
void drawMap(volatile unsigned char*bufferArea, int rowStart, int rowEnd, int colStart, int colEnd){
  for(int row = rowStart; row < rowEnd; row++){
    for(int col = colStart; col < colEnd; col++){
      bufferArea[row * 320 + col] = 170;
    }
  }
}

// Milton
void printMap(volatile unsigned char *bufferArea, int map[][32]){
  for(int row = 0; row < 24; row++){
    for(int col = 0; col < 32; col++){
      switch(map[row][col]){
        case 0:
          drawPellet(bufferArea, row, col);                                         // prints pellets w/o blueprints
          break;
        case 1:
          drawMap(bufferArea, row * 10,row * 10 + 10,col * 10,col * 10 + 10);       // prints walls w/o blueprints
          break;
        case 3:
          printUnit(bufferArea, appleBlpt, row, col);
          break;
        case 4:
          printUnit(bufferArea, pacBlpt, row, col);
          break;
        case 9:
          printUnit(bufferArea, pupBlpt, row, col);
          break;
      }
    }
  }
}

// Milton
void switchBuffer(volatile unsigned char *bufferArea){
  *backBufferRegister = (unsigned int)bufferArea;
  *bufferRegister = 1;
  while((*statContRegister & 0x1) != 0){

  }
}

// Milton
void clearBuffer(volatile unsigned char *bufferArea){
  for(int i=0; i<320*240; i++){
    bufferArea[i] = 0;
  }
}

// Unused - Milton
void displayPrint(volatile unsigned char*bufferArea){ 
  // Clear the entire VGA buffer area by writing the value 0 (=black)
  for (int i = 0; i < 320*480; i++){
    buffer1[i] = 0; 
  }

  printMap(bufferArea, map);
}