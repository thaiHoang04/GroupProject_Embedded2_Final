/******************************************************************************
 * @file     EBI_LCD_Module.c
 *
 * @brief    Subfunctins to display contents on LCD module thru EBI interface
 * @version  1.0.0
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NuMicro.h"

#include "EBI_LCD_Module.h"

/* LCD Module "RS" */
#define PH_CTRL_RS      1
#if PH_CTRL_RS
#define SET_RS          PH3 = 1;
#define CLR_RS          PH3 = 0;
#endif

/* LCD Module "RESET" */
#define SET_RST         PB6 = 1;
#define CLR_RST         PB6 = 0;

volatile    uint8_t     Timer1_flag = 0;
volatile    uint8_t     Timer1_cnt = 0;

volatile    uint32_t    g_u32AdcIntFlag_TP;

/**
 * @brief       TMR1 IRQ handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer1 default IRQ, declared in startup_M480.s
 */
void TMR1_IRQHandler(void)
{
    /* Clear timer interrupt flag */
    TIMER1->INTSTS |= (1 << 0);

    /* Set Timer3_flag = 1 */
    Timer1_flag = 1;

    /* Timer3_cnt + 1 */
    Timer1_cnt = Timer1_cnt + 1;

}
/**
 * @brief       Write command to LCD
 *
 * @param       cmd: the command will be written
 *
 * @return      None
 *
 * @details     To write command to LCD thru the EBI interface
 */
#if PH_CTRL_RS
void LCD_WR_REG(uint16_t cmd)
{
    CLR_RS
    EBI0_WRITE_DATA16(0x00000000, cmd);
    SET_RS
}
#else
void LCD_WR_REG(uint16_t cmd)
{
    EBI0_WRITE_DATA16(0x00000000, cmd);
}
#endif

/**
 * @brief       Write data to LCD
 *
 * @param       dat: the data will be written
 *
 * @return      None
 *
 * @details     To write data to LCD thru the EBI interface
 */
void LCD_WR_DATA(uint16_t dat)
{
    EBI0_WRITE_DATA16(0x00030000, dat);

}

/**
 * @brief       Read back data from LCD
 *
 * @param       None
 *
 * @return      The read back data
 *
 * @details     To read data from LCD thru the EBI interface
 */
uint16_t LCD_RD_DATA(void)
{
    return EBI0_READ_DATA16(0x00030000);

}

/**
 * @brief       Set LCD window
 *
 * @param       x_s: the start position in the x-direction of window
 *              x_e: the end position in the x-direction of window
 *              y_s: the start position in the y-direction of window
 *              y_e: the end position in the y-direction of window
 * @return      None
 *
 * @details     To set the window area of LCD module
 */
void LCD_SetWindow(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e)
{
    /* X range */
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(x_s>>8);
    LCD_WR_DATA(x_s);
    LCD_WR_DATA(x_e>>8);
    LCD_WR_DATA(x_e);

    /* Y range */
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(y_s>>8);
    LCD_WR_DATA(y_s);
    LCD_WR_DATA(y_e>>8);
    LCD_WR_DATA(y_e);

    /* Memory write */
    LCD_WR_REG(0x2C);

}

/**
 * @brief       Initial ILI9341 LCD driver
 *
 * @param       None
 *
 * @return      None
 *
 * @details     To initialize the ILI9341 LCD driver
 */
void ILI9341_Initial(void)
{
    /* Hardware reset */
    SET_RST;
    CLK_SysTickDelay(5000);     // Delay 5ms

    CLR_RST;
    CLK_SysTickDelay(20000);    // Delay 20ms

    SET_RST;
    CLK_SysTickDelay(40000);    // Delay 40ms

    /* Initial control registers */
    LCD_WR_REG(0xCB);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x02);

    LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC1);
    LCD_WR_DATA(0x30);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x85);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x78);

    LCD_WR_REG(0xEA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x81);

    LCD_WR_REG(0xF7);
    LCD_WR_DATA(0x20);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x23);

    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x10);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x3e);
    LCD_WR_DATA(0x28);

    LCD_WR_REG(0xC7);
    LCD_WR_DATA(0x86);

    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x18);

    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x82);
    LCD_WR_DATA(0x27);

    LCD_WR_REG(0xF2);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x26);
    LCD_WR_DATA(0x01);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x4E);
    LCD_WR_DATA(0xF1);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0xC1);
    LCD_WR_DATA(0x48);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x0F);

    LCD_WR_REG(0x11);
    CLK_SysTickDelay(200000);   // Delay 200ms

    LCD_WR_REG(0x29);           //Display on

    printf("Initial ILI9341 LCD Module done.\n\n");

}

