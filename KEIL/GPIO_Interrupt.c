#include <stdio.h>
#include "NuMicro.h"

extern volatile uint16_t   currentX;
extern volatile uint16_t   currentY;
extern volatile uint8_t    currentDirection;
extern volatile uint8_t    currentType;
extern volatile uint8_t    currentXRLimit;
extern volatile uint8_t    currentXLLimit;
extern volatile uint16_t   currentYLimit;

/*---------------------------------------------------------------------------*/
/*GPIO Interrupt                                                             */
/*---------------------------------------------------------------------------*/
void GPG_IRQHandler(void)
{
	if (PG->INTSRC & (1 << 2)) // Check if the interrupt is from PG.2
    {   
        clearCurrentMove(currentX, currentY, currentType, currentDirection);
        currentDirection++; 
        if (currentType == 1 || currentType == 5 || currentType == 6) {
            if (currentDirection > 2) {
                currentDirection = 1;
            }
        } else {
            if (currentDirection > 4) {
            currentDirection = 1;
            }
        } 
		
        switch(currentType) {
            case 1: 
                if (currentDirection == 2 && currentX >= 130){
                    currentX = 120;
                }
                break;
            case 3:
                if ((currentDirection == 2 || currentDirection == 4) && currentX == 140) {
                    currentX = currentX - 10;
                } 
                break;
            case 4:
                if ((currentDirection == 2 || currentDirection == 4) && currentX == 140) {
                    currentX = currentX - 10;
                } 
                break;
            case 5:
                if (currentDirection == 2 && currentX == 140) {
                    currentX = currentX - 10;
                }
                break;
            case 6:
                if (currentDirection == 2 && currentX == 140) {
                    currentX = currentX - 10;
                }
                break;
            case 7:
                if ((currentDirection == 2 || currentDirection == 4) && currentX == 140) {
                    currentX = currentX - 10;
                } 
                break;
        }
        
        switch(currentType) {
            case 1:
                if (currentDirection == 1) {
                    currentXRLimit = 150;
                    currentYLimit = 260;
                } else {
                    currentXRLimit = 120;
                    currentYLimit = 290;
                }
                LCD_Draw_BlockType1(currentX, currentY, currentDirection);
                break;
            case 3:
                if (currentDirection == 1 || currentDirection == 3) {
                    currentYLimit = 270;
                    currentXRLimit = 140;
                } else {
                    currentYLimit = 280;
                    currentXRLimit = 130;
                }
                LCD_Draw_BlockType3(currentX, currentY, currentDirection);
                break;
            case 4:
                if (currentDirection == 1 || currentDirection == 3) {
                    currentYLimit = 270;
                    currentXRLimit = 140;
                } else {
                    currentYLimit = 280;
                    currentXRLimit = 130;
                }
                LCD_Draw_BlockType4(currentX, currentY, currentDirection);
                break;
            case 5:
                if (currentDirection == 1) {
                    currentYLimit = 270;
                    currentXRLimit = 140;
                } else {
                    currentYLimit = 280;
                    currentXRLimit = 130;
                }
                LCD_Draw_BlockType5(currentX, currentY, currentDirection);
                break;
            case 6:
                if (currentDirection == 1) {
                    currentYLimit = 270;
                    currentXRLimit = 140;
                } else {
                    currentYLimit = 280;
                    currentXRLimit = 130;
                }
                LCD_Draw_BlockType6(currentX, currentY, currentDirection);
                break;
            case 7: 
                if (currentDirection == 1 || currentDirection == 3) {
                    currentYLimit = 270;
                    currentXRLimit = 140;
                } else {
                    currentYLimit = 280;
                    currentXRLimit = 130;
                }
                LCD_Draw_BlockType7(currentX, currentY, currentDirection);
                break;
        }
        
        PG->INTSRC |= (1 << 2); // Clear the interrupt flag
	}
    
    if (PG->INTSRC & (1 << 4)) // Check if the interrupt is from PG.4
    {
        clearCurrentMove(currentX, currentY, currentType, currentDirection);
        if (currentX <= currentXRLimit - 10) {
            currentX+=10;
        }
		LCD_Draw_A_Tetris_Block(currentX, currentY, currentType, currentDirection);
        PG->INTSRC |= (1 << 4); // Clear the interrupt flag
	}
	 
	
}

void GPC_IRQHandler(void) {
    if (PC->INTSRC & (1 << 9)) // Check if the interrupt is from PG.4
    {
        clearCurrentMove(currentX, currentY, currentType, currentDirection);
        if (currentX > currentXLLimit) {
            currentX-=10;
        }
		LCD_Draw_A_Tetris_Block(currentX, currentY, currentType, currentDirection);
        PC->INTSRC |= (1 << 9); // Clear the interrupt flag
	}
    
    if (PC->INTSRC & (1 << 10)) // Check if the interrupt is from PG.4
    {
        if (currentY <= currentYLimit) {
            clearCurrentMove(currentX, currentY, currentType, currentDirection);
            currentY+=10;
            LCD_Draw_A_Tetris_Block(currentX, currentY, currentType, currentDirection);
        }
        PC->INTSRC |= (1 << 10); // Clear the interrupt flag
	}
}