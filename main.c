
/******************************************************************************
 * @file     main.c
 * @brief    This is a M480 EBI sample code to drive the LCD displaying and
 *           also utilize ADC to judge the position of finger touch.
 * @version  1.0.0
 * @Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "NuMicro.h"
#include "EBI_LCD_Module.h"

/*---------------------------------------------------------------------------*/
/* Define                                                                    */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* Global variables                                                          */
/*---------------------------------------------------------------------------*/
extern  volatile    uint8_t Timer1_flag;
extern  volatile    uint8_t Timer1_cnt;

/*---------------------------------------------------------------------------*/
/* Local variables                                                           */
/*---------------------------------------------------------------------------*/
//The varible for the current status
volatile uint16_t   currentX;
volatile uint16_t   currentY;
volatile uint8_t    currentDirection;
volatile uint8_t    currentType;
volatile uint8_t    currentXRLimit;
volatile uint8_t    currentXLLimit;
volatile uint16_t   currentYLimit;

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/


void generateNewBlock() {

}

/*---------------------------------------------------------------------------*/
/*Timer Interrupt                                                            */
/*---------------------------------------------------------------------------*/

void TMR0_IRQHandler(void) {
    if ((currentY <= currentYLimit) && isValidStep() == 0) {
        clearCurrentMove(currentX, currentY, currentType, currentDirection);
        currentY+=10;
        LCD_Draw_A_Tetris_Block(currentX, currentY, currentType, currentDirection);
    } else {
        generateNewBlock();
    }
    /* Clear timer interrupt flag */
    TIMER0->INTSTS |= (1 << 0);
}






/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    //UART_Open(UART0, 115200);
    // UART 0 operation configuration
	UART0->LINE |= (0b11 << 0); 			                    // 8 data bit
	UART0->LINE &= ~(1 << 2); 				                    // One stop bit
	UART0->LINE &= ~(1 << 3);				                    // No parity bit
	UART0->FIFO |= (1 << 1); 				                    // Clear RX FIFO
	UART0->FIFO |= (1 << 2); 				                    // Clear TX FIFO
	UART0->FIFO &= ~(0xF << 16); 			                    // FIFO Trigger level is 1 byte
	
	// Baud rate config: Mode 0 - 115200 bps
	UART0->BAUD &= ~(0b11 << 28);                               // mode 0
	UART0->BAUD &= ~(0xFFFF << 0);                              // Clear the current baud rate divisor value
	UART0->BAUD |= 4;                                           //Divisor is equal 4 to generate the baud rate = 115200	

    /* Initialize EBI bank0 to access external LCD Module */
    EBI->CTL0 &= ~((0xF << 0) | (1 << 4) | (0b111 << 8)         // Clear the current setting in EBI Bank0.
                    | (0b111 << 16) | (1 << 24));               
    EBI->CTL0 |= (1 << 1);                                      // Set the EBI data width to 16-bit.
    EBI->CTL0 |= (0b001 << 8);                                  // Set the external output clock divider to HCLK / 2 (001).
    EBI->CTL0 &= ~(1 << 2);                                     // Select the active low chip pin.
    EBI->CTL0 |= (1 << 0);                                      // Enable EBI function/
    EBI->CTL0 |= (3 << 16);                                     // Set the extend time of ALE.
    EBI->CTL0 |= (1 << 4);                                      // Enable the continous data access mode.
    EBI->TCTL0 |= ((1 << 23) | (1 << 22));                      // Disable the data access hold time during EBI writing and reading.

    /* Init LCD Module */
    ILI9341_Initial();

    /* PB.7 BL_CTRL pin */
    PB7 = 1;

    /* Set PH7/PH6 as output mode for LED1/LED2 */
    PH->MODE &= ~((0x03 << 12) | (0x03 << 14)); 	            // clear settings for PH.6 and PH.7
	PH->MODE |= ((0x01 << 12) | (0x01 << 14)); 			        // Set output push-pull mode for PH.6 and PH.7
    PH7 = 1;
    PH6 = 1;

    /* Init ADC for TP */
    /* Set input mode as single-end and enable the A/D converter */
    //EADC_Open(EADC, EADC_CTL_DIFFEN_SINGLE_END);
    EADC->CTL &= ~(1 << 8);                                     // Set the single end analog input mode.
    EADC->CTL |= (1 << 0);                                      //Enable EADC.

    /*=== Show the 1st display ===*/
    /* Blank screen */
    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
    
    currentX = 40;
    currentY = 40;
    currentType = 7;
    currentDirection = 4;
    currentXRLimit = 120;
    currentXLLimit = 10;
    currentYLimit = 320;
    
    LCD_Draw_GameField();
    LCD_Draw_BlockType7(currentX, currentY, 4);
    
    while(1){}
}



