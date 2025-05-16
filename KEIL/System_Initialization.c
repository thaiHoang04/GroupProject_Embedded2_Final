#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------*/
/* Define                                                                    */
/*---------------------------------------------------------------------------*/

#define HXTSTB 1<<0				// HXT Clock Sourse Stable Flag
#define LXTSTB 1<<1				// LXT Clock Sourse Stable Flag
#define PLLSTB 1<<2				// Internal PLL Clock Source Stable Flag
#define HXTEN  1<<0				// HXT Enable Bit, write 1 to enable
#define LXTEN  1<<1				// LXT Enable Bit, write 1 to enable
#define HIRCEN 1<<2				// HIRC Enable Bit, write 1 to enable
#define HIRC_STATUS 1 << 4 // 12 MHz

/*---------------------------------------------------------------------------*/
/* Global variables                                                          */
/*---------------------------------------------------------------------------*/
extern  volatile    uint8_t Timer1_flag;
extern  volatile    uint8_t Timer1_cnt;

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/
void enableHXT() {
    //Set XT1_OUT (PF.2) and XT1_IN (PF.3) to input mode
	// 0x00: input; 0x01: Output (push-pull)
    PF->MODE &= ~((0x00 << 4) | (0x00 << 6));

    // Enable and wait until HXT 12MHz clock source is stable
    CLK->PWRCTL |= HXTEN;                                       // Enable HXT
    while (!(CLK->STATUS & HXTSTB));                            // Wait for HXT to stabilize
}

void enableLXT() {
    //Set XT32_OUT (PF.4) and XT32_IN (PF.5) to input mode
    // 0x00: input; 0x01: Output (push-pull)
    PF->MODE &= ~((0b11 << 8) | (0b11 << 10));
    
    // Enable and wait until LXT 32.7 kHz clock source is stable
    CLK->PWRCTL |= LXTEN;                                       // Enable LXT
    while (!(CLK->STATUS & LXTSTB));                            // Wait for LXT to stabilize
}

void enableHIRC() {
    // Enable and wait until HIRC 12 MHz clock source is stable.
    CLK->PWRCTL |= HIRCEN;                                   // Enable HIRC
    while (!(CLK->STATUS & HIRC_STATUS));                       // Wait for HIRC to stabilize
}

void configJoyStickUpRight() {
    // Interrupt Source Joystick on PG.02 and PG.04 - Edge trigger interrupt
    PG->MODE &= ~((0x3 << 4) | (0x3 << 8)); 		// Clear bits [4:5] and [8:9] for PG.2 and PG.4

    PG->INTTYPE &= ~((1 << 2) | (1 << 4)); 		    // Edge trigger interrupt for PG.2 and PG.4
    PG->INTEN |= ((1 << 2) | (1 << 4)); 		    // Falling edge interrupt enable
    PG->INTSRC |= ((1 << 2) | (1 << 4));		    // Clear any pending interrupt flag for PG.2 and PG.4

    // NVIC interrupt configuration
    NVIC->ISER[2] |= (1 << (72 - 64)); 		        // Enable NVIC for the GPIO interrupt on Port G - 
                                                    // GPG_INT is bit 72 --> belong to ISER2 (64 to 95)
}

void configJoyStickDownLeft() {
    // Interrupt Source Joystick on PC.09 and PC.10 - Edge trigger interrupt
    PC->MODE &= ~((0x3 << 18) | (0x03 << 20)); 		// Clear bits [18:19] and [20:21] for PC.9 and PC.10

    PC->INTTYPE &= ~((1 << 9) | (1 << 10)); 		// Edge trigger interrupt for PC.9 and PC.10
    PC->INTEN |= ((1 << 9) | (1 << 10)); 		    // Falling edge interrupt enable
    PC->INTSRC |= ((1 << 9) | (1 << 10));		    // Clear any pending interrupt flag for PC.9 and PC.10

    // NVIC interrupt configuration
    NVIC->ISER[0] |= (1 << 18); 		            // Enable NVIC for the GPIO interrupt on Port G - 
                                                    // GPG_INT is bit 72 --> belong to ISER2 (64 to 95)   
}

