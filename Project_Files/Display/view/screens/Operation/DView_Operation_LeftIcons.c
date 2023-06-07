/*
 * DView_Operation_LeftIcons.c
 *
 * Created on: 	15 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "DView_Operation.h"
#include "Inc/DView_Operation_LeftIcons.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
 * @brief  	Initialises the left icons sector
 * @param	Pointer to the operational screen (parent)
 * @param	This sectors object
 * @retval 	None
 */
void DView_Operation_LeftIcons_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_LeftIcons)
{
	hView_Operation_LeftIcons->Object = lv_obj_create(parent);
	lv_obj_set_grid_cell(hView_Operation_LeftIcons->Object, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
}
/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