/**
 * @brief       Put an 8x16 character to LCD screen
 *
 * @param       x: the start position in the x-direction
 *              y: the start position in the y-direction
 *              c: the character of ASCII
 *              fcolor: the front color
 *              bcolor: the background color
 * @return      None
 *
 * @details     To show an 8x16 character on the LCD screen
 */
void LCD_PutChar8x16(uint16_t x, uint16_t y, uint8_t c, uint32_t fColor, uint32_t bColor)
{
    uint32_t    i, j;
    uint8_t     m;

    for(i=0; i<16; i++) {
        m = Font8x16[c*16+i];
        LCD_SetWindow(x, x+7, y+i, y+i);
        for(j=0; j<8; j++) {
            if((m & 0x80) == 0x80) {
                LCD_WR_DATA(fColor);
            } else {
                LCD_WR_DATA(bColor);
            }
            m <<= 1;
        }
    }

}

/**
 * @brief       Put a string to LCD screen
 *
 * @param       x: the start position in the x-direction
 *              y: the start position in the y-direction
 *              *s: the point of string
 *              fcolor: the front color
 *              bcolor: the background color
 * @return      None
 *
 * @details     To show a string on the LCD screen
 */
void LCD_PutString(uint16_t x, uint16_t y, uint8_t *s, uint32_t fColor, uint32_t bColor)
{
    uint8_t     l = 0;

    while(*s) {
        if(*s < 0x80) {
            LCD_PutChar8x16(x+l*8, y, *s, fColor, bColor);
            s++;
            l++;
        }
    }

}

/**
 * @brief       Put a larger 16x32 number (0~9) to LCD screen
 *
 * @param       x: the start position in the x-direction
 *              y: the start position in the y-direction
 *              c: the number 0~9
 *              fcolor: the front color
 *              bcolor: the background color
 * @return      None
 *
 * @details     To show a larger 16x32 number on the LCD screen
 */
void LCD_PutChar16x32(uint16_t x, uint16_t y, uint16_t c, uint32_t fColor, uint32_t bColor)
{
    uint32_t    i, j;
    uint16_t    m;

    for(i=0; i<32; i++) {
        m = Font16x32[c*32+i];
        LCD_SetWindow(x, x+15, y+i, y+i);
        for(j=0; j<16; j++) {
            if((m & 0x8000) == 0x8000) {
                LCD_WR_DATA(fColor);
            } else {
                LCD_WR_DATA(bColor);
            }
            m <<= 1;
        }
    }

}

void LCD_Draw_Rectangle(uint16_t X, uint16_t Y, uint16_t W, uint16_t H) {
    LCD_BlankArea(X, Y, W, H, C_WHITE);
}

void LCD_Draw_Square(uint16_t X, uint16_t Y, uint16_t H, uint32_t fColor) {
    LCD_BlankArea(X, Y, H, H, fColor);
}

void LCD_Draw_Circle(uint16_t X, uint16_t Y, uint16_t R) {
    int16_t    i, j;
    
    LCD_SetWindow(X, X + (2* R), Y, Y+(2* R));
    for (i = R; i >= -R; i--) {
        for (j = -R; j <= R; j++) {
            if (i*i + j*j <= R*R) {
                LCD_WR_DATA(C_WHITE);
            } else {
                LCD_WR_DATA(C_BLACK);
            }
        }
    
    }
    
}

void LCD_Draw_Picture() {
    uint32_t i, j;
    int numOfPixel = 0;
    uint16_t m;    
    
    //Assign the value to the picture position.
    int x = 2;
    int y = 320;
    
    for (i = 0; i < 76800; i++) {
        m = image_data_320x240x16_2[i];
        LCD_Draw_Square(x, y, 1, m);
        
        //Updating the idx position of the pixel in the picture.
        numOfPixel++;
        
        if (numOfPixel == 320) {
            y = 320;
            numOfPixel = 0;
            x+=1;
        } else {
            y-=1;
        }
    }
}

void LCD_Draw_TetrisBlock(uint16_t X, uint16_t Y, uint16_t color) {
    int i, j;
    int x = X;
    int y = Y;
    uint16_t m;
    int numOfPixel = 0;
    
    
    for (i = 0; i < 100; i++) {
        if (color == 1) {
            m = skyBlueBlock[i];
        } else if (color == 2) {
            m = blueBlock[i];
        } else if (color == 3) {
            m = orangeBlock[i];
        } else if (color == 4) {
            m = yellowBlock[i];
        } else if (color == 5) {
            m = greenBlock[i];
        } else if (color == 6) {
            m = purpleBlock[i];
        } else if (color == 7){
            m = redBlock[i];
        } else if (color == 8) {
            m = grayBlock[i];
        } else {
            m = C_BLACK;
        }
        
        LCD_Draw_Square(x, y, 1, m);
        
        //Updating the idx position of the pixel in the picture.
        numOfPixel++;
        
        if (numOfPixel == 10) {
            x = X;
            y+=1;
            numOfPixel = 0;
        } else {
            x+=1;
        }
    }
}

//Display next character
void LCD_Display_Word(uint16_t X, uint16_t Y, int idxOfWord) {
    int i, j;
    int x = X;
    int y = Y;
    char m;
    int numOfPixel = 0;
    
    
    for (i = 0; i < 270; i++) {
        if (idxOfWord == 1) {
            m = next[i];
        } else if (idxOfWord == 2) {
            m = score[i];
        }
        
        if (m == '*') {
            LCD_Draw_Square(x, y, 1, C_BLACK);
        } else if (m == '$') {
            LCD_Draw_Square(x, y, 1, C_WHITE);
        } 
        
        //Updating the idx position of the pixel in the picture.
        numOfPixel++;
        
        if (numOfPixel == 27) {
            x = X;
            y+=1;
            numOfPixel = 0;
        } else {
            x+=1;
        }
    }
}

void LCD_Draw_GameField() {
    //Draw the horizontal line
    int i;
    int y = 10;
    for (i = 0; i < 30; i++) {
        LCD_Draw_TetrisBlock(0, y, 8); // The top border line
        LCD_Draw_TetrisBlock(230, y, 8); // The bottom border line
        LCD_Draw_TetrisBlock(160, y, 8); // The middle seperate line
        y+=10;
    }
    
    //Draw the vertical line
    int x = 0;
    for (i = 0; i < 24; i++) {
        LCD_Draw_TetrisBlock(x, 0, 8); // The right border line
        LCD_Draw_TetrisBlock(x, 310, 8); // The left border line
        x+=10;
    }
    
    //Draw the seperate line in the non-playable area
    x = 170;
    for (i = 0; i < 6; i++) {
        LCD_Draw_TetrisBlock(x, 80, 8); // The left border line
        LCD_Draw_TetrisBlock(x, 250, 8); // The left border line
        x+= 10;
    }
    
    LCD_Display_Word(185, 20, 1);
    
    LCD_Display_Word(180, 100, 2);
}

//Line Type
void LCD_Draw_BlockType1(uint16_t X, uint16_t Y, uint16_t direction) {
    int i = 0;
    // 1: horizontal, 2: vertical
    for (i = 0; i < 4; i++) {
        if (direction == 1) {
            LCD_Draw_TetrisBlock(X, Y + (i * 10), 1);
        } else {
            LCD_Draw_TetrisBlock(X + (i * 10), Y, 1); // The vertical line
        }
        
    }
}

//Square Type
void LCD_Draw_BlockType2(uint16_t X, uint16_t Y) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j <2; j++) {
            LCD_Draw_TetrisBlock(X + (j * 10), Y + (i * 10), 4); // The left border line
        }
        
    }
}

// right L type
void LCD_Draw_BlockType3(uint16_t X, uint16_t Y, uint16_t direction) {
    int i;
    // 1: horizontal with the L is on the top
    // 2: vertical with the L is down
    // 3: horizontal with the L is on the bottom
    // 4: normal direction
    if (direction == 1) {
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X, Y + (i * 10), 2); 
        }
        LCD_Draw_TetrisBlock(X + 10, Y, 2); 
    } else if (direction == 2) {
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y, 2); 
        }
        LCD_Draw_TetrisBlock(X + 20, Y + 10, 2); 
    } else if (direction == 3) {
        LCD_Draw_TetrisBlock(X, Y + 20, 2); 
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + 10, Y + (i * 10), 2); 
        }
    } else {
        LCD_Draw_TetrisBlock(X, Y, 2); 
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y + 10, 2); 
        }
    }
}

void LCD_Draw_BlockType4(uint16_t X, uint16_t Y, uint16_t direction) {
    int i;
    
    
    // 1: horizontal with the L is on the top
    // 2: vertical with the L is down
    // 3: horizontal with the L is on the bottom
    // 4: normal direction
    if (direction == 1) {
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X, Y + (i * 10), 3); 
        }
        LCD_Draw_TetrisBlock(X + 10, Y + 20, 3); 
    } else if (direction == 2) {
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y, 3); 
        }
        LCD_Draw_TetrisBlock(X, Y + 10, 3); 
    } else if (direction == 3) {
        LCD_Draw_TetrisBlock(X, Y, 3); 
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + 10, Y + (i * 10), 3); 
        }
    } else {
        LCD_Draw_TetrisBlock(X + 20, Y, 3); 
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y + 10, 3); 
        }
    }
}

void LCD_Draw_BlockType5(uint16_t X, uint16_t Y, uint16_t direction) {
    int i;
    
    if (direction == 1) {
        for (i = 0; i < 2; i++) {
            LCD_Draw_TetrisBlock(X, Y + (i * 10), 5); 
        }
        for (i = 1; i <= 2; i++) {
            LCD_Draw_TetrisBlock(X + 10, Y + (i * 10), 5); 
        }
    } else {
        for (i = 1; i < 3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y, 5); 
        }
        for (i = 0; i < 2; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y + 10, 5); 
        }
    }
}

void LCD_Draw_BlockType6(uint16_t X, uint16_t Y, uint16_t direction) {
    int i, j;
    if (direction == 1) {
        for (i = 0; i < 2; i++) {
            LCD_Draw_TetrisBlock(X + 10, Y + (i * 10), 7); 
        }
        for (i = 1; i <= 2; i++) {
            LCD_Draw_TetrisBlock(X, Y + (i * 10), 7); 
        }
    } else {
        for (i = 0; i < 2; i++) {
        LCD_Draw_TetrisBlock(X + (i * 10), Y, 7); 
        }
    
        for (j = 1; j < 3; j++) {
            LCD_Draw_TetrisBlock(X + (j * 10), Y + 10, 7); 
        }
    }
    
    
}

void LCD_Draw_BlockType7(uint16_t X, uint16_t Y, uint16_t direction) {
    int i, j;
    if (direction == 1) {
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X, Y + (i * 10), 6); 
        }
        LCD_Draw_TetrisBlock(X + 10, Y + 10, 6); 
    } else if (direction == 2) {
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y, 6); 
        }
        LCD_Draw_TetrisBlock(X + 10, Y + 10, 6); 
    } else if (direction == 3) {
        LCD_Draw_TetrisBlock(X, Y + 10, 6); 
        for (i = 0; i <3; i++) {
            LCD_Draw_TetrisBlock(X + 10, Y + (i * 10), 6); 
        }
    } else {
        LCD_Draw_TetrisBlock(X + 10, Y, 6); 
    
        for (i = 0; i < 3; i++) {
            LCD_Draw_TetrisBlock(X + (i * 10), Y + 10, 6); 
        }
    }
}

void LCD_Draw_A_Tetris_Block(uint16_t currentX, uint16_t currentY,uint16_t currentType, uint16_t currentDirection) {
    switch(currentType) {
        case 1:
            LCD_Draw_BlockType1(currentX, currentY, currentDirection);
            break;
        case 3:
            LCD_Draw_BlockType3(currentX, currentY, currentDirection);
            break;
        case 4:
            LCD_Draw_BlockType4(currentX, currentY, currentDirection);
            break;
        case 5:
            LCD_Draw_BlockType5(currentX, currentY, currentDirection);
            break;
        case 6:
            LCD_Draw_BlockType6(currentX, currentY, currentDirection);
            break;
        case 7: 
            LCD_Draw_BlockType7(currentX, currentY, currentDirection);
            break;
    }
}


/**
 * @brief       Blank an area of LCD screen
 *
 * @param       X: the X start position of this area, 0~239
 *              Y: the Y start position of this area, 0~319
 *              W: the width of this area, X+W < 240
 *              H: the height of this area, Y+H < 320
 *              color: the color will be filled into this area
 * @return      None
 *
 * @details     To fill a fixed color into an area on the LCD screen
 */
void LCD_BlankArea(uint16_t X, uint16_t Y, uint16_t W, uint16_t H, uint16_t color)
{
    uint16_t    i, j;

    LCD_SetWindow(X, X+W-1, Y, Y+H-1);
    for(j=0; j<H; j++) {
        for(i=0; i<W; i++)
            LCD_WR_DATA(color);
    }

}

/**
 * @brief       Get X position from touch panel thru the ADC input
 *
 * @param       None
 *
 * @return      The X position on LCD screen
 *
 * @details     To get the X position when finger touching on the LCD screen
 */
uint16_t Get_TP_X(void)
{
    uint16_t    x_adc_in;
    uint16_t    X_pos;

    /*=== Get X from ADC input ===*/
    PB9 = 1;
    PH4 = 0;
    GPIO_SetMode(PB, BIT9, GPIO_MODE_OUTPUT);   // XR
    GPIO_SetMode(PH, BIT4, GPIO_MODE_OUTPUT);   // XL
    GPIO_SetMode(PH, BIT5, GPIO_MODE_INPUT);    // YD

    /* Configure the GPB8 ADC analog input pins. */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB8MFP_Msk | SYS_GPB_MFPH_PB9MFP_Msk);
    SYS->GPB_MFPH |= SYS_GPB_MFPH_PB8MFP_EADC0_CH8;

    /* Disable the GPB8 digital input path to avoid the leakage current. */
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT8);

    /* Configure the sample module 1 for analog input channel 8 and software trigger source.*/
    EADC_ConfigSampleModule(EADC, 1, EADC_SOFTWARE_TRIGGER, 8); // YU

    /* Clear the A/D ADINT1 interrupt flag for safe */
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF1_Msk);

    /* Enable the sample module 1 interrupt. */
    EADC_ENABLE_INT(EADC, BIT1);    //Enable sample module A/D ADINT1 interrupt.
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 1, BIT1);    //Enable sample module 1 interrupt.
    NVIC_EnableIRQ(EADC01_IRQn);

    /* Reset the ADC interrupt indicator and trigger sample module 1 to start A/D conversion */
    g_u32AdcIntFlag_TP = 0;
    EADC_START_CONV(EADC, BIT1);

    /* Wait ADC interrupt (g_u32AdcIntFlag_TP will be set at IRQ_Handler function) */
    while(g_u32AdcIntFlag_TP == 0) {};
    x_adc_in = EADC_GET_CONV_DATA(EADC, 1)>>2;

    /*=== Calculate the X position ===*/
    X_pos = (x_adc_in - 170)/2.8;	// range of x_adc_in is [0:842]

    if(X_pos >= (LCD_W - 1)) X_pos = LCD_W - 1;

    printf("Position X: %d\n", X_pos);
    return X_pos;

}

/**
 * @brief       Get Y position from touch panel thru the ADC input
 *
 * @param       None
 *
 * @return      The Y position on LCD screen
 *
 * @details     To get the Y position when finger touching on the LCD screen
 */
uint16_t Get_TP_Y(void)
{
    uint16_t    y_adc_in;
    uint16_t    Y_pos;

    /*=== Get Y from ADC input ===*/
    PB8 = 1;
    PH5 = 0;
    GPIO_SetMode(PB, BIT8, GPIO_MODE_OUTPUT);   // YU
    GPIO_SetMode(PH, BIT5, GPIO_MODE_OUTPUT);   // YD
    GPIO_SetMode(PH, BIT4, GPIO_MODE_INPUT);    // XL

    /* Configure the GPB9 ADC analog input pins. */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB8MFP_Msk | SYS_GPB_MFPH_PB9MFP_Msk);
    SYS->GPB_MFPH |= SYS_GPB_MFPH_PB9MFP_EADC0_CH9;

    /* Disable the GPB9 digital input path to avoid the leakage current. */
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT9);

    /* Configure the sample module 2 for analog input channel 9 and software trigger source.*/
    EADC_ConfigSampleModule(EADC, 2, EADC_SOFTWARE_TRIGGER, 9); // XR

    /* Clear the A/D ADINT1 interrupt flag for safe */
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF1_Msk);

    /* Enable the sample module 2 interrupt. */
    EADC_ENABLE_INT(EADC, BIT2);    //Enable sample module A/D ADINT1 interrupt.
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 1, BIT2);    //Enable sample module 2 interrupt.
    NVIC_EnableIRQ(EADC02_IRQn);

    /* Reset the ADC interrupt indicator and trigger sample module 2 to start A/D conversion */
    g_u32AdcIntFlag_TP = 0;
    EADC_START_CONV(EADC, BIT2);

    /* Wait ADC interrupt (g_u32AdcIntFlag_TP will be set at IRQ_Handler function) */
    while(g_u32AdcIntFlag_TP == 0) {};
    y_adc_in = EADC_GET_CONV_DATA(EADC, 2)>>2;

    /*=== Calculate the Y position ===*/
			Y_pos = (y_adc_in - 150)/2.34; // range of y_adc_in is [0:898.8]

    if(Y_pos >= (LCD_H - 1)) Y_pos = LCD_H - 1;

    printf("Position Y : %d \n", Y_pos);
    return Y_pos;

}

/**
 * @brief       ADC01 IRQ handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The ADC01 default IRQ, declared in startup_M480.s
 */
void EADC01_IRQHandler(void)
{
    /* Clear the A/D ADINT1 interrupt flag */
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF1_Msk);

    g_u32AdcIntFlag_TP = 1;

}

void clearCurrentMove(uint16_t currentX, uint16_t currentY, uint8_t type, uint8_t direction) {
    switch(type) {
        case 1:
            if (direction == 1) {
                LCD_BlankArea(currentX, currentY, 10, 40, C_BLACK);
            } else {
                LCD_BlankArea(currentX, currentY, 40, 10, C_BLACK);
            }
            break;
        case 3:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY, 10, 10, C_BLACK);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 30, 10, C_BLACK);
                    LCD_BlankArea(currentX + 20, currentY + 10, 10, 10, C_BLACK);
                    break;
                case 3:
                    LCD_BlankArea(currentX + 10, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX, currentY + 20, 10, 10, C_BLACK);
                    break;
                case 4:
                    LCD_BlankArea(currentX, currentY + 10, 30, 10, C_BLACK);
                    LCD_BlankArea(currentX, currentY, 10, 10, C_BLACK);
                    break;
            }
            break;
        case 4:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY + 20, 10, 10, C_BLACK);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 30, 10, C_BLACK);
                    LCD_BlankArea(currentX, currentY + 10, 10, 10, C_BLACK);
                    break;
                case 3:
                    LCD_BlankArea(currentX + 10, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX, currentY, 10, 10, C_BLACK);
                    break;
                case 4:
                    LCD_BlankArea(currentX, currentY + 10, 30, 10, C_BLACK);
                    LCD_BlankArea(currentX + 20, currentY, 10, 10, C_BLACK);
                    break;
            }
            break;
        case 5:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 20, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY + 10, 10, 20, C_BLACK);
                    break;
                case 2:
                    LCD_BlankArea(currentX + 10, currentY, 20, 10, C_BLACK);
                    LCD_BlankArea(currentX, currentY + 10, 20, 10, C_BLACK);
                    break;
            }
            break;
        case 6:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY + 10, 10, 20, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY, 10, 20, C_BLACK);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 20, 10, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY + 10, 20, 10, C_BLACK);
                    break;
            }
            break;
        case 7:
            switch(direction) {
                case 1:
                    LCD_BlankArea(currentX, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY + 10, 10, 10, C_BLACK);
                    break;
                case 2:
                    LCD_BlankArea(currentX, currentY, 30, 10, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY + 10, 10, 10, C_BLACK);
                    break;
                case 3:
                    LCD_BlankArea(currentX + 10, currentY, 10, 30, C_BLACK);
                    LCD_BlankArea(currentX, currentY + 10, 10, 10, C_BLACK);
                    break;
                case 4:
                    LCD_BlankArea(currentX, currentY + 10, 30, 10, C_BLACK);
                    LCD_BlankArea(currentX + 10, currentY, 10, 10, C_BLACK);
                    break;
            }
            break;
    } 

};

void updateTheCurrentMove(uint16_t currentX, uint16_t currentY, uint16_t type, uint16_t direction);