void configTimer1() {
    //Config the timer 1
    CLK->CLKSEL1 &= ~ (0b111 << 12);                            // clear setting and choose clock source from HXT
    CLK->CLKSEL1 |= (0b010 << 12);                             // Set clock source for Timer 1 to PCLK0
    CLK->APBCLK0 |= (1 << 3); 		                            // Clock enable for Timer 1
	
    // Set Prescale
    TIMER1->CTL &= ~(0xFF << 0);                                // clear current setting for Prescale
    TIMER1->CTL |= (11 << 0);                                   // Prescale = (11+1) = 12 -> TM1 Clock = HCLK / 12 = 1 MHz
	
    // Set TM1 operation mode to Periodic Mode
    TIMER1->CTL &= ~(0b11 << 27);                               // Clear current settings
    TIMER1->CTL |= (0b01 << 27);	                            // Periodic Mode

    // The behavior selection in periodic mode is Enabled.
    TIMER1->CTL |= (1 << 20);
	
    // Enable TM1 interrup flag TIF
    TIMER1->CTL |= (1 << 29);

    // Set timeout value 
    // Time-out = 100ms -> TM1 CMP = 100000 - 1
    TIMER1->CMP = 100000 - 1;
	
    // TM1 Start Counting
    TIMER1->CTL |= (1 << 30);
    
    // NVIC interrupt configuration
    NVIC->ISER[1] |= (1 << (33 - 32));		                    // Enable NVIC for the Timer 1 interrupt - 
                                                                // TM1_INT is bit 33 --> belong to ISER1 (32 to 63)
                                                                
    /* Clear Timer1_flag */
    Timer1_flag = 0;

    /* Reset Timer1_cnt */
    Timer1_cnt = 0;
}

