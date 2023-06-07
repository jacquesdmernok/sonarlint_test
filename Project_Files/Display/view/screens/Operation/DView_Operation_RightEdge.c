/*
 * DView_Operation_RightEdge.c
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "Inc/DView_Operation_RightEdge.h"
#include "DView_Operation.h"
#include "DShared.h"
#include "DView.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
enum DView_Operation_RightEdge_Subsectors
{
	Subsectors_Button_0 = 0,
	Subsectors_Button_1 = 1,
	Subsectors_Button_2	= 2,
	Subsectors_Button_3	= 3,

	Subsectors_count
};

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Initialises the right ebutton area for operation
  * @param	Pointer to the operational screen (parent)
  * @param	This sectors object
  * @retval None
  */
void DView_Operation_RightEdge_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_RightEdge)
{
	//Parent, object and children for left pane
	hView_Operation_RightEdge->Object = lv_obj_create(parent);
	static lv_obj_t* children[Subsectors_count];
	hView_Operation_RightEdge->Children = children;
	hView_Operation_RightEdge->ChildrenCount = Subsectors_count;

	//Set object and grid position
	lv_obj_set_grid_cell(hView_Operation_RightEdge->Object, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 0, 3);
	lv_obj_set_style_pad_all(hView_Operation_RightEdge->Object, 0, 0);

    //Make sector transparent
    lv_obj_set_style_bg_color(hView_Operation_RightEdge->Object, lv_obj_get_style_bg_color(parent, 0), 0);

    lv_color_t bg = lv_color_make(10, 10, 12);

	//Define the rows
	static lv_coord_t column_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_dsc_array(hView_Operation_RightEdge->Object, column_dsc, row_dsc);

	//Define the children
	for(uint8_t i = 0; i < Subsectors_count; i++)
	{
		lv_obj_t* tempContainer = lv_obj_create(hView_Operation_RightEdge->Object);
		lv_obj_set_grid_cell(tempContainer, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, i, 1);
		lv_obj_set_style_bg_color(tempContainer, bg, 0);
		lv_obj_set_scrollbar_mode(tempContainer, LV_SCROLLBAR_MODE_OFF);

		children[i] = lv_img_create(tempContainer);
		lv_timer_t* temptimer = lv_timer_create(DView_ObjectFlashCB, 1000, children[i]);
		lv_timer_pause(temptimer);
		lv_obj_set_user_data(children[i], temptimer);
	}
}

/**
 * @brief  	Updates the right edge
 * @note	The right edge "children" are not the images, they are the parents to tge images
 * @param	Handle to the local right edge data
 * @param	Handle to the shared right edge data
 * @retval 	Error state that needs to propegate to the operational screen
 */
DShared_ErrorState_t DView_Operation_RightEdge_Update(DView_Sector_t* hView_Operation_RightEdge, DShared_Operation_RightEdge_t* hShared_Operation_RightEdge)
{
	if(!DShared_CheckUpdateRequest(&hShared_Operation_RightEdge->Flags.Controller, &hShared_Operation_RightEdge->Flags.View))
	{
		return DShared_ErrorState_None;
	}

	hShared_Operation_RightEdge->Flags.ErrorState = DShared_ErrorState_None;

	for(uint8_t i = 0; i < Subsectors_count; i++)
	{
		hShared_Operation_RightEdge->Flags.ErrorState = DView_UpdateImage(hView_Operation_RightEdge->Children[i], &hShared_Operation_RightEdge->Images[i]);

		if(hShared_Operation_RightEdge->Flags.ErrorState != DShared_ErrorState_None)
		{
			return DShared_ErrorState_Screen_CheckSector;
		}
	}

	DShared_CompleteUpdate(&hShared_Operation_RightEdge->Flags.Controller, &hShared_Operation_RightEdge->Flags.View);

	return DShared_ErrorState_None;
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
