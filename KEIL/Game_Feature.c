#include <stdio.h>
#include "NuMicro.h"


#include "Game_Feature.h"
#include "EBI_LCD_Module.h"

void clearCurrentMove(uint16_t currentX, uint16_t currentY, uint16_t type, uint16_t direction) {
    switch(type) {
        case 1:
            if (direction == 1) {
                LCD_BlankArea(currentX, currentY, 40, 10);
            } else {
                LCD_BlankArea(currentX, currentY, 10, 40);
            }
            break;
        case 3:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 30);
                    LCD_BlankArea(currentX + 10, currentY, 10, 10);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 30, 10);
                    LCD_BlankArea(currentX + 20, currentY + 10, 10, 10);
                    break;
                case 3:
                    LCD_BlankArea(currentX + 10, currentY, 10, 30);
                    LCD_BlankArea(currentX, currentY + 20, 10, 10);
                    break;
                case 4:
                    LCD_BlankArea(currentX, currentY + 10, 30, 10);
                    LCD_BlankArea(currentX, currentY, 10, 10);
                    break;
            }
            break;
        case 4:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 30);
                    LCD_BlankArea(currentX + 10, currentY + 20, 10, 10);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 30, 10);
                    LCD_BlankArea(currentX, currentY + 10, 10, 10);
                    break;
                case 3:
                    LCD_BlankArea(currentX + 10, currentY, 10, 30);
                    LCD_BlankArea(currentX, currentY, 10, 10);
                    break;
                case 4:
                    LCD_BlankArea(currentX, currentY + 10, 30, 10);
                    LCD_BlankArea(currentX + 20, currentY, 10, 10);
                    break;
            }
            break;
        case 5:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 20);
                    LCD_BlankArea(currentX + 10, currentY + 10, 10, 20);
                    break;
                case 2:
                    LCD_BlankArea(currentX + 10, currentY, 20, 10);
                    LCD_BlankArea(currentX, currentY, 20, 10);
                    break;
            }
            break;
        case 6:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY + 10, 10, 20);
                    LCD_BlankArea(currentX + 10, currentY, 10, 20);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 20, 10);
                    LCD_BlankArea(currentX + 10, currentY + 10, 20, 10);
                    break;
            }
            break;
        case 7:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY + 10, 10, 10);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 30, 10);
                    LCD_BlankArea(currentX + 10, currentY + 10, 10, 10);
                    break;
                case 3:
                    LCD_BlankArea(currentX + 10, currentY, 10, 30);
                    LCD_BlankArea(currentX, currentY + 10, 10, 10);
                    break;
                case 4:
                    LCD_BlankArea(currentX, currentY + 10, 30, 10);
                    LCD_BlankArea(currentX + 10, currentY, 10, 10);
                    break;
            }
            break;
    } 

};

void updateTheCurrentMove(uint16_t currentX, uint16_t currentY, uint16_t type, uint16_t direction);