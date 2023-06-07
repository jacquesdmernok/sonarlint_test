/*================================
Statemachine.h
Created on: 	19 May 2023
Company: 		Mernok Elektronik
Author: 		H. du Plessis
=================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/*--------------------------------
 *Local Includes
 *------------------------------*/

/*--------------------------------
 *Local Defines
 *------------------------------*/

/*--------------------------------
 *Extern Variables
 *------------------------------*/

/*--------------------------------
 *Local Variables
 *------------------------------*/

/*--------------------------------
 *Function Prototypes
 *------------------------------*/

/*--------------------------------
 *Functions
 *------------------------------*/
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	Error_Handler();
	// Handle the stack overflow error
	// ...
}
