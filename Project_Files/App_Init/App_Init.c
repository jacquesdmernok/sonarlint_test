/*=================================
App_Init.c
Created on: 	16 Nov 2022
Company: 		Mernok Elektronik
Author: 		H. du Plessis
==================================*/

/*--------------------------------
 *Local Includes
 *------------------------------*/
#include "App_Init.h"
#include "Flash.h"
#include "SharedMemory.h"
#include "SDRAM.h"
#include "LCD.h"
#include "LCDDraw.h"
#include "ltdc.h"
#include "../LVGL/_LVGL.h"
#include "LCDText.h"
#include "FreeRTOS.h"
#include "DView.h"

/*--------------------------------
 *Local Variables
 *------------------------------*/


/*--------------------------------
 *Prototype Functions
 *------------------------------*/

/*--------------------------------
 *Functions
 *------------------------------*/




void App_Init()
{
	while(!Shared_CM4.LCD.IsDSI_Ready)
	{

	}

	LCD_Init(LCD_Type_7InchAmpire);

	LCDDraw_ClearScreen(LCD_COLOR_BLACK);

	LVGL_Init();
//
//	BaseType_t Status;
//	Status = xTaskCreate(DView_Task, "DView", 200, &DView, 1, &hTask_DView);
//	configASSERT(Status == pdPASS);
}