void configTimer0() {
    // Set Prescale
    TIMER0->CTL &= ~(0xFF << 0);                                // clear current setting for Prescale
    TIMER0->CTL |= (11 << 0);                                   // Prescale = (11+1) = 12 -> TM1 Clock = HCLK / 12 = 1 MHz
	
    // Set TM1 operation mode to Periodic Mode
    TIMER0->CTL &= ~(0b11 << 27);                               // Clear current settings
    TIMER0->CTL |= (0b01 << 27);	                            // Periodic Mode

    // The behavior selection in periodic mode is Enabled.
    TIMER0->CTL |= (1 << 20);
	
    // Enable TM1 interrup flag TIF
    TIMER0->CTL |= (1 << 29);

    // Set timeout value 
    // Time-out = 1000ms -> TM1 CMP = 100000 - 1
    TIMER0->CMP = 1000000 - 1;
	
    // TM1 Start Counting
    TIMER0->CTL |= (1 << 30);
    
    // NVIC interrupt configuration
    NVIC->ISER[1] |= (1 << (32 - 32));		                    // Enable NVIC for the Timer 1 interrupt - 
                                                                // TM1_INT is bit 33 --> belong to ISER1 (32 to 63)
}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
    
    // Enable HXT
    enableHXT();
    
    // Enable LXT
    enableLXT();

    // Enable HIRC
    enableHIRC();

    /* Switch HCLK clock source to HXT */
    // Set HCLK to HXT
	CLK->CLKSEL0 &= (~(0x07 << 0));                             // Clear current settings  
    CLK->CLKSEL0 |= (0b000 << 0); 					            // Set a new value
    
    /* Switch HCLK clock source to HIRC */
    // Set HCLK to HXT
	CLK->CLKSEL0 &= (~(0x07 << 0));                             // Clear current settings  
    CLK->CLKSEL0 |= (0b111 << 0); 					            // Set a new value
    
    // Set HCLK Divider to 0
	CLK->CLKDIV0 &= (~0x0F);                                    // Clear current settings for HCLKDIV
	CLK->CLKDIV0 |= 0x00;			                            // Set new value
    
    //Configure PLL for 192 MHz
    CLK->PLLCTL = 0; // Reset PLLCTL
    CLK->PLLCTL &= (~(1 << 19));                                // PLL Source is HXT
    CLK->PLLCTL &= (~(1 << 16));                                // PLL is in normal mode
    
    // Configure PLL output frequency
    // FIN = 12 MHZ; FOUT = 192 MHZ
    // Choose:
    // NR = 2 -> INDIV = 1
    // NF = 32 -> FBDIV = 30
    // NO = 2 -> OUTDIV = "01"
    // CLK->PLLCTL |= (9 << 1);                                    // INDIV
    // CLK->PLLCTL |= (0 << 30);                                   // FBDIV
    // CLK->PLLCTL &= (14 << 0x01);                                // OUTDIV
    CLK->PLLCTL |= 0x821E;
    while (!(CLK->STATUS & PLLSTB));                            // Wait for PLL to stabilize

    // Select PLLFOUT for CPU
    CLK->CLKSEL0 &= ~(0x07 << 0);                             // Clear current settings for 
    CLK->CLKSEL0 |= (0x02 << 0);                              // CLKSEL0[2:0] = 0b010

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV |= (0x1 << 0) | (0x1 << 4);

    /* Select IP clock source */
    // UART 0 clock setting
	CLK->CLKSEL1 |= (0b00 << 24); 		                        // UART 0 clock source is 12 MHz external high speed Oscillator (HXT)
	CLK->CLKDIV0 &= ~(0xF << 8);		                        // Clock divider is 1
    
    
    // EADC Clock selection and configuration
    // EADC clock source is PCLK1 12 MHz
    CLK->CLKDIV0 &= ~(0x0FF << 16);                             // Clear current settings
    CLK->CLKDIV0 |= (7 << 16);                                  // EADC clock divider is (7+1) --> ADC clock is 12/8 = 1.5 MHz  
    
    //Config the timer 1
    configTimer1();
    
    // Config the timer 0
    configTimer0();

    /* Enable IP clock */
    //CLK_EnableModuleClock(UART0_MODULE);
    CLK->APBCLK0 |= (1 << 16);                                  // enable UART0 clock
    //CLK_EnableModuleClock(EBI_MODULE);
    CLK->AHBCLK  |= (1 << 3);                                   // enable EBI clock
    //CLK_EnableModuleClock(EADC_MODULE);
    CLK->APBCLK0 |= (1 << 28);                                  // enable EADC0 clock
    // TRNG peripheral clock
    CLK->APBCLK1 |= (1 << 25);                                  // enable TRNG clock

    // Turn off TRNG then set the clock prescaler then turn on TRNG
    TRNG->CTL &= ~(0x1 << 0);                         // Clear TRNG_CTL[0] to disable TRNG

    // Clock Prescaler - PSC for TRNG
    TRNG->CTL &= ~(0x7 << 2);                               // Clear current settings
    TRNG->CTL |= (0 << 2);                                  // Set new value for TRNG clock prescaler

    // Activate TRNG
    TRNG->ACT |= (1 << 0);
    while(!(TRNG->CTL & (1 << 7)));                       // Wait until TRNG is ready

    // Enable TRNG
    TRNG->CTL |= (1 << 0);                                // Set TRNG_CTL[0] to enable TRNG

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /*=== EBI (LCD module) mult-function pins ===*/
    /* EBI AD0~5 pins on PG.9~14 */
    SYS->GPG_MFPH &= ~((0xF << 4) | (0xF << 8) | (0xF << 12) | (0xF << 16) 
                        | (0xF << 20) | (0xF << 24));           // Clear multi-function settings for PG.9, PG.10, PG.11, PG.12, PG.13, PG.14
    SYS->GPG_MFPH |= ((2 << 4) | (2 << 8) | (2 << 12)           // Set PG.9, PG.10, PG.11, PG.12, PG.13, PG.14 
                      | (2 << 16) | (2 << 20) | (2 << 24));     // as EBI_AD0, EBI_AD1, EBI_AD2, EBI_AD3, EBI_AD4, EBI_AD5 pins

    /* EBI AD6, AD7 pins on PD.8, PD.9 */
    SYS->GPD_MFPH &= ~((0xF << 0) | (0xF << 4));                // Clear multi-function settings for PD.8, PD.9
    SYS->GPD_MFPH |= ((2 << 0) | (2 << 4));                     // Set PD.8, PD.9 as EBI_AD6, EBI_AD7 pins

    /* EBI AD8, AD9 pins on PE.14, PE.15 */
    SYS->GPE_MFPH &= ~((0xF << 24) | (0xF << 28));              // Clear multi-function settings for PE.14, PE.15
    SYS->GPE_MFPH |= ((2 << 24) | (2 << 28));                   // Set PD.8, PD.9 as EBI_AD8, EBI_AD9 pins

    /* EBI AD10, AD11 pins on PE.1, PE.0 */
    SYS->GPE_MFPL &= ~((0xF << 0) | (0xF << 4));                // Clear multi-function settings for PE.1, PE.0
    SYS->GPE_MFPL |= ((2 << 0) | (2 << 4));                     // Set PE.1, PE.0 as EBI_AD10, EBI_AD11 pins

    /* EBI AD12~15 pins on PH.8~11 */
    SYS->GPH_MFPH &= ~((0xF << 0) | (0xF << 4)                  // Clear multi-function settings for PH.8, PH.9, PH.10, PH.11
                        | (0xF << 8) | (0xF << 12));
    SYS->GPH_MFPH |= ((2 << 0) | (2 << 4)                       // Set PH.8, PH.9, PH.10, PH.11 
                      | (2 << 8) | (2 << 12));                  // as EBI_AD12, EBI_AD13, EBI_AD14, EBI_AD15 pins

    /* Configure PH.3 as Output mode for LCD_RS (use GPIO PH.3 to control LCD_RS) */
    // Set PH.3 as output
	PH->MODE &= (~(0x03 << 6)); 	                            // clear settings for PH.3 first
	PH->MODE |= (0x01 << 6); 			                        // Set: output push-pull mode
    PH3 = 1;

    /* EBI RD and WR pins on PE.4 and PE.5 */
    SYS->GPE_MFPL &= ~((0xF << 16) | (0xF << 20));              // Clear multi-function settings for PE.4 and PE.5
    SYS->GPE_MFPL |= ((2 << 16) | (2 << 20));                   // Set PE.4 and PE.5 as EBI_nWR, EBI_nRD pins

    /* EBI CS0 pin on PD.14 */
    SYS->GPD_MFPH &= ~(0xF << 24);                              // Clear multi-function settings for PD.14
    SYS->GPD_MFPH |= (2 << 24);                                 // Set PD.14 as EBI_nCS0 pins

    /* Configure PB.6 and PB.7 as Output mode for LCD_RST and LCD_Backlight */
    PB->MODE &= ~((0x03 << 12) | (0x03 << 14)); 	            // clear settings for PB.6 and PB.7
	PB->MODE |= ((0x01 << 12) | (0x01 << 14)); 			        // Set output push-pull mode for PB.6 and PB.7
    PB6 = 1;
    PB7 = 0;

    /*=== UART mult-function pins ===*/
    /* Set PD.2 and PD.3 multi-function pins for UART0 RXD, TXD */
    SYS->GPD_MFPL &= ~((0xF << 8) | (0xF << 12));               // Clear multi-function settings for PD.2 and PD.3
    SYS->GPD_MFPL |= ((9 << 8) | (9 << 12));                    // Set PD.2 and PD.3 as UART0_RXD and UART0_TXD pins
    
    //Config the joy stick
    configJoyStickDownLeft();
    configJoyStickUpRight();
                       
}