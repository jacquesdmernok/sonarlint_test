
/*=================================
Statemachine.c
Created on: 	29 Nov 2022
Company: 		Mernok Elektronik
Author: 		H. du Plessis
==================================*/

/*--------------------------------
 *Local Includes
 *------------------------------*/
#include "Statemachine_Display.h"
#include "lvgl.h"

/*--------------------------------
 *Local Variables
 *------------------------------*/

/*--------------------------------
 *Prototype Functions
 *------------------------------*/

/*--------------------------------
 *Functions
 *------------------------------*/
void SM_Display_Control()
{
	lv_task_handler();
//	lv_timer_handler_run_in_period(1);
}
