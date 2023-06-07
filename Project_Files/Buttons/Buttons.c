/*=================================
Buttons.c
Created on: 	Dec 21, 2022
Company: 		Mernok Elektronik
Author: 		H du Plessis
=================================*/

/*--------------------------------
 *Includes
 *------------------------------*/
#include "Buttons.h"

/*--------------------------------
 *Defines
 *------------------------------*/

/*--------------------------------
 *Extern Variables
 *------------------------------*/
Button_HandlerTydedef HButtons;

/*--------------------------------
 *Static Variables
 *------------------------------*/

/*--------------------------------
 *Prototype Functions
 *------------------------------*/
void Buttons_Control(void);

/*--------------------------------
 *Functions
 *------------------------------*/

void Buttons_Init()
{
	// === Create task ===
	//BaseType_t Status = xTaskCreate(Buttons_Task, "Buttons", 200, &,3, &);
	//configASSERT(Status == pdPASS);

}

void Buttons_Task(void* parameters)
{
	while(1)
	{
		Buttons_Control();
	}

	vTaskDelete(NULL);
}

void Buttons_Control(void)
{
	ulTaskNotifyTakeIndexed(2, pdTRUE, portMAX_DELAY);

	if(HButtons.Button1)
	{

	}
	if(HButtons.Button2)
	{

	}
	if(HButtons.Button3)
	{

	}
	if(HButtons.Button4)
	{

	}


}
