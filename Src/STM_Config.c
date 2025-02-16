/*                                     MOSWARE SYSTEMS DEVELOPMENT TEAM
 *
 * Title : Clock Configuration File
 * Source File : STM_Config.c
 *
 * MCU : STM32F401CCU6
 * Core Frequency(max) : 84 MHz
 * Processor : ARM Cortex M4
 *
 * Clock Frequency fed to peripherals and modules
 * APB1 Peripheral Clock Frequency : 42 MHz
 * APB1 Timer Clock Frequency : 84 MHz
 * APB2 Peripheral Clock Frequency: 84 MHz
 * APB2 Timer Clock Frequency :  84 MHz
 *
 *
 *
 *
 *  Created on: Feb 13, 2025
 *      Author: Navier
 */


#include "stm32f401xc.h"


#define PLLM   25
#define PLLN   168
#define PLLP   3


void STM_ClockConfig(void)
{
	// Enable the HSE clock and wait for it to become ready
	RCC -> CR |= RCC_CR_HSEON;
	while(!(RCC -> CR & (1 << 17)));

	// Enable the power interface and voltage scaling
	RCC -> APB1ENR |= RCC_APB1ENR_PWREN;
    PWR -> CR |= (0x2 << 14);

    // Set the latency and enable cache related bits and set voltage scaling as mode 2
    FLASH -> ACR |= (FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN);
    FLASH -> ACR |= FLASH_ACR_LATENCY_2WS;

    // PLL Configuration registers setting
    RCC -> PLLCFGR |= (PLLM << 0);
    RCC -> PLLCFGR |= (PLLN << 6);
    RCC -> PLLCFGR &= ~(PLLP << 16);

    // Select the PLL source as HSE clock
    RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

    // Select the prescalers for AHB,APB1 and APB2 bus
    RCC -> CFGR &= ~(RCC_CFGR_HPRE_Msk);
    RCC -> CFGR |= RCC_CFGR_PPRE1_2;
    RCC -> CFGR &= ~(RCC_CFGR_PPRE2_Msk);

    // Enable the main PlL and wait for it to become ready
    RCC -> CR |= RCC_CR_PLLON;
    while(!(RCC -> CR & RCC_CR_PLLRDY));

    // Select PLL as system clock and wait for it to become ready
    RCC -> CFGR |= RCC_CFGR_SW_PLL;
    while((RCC -> CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_PLL));

    // Timer 2 is used in delay function

    // Enable the clock to Timer 2
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Set the counter as upcounter
    TIM2 -> CR1 &= ~(0x1 << 4);

    // Divide the APB1 Timer clock by 84 for 1 us delay
    TIM2 -> PSC = 84-1;

    // Set the ARR to the max value
    TIM2 -> ARR = 0xFFFF;

    // Enable the counter
    TIM2 -> CR1 |= (0x1 << 0);

    while(!(TIM2 -> SR & (0x1 <<0)));
}


void STM_DelayUS(int us)
{
	// Reset the counter
	TIM2 -> CNT = 0;

	// Wait for the counter to reach the desired value
	while(TIM2 -> CNT < us);
}

void STM_DelayMS(int ms)
{
	//
	for(uint16_t i = 0 ; i < ms ;i++)
	{
		STM_DelayUS(1000);
	}
}
