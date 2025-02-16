/*
 * Source File : STM_LCD.c  (16*2 LCD Display)
 *
 *  Created on: Feb 16, 2025
 *      Author: Navier
 *
 *
 *      Pin Connection
 *      STM32F4    ->     LCD
 *      PB0        ->     RS
 *      PB1        ->     RW
 *      PB2        ->     EN
 *      PA0-PA7    ->    D0-D7  Data Lines
 *
 *      RW(Read/Write) Pin
 *      0 - Write Operation
 *      1 - Read Operation
 *
 *      RS(Register Select) Pin
 *      0 - Command
 *      1 - Data
 *
 *      EN(Enable) Pin
 *      A low to high pulse of 3ms
 *
 */

// Include header file for STM32F401CCU6 clock configuration
#include "STM_Config.h"
#include "stm32f401xc.h"



// Initialize the pins for LCD
void GPIO_Init()
{
	// Set up the user clock configuration
	STM_ClockConfig();


	// Initialize the ports for LCD 16*2 LCD Display
	// Enable the clock to GPIO Port A and B
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Set the pin PA0-PA7 as output mode
	GPIOA -> MODER |= 0x00005555;
	GPIOB -> MODER |= 0x00000015;

	// Set EN and RW pin as low
	GPIOB -> BSRR |= (0x1 << 17);
	GPIOB -> BSRR |= (0x1 << 18);
}


void LCD_Command(unsigned char command)
{
	// RS = 0 RW = 0
	GPIOB -> BSRR |= (1 << 16);      // Clear the RS(16) and RW(17) pin
	GPIOB -> BSRR |= (1 << 17);
	GPIOA -> ODR = command;
	GPIOB -> BSRR |= (0x1 << 2);     // Set enable pin
	STM_DelayMS(30);
	GPIOB -> BSRR |= (0x1 << 18);    // Clear enable pin
	STM_DelayMS(30);
}

void LCD_Data(unsigned char data)
{
	// RS = 1 RW = 0
	GPIOB -> BSRR |= (0x1 << 0);     // Set the RS pin
	GPIOB -> BSRR |= (0x1 << 17);    // Clear the RW pin
	GPIOA -> ODR = data;
	GPIOB -> BSRR |= (0x1 << 2);     // Set enable pin
	STM_DelayMS(50);
	GPIOB -> BSRR |= (0x1 << 18);    // Clear enable pin
	STM_DelayMS(50);
}


void LCD_Init()
{
	// Initialize the pin for LCD display
		GPIO_Init();

	// Select 2 Lines and 5*7 matrix
		LCD_Command(0x38);
		STM_DelayMS(20);

	// Display on Cursor off
		LCD_Command(0x0C);
		STM_DelayMS(20);

	// Increment cursor (Shift cursor to right)
		LCD_Command(0x06);
		STM_DelayMS(20);

	// Clear Display screen
		LCD_Command(0x01);
		STM_DelayMS(20);

	// Force cursor to the beginning (1st line)
		LCD_Command(0x80);
		STM_DelayMS(50);
}

void LCD_Clear()
{
	// Clear Display Screen
	LCD_Command(0x01);
	STM_DelayMS(2);
}

void LCD_PutCursor(int row, int col)
{
    switch (row)
    {
        case 0:
           LCD_Command(0x80);       // Put cursor in the beginning of 1st row

            break;
        case 1:
           LCD_Command(0xC0);       // Put cursor in the beginning of 2nd row
            break;
    }
}

void LCD_Send_String(char *str)
{
	while (*str)
	{
		// Send characters to LCD
		LCD_Data(*str++);
	}

}
