/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC18xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_libcfg_default.h"
#include "debug_frmwrk.h"
#include "lpc18xx_cgu.h"


#include <string.h>
#include <stdio.h>




/* Example group ----------------------------------------------------------- */
/** @defgroup Gpio_Input	Gpio_Input
 * @ingroup GPIO_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/

#define LED1_BIT				11
#define LED1_PORT				1

#define PIN_INT_PORT			0
#define PIN_INT_BIT				7

#define PIN_INT_NUM				0
#define INT_ID					PIN_INT0_IRQn


/************************** PRIVATE FUNCTIONS *************************/

void GPIO0_IRQHandler(void);
void delay (void);
void print_menu(void);


/************************** PRIVATE VARIABLES *************************/
uint8_t intMode;
uint32_t msec;
const uint8_t menu[] =
"********************************************************************************\n\r"
        "NGX Technologies \n\r"
        "GPIO External Interrupt demo \n\r"
        "\t - MCU: LPC18xx \n\r"
        "\t - Core: ARM CORTEX-M3 \n\r"
#ifdef UART_DEBUG_MESSEGES
		"\t - Communicate via: UART0 - 9600 bps \n\r"
#else
		"\t - Communicate via: USB VCOM \n\r"
#endif
        "This example used to configure the\n\r "
        "GPIO as source of external Interrupt \n\r"
        "********************************************************************************\n\r";

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu);
}


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		GPIO interrupt 0 handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint32_t int_flag=0;

void GPIO0_IRQHandler(void)
{
  if(GPIO_GetIntStatus(PIN_INT_NUM))
	 {
			GPIO_ClearInt(intMode,1<<PIN_INT_NUM);
			if (!(GPIO_ReadValue(0) & (1<<7)))
			{
				int_flag = 1;
			}
	 }
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/

int c_entry (void) {                       /* Main Program                       */

	SystemInit();
	CGU_Init();

	/* Initialize debug via UART1
	 * � 9600bps
	 * � 8 data bit
	 * � No parity
	 * � 1 stop bit
	 * � No flow control
	 */
	debug_frmwrk_init();


	/* Configure pin function */
	scu_pinmux(0x2 ,11 , MD_PDN, FUNC0); 					//LED1 P2_11
	scu_pinmux(0x2 ,7 ,(MD_PLN|MD_EZI|MD_ZI), FUNC0);		//P2.7 : G_INT

	/* Set direction for GPIO port */
	GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), 1);
	GPIO_SetDir(PIN_INT_PORT,(1<<PIN_INT_BIT), 0);			//G_INT farnell design

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(INT_ID, ((0x01<<3)|0x01));
	intMode = 1;
	/* Configure GPIO interrupt */
	GPIO_IntCmd(PIN_INT_NUM,PIN_INT_PORT,PIN_INT_BIT,intMode); //GPIO0[7] : P2_7

	/* Enable interrupt for Pin Interrupt */
	NVIC_EnableIRQ(INT_ID);
	while(int_flag != 1);
	print_menu();
	while (1)
	{
		if(int_flag == 1)
		{
			_DBG("USER_SWITCH is Pressed\r\n");
			LPC_GPIO_PORT->NOT[LED1_PORT] =(1<<LED1_BIT);
			int_flag = 0;
		}
	}						 // Loop forever
}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif
