#include <stdio.h>
#include "NuMicro.h"

//Play Field Data
volatile int playFieldData[30][14];
volatile int numOfBlockInLine[30];
volatile int highestScoreVal = 0;
volatile int scoreVal = 0;
volatile int stage = 0;

void initialTheStartGameStatus() {
    int i, j;
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 14; j++) {
            playFieldData[i][j] = 0;
        }
        numOfBlockInLine[i] = 0;
    }
}

int isValidStep(uint16_t currentX, uint16_t currentY, uint16_t currentType, uint16_t currentDirection) {
    int x4checking[4] = {0,0,0,0}, y4checking[4] = {0, 0, 0, 0};
    int i;
    
    switch(currentType) {
        case 1: 
            if (currentDirection == 1) {
                for (i = 0; i < 4; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 10;
                }
            } else {
                x4checking[0] = currentX;
                y4checking[0] = currentY + 40;
            }
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                x4checking[i] = currentX + (i * 10);
                y4checking[i] = currentY + 20;
            }
            break;
        case 3:
            switch(currentDirection) {
                case 1:
                    x4checking[0] = currentX;
                    x4checking[0] = currentY + 30;
                
                    x4checking[1] = currentX + 10;
                    y4checking[1] = currentY + 10;
                    break;
                case 2:
                    for (i = 0; i < 2; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 10;
                    }
                    x4checking[2] = currentX + 20;
                    y4checking[2] = currentY + 20;
                    break;
                case 3:
                    for (i = 0; i < 2; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 30;
                    }
                    break;
                case 4:
                    for (i = 0; i < 3; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 20;
                    }
                    break;
            
            }
            break;
        case 4:
            switch(currentDirection) {
                case 1:
                    for (i = 0; i < 2; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 30;
                    }
                    break;
                case 2:
                    for (i = 1; i < 3; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 10;
                    }
                    x4checking[0] = currentX;
                    y4checking[0] = currentY + 20;
                    break;
                case 3:
                    x4checking[0] = currentX;
                    x4checking[0] = currentY + 10;
                
                    x4checking[1] = currentX + 10;
                    y4checking[1] = currentY + 30;
                    break;
                case 4:
                    for (i = 0; i < 3; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 20;
                    }
                    break;
            
            }
            break;
        case 5:
            if (currentDirection == 1) {
                x4checking[0] = currentX;
                y4checking[0] = currentY + 20;
                
                x4checking[1] = currentX + 10;
                y4checking[1] = currentY + 30;
            } else {
                for (i = 0; i < 2; i++) {
                    x4checking[i] = currentX + (i * 10);
                    y4checking[i] = currentY + 30;
                }
                x4checking[2] = currentX + 20;
                y4checking[2] = currentY + 10;
            }
            break;
        case 6:
            if (currentDirection == 1) {
                x4checking[0] = currentX;
                y4checking[0] = currentY + 30;
                
                x4checking[1] = currentX + 10;
                y4checking[1] = currentY + 20;
            } else {
                for (i = 1; i < 3; i++) {
                    x4checking[i] = currentX + (i * 10);
                    y4checking[i] = currentY + 30;
                }
                x4checking[0] = currentX;
                y4checking[0] = currentY + 10;
            }
            break;
        case 7:
            switch(currentDirection) {
                case 1:
                    x4checking[0] = currentX;
                    y4checking[0] = currentY + 30;
                
                    x4checking[1] = currentX + 10;
                    y4checking[1] = currentY + 20;
                    break;
                case 2:
                    x4checking[0] = currentX;
                    y4checking[0] = currentY + 10;
                
                    x4checking[1] = currentX + 10;
                    y4checking[1] = currentY + 20;
                
                    x4checking[1] = currentX + 20;
                    y4checking[1] = currentY + 10;
                    break;
                case 3:
                    x4checking[0] = currentX;
                    y4checking[0] = currentY + 20;
                
                    x4checking[1] = currentX + 10;
                    y4checking[1] = currentY + 30;
                    break;
                case 4:
                    for (i = 0; i < 3; i++) {
                        x4checking[i] = currentX + (i * 10);
                        y4checking[i] = currentY + 20;
                    }
                    break;
            
            }
            break;
    }
    
    int isValid = 0;
    
    for (i = 0; i < 4; i++) {
        if (x4checking[i] != 0 && y4checking[i] != 0) {
            isValid = (playFieldData[y4checking[i] / 10][x4checking[i] / 10] != 0) ? 1 : 0;
        }
    }
    
    
    return isValid;
}

void increasePoint(uint16_t currentX, uint16_t currentY) {


}