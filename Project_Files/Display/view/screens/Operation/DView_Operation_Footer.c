/*
 * DView_Operation_Footer.c
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "DShared.h"
#include "DView_Operation.h"
#include "Inc/DView_Operation_Footer.h"
#include "Styles/DView_Styles.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
enum DView_Operation_Footer_Subsectors
{
	Subsectors_Date 		= 0,
	Subsectors_Speed 		= 1,
	Subsectors_Time 		= 2,

	Subsectors_Count
};

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
 * @brief  Initialises the footer for operation
 * @param	Pointer to the operational screen (parent)
 * @param	This sectors object
 * @retval None
 */
void DView_Operation_Footer_Init(lv_obj_t* parent, 	DView_Sector_t* hView_Operation_Footer)
{
	//Parent, object and children for left pane
	hView_Operation_Footer->Object = lv_obj_create(parent);
	static lv_obj_t* children[Subsectors_Count];
	hView_Operation_Footer->Children = children;
	hView_Operation_Footer->ChildrenCount = Subsectors_Count;

	//Set object and grid position
	lv_obj_set_grid_cell(hView_Operation_Footer->Object, LV_GRID_ALIGN_STRETCH, 1, 3, LV_GRID_ALIGN_STRETCH, 2, 1);
	lv_obj_set_style_pad_all(hView_Operation_Footer->Object, 0, 0);

	//Make sector transparent
	lv_obj_set_style_bg_opa(hView_Operation_Footer->Object, LV_OPA_TRANSP, 0);

	//Define the grid
	static lv_coord_t column_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_dsc_array(hView_Operation_Footer->Object, column_dsc, row_dsc);

	//Define the children
	lv_color_t bg = lv_color_make(10, 10, 12);
	for(uint8_t i = 0; i < Subsectors_Count; i++)
	{
		//Create temporary objects (we dont need to keep reference to them, but they are required to align the labels)
		lv_obj_t* tempObject = lv_obj_create(hView_Operation_Footer->Object);
		lv_obj_set_grid_cell(tempObject, LV_GRID_ALIGN_STRETCH, i, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
		lv_obj_set_style_bg_color(tempObject, bg, 0);
		lv_obj_set_style_bg_opa(tempObject, LV_OPA_COVER, 0);
		lv_obj_set_style_pad_hor(tempObject, 15, 0);
		lv_obj_set_scrollbar_mode(tempObject, LV_SCROLLBAR_MODE_OFF);

		//Create the labels
		children[i] = lv_label_create(tempObject);
		lv_obj_set_style_text_color(children[i], lv_color_white(), 0);
		lv_obj_set_style_text_font(children[i], &lv_font_montserrat_22, 0);
	}

	lv_obj_align(children[Subsectors_Date], LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_align(children[Subsectors_Speed], LV_ALIGN_CENTER, 0, 0);
	lv_obj_align(children[Subsectors_Time], LV_ALIGN_RIGHT_MID, 0, 0);
}

/**
 * @brief  	Updates the footer from the shared data struct
 * @note	Will first check if updates are available before updating
 * @param	Handle to the local footer data
 * @param	Handle to the shared footer data
 * @retval 	Error state that needs to propegate to the operational screen (Not the internal footer error state)
 */
DShared_ErrorState_t DView_Operation_Footer_Update(DView_Sector_t*	hView_Operation_Footer, DShared_Operation_Footer_t* hShared_Operation_Footer)
{
	if(!DShared_CheckUpdateRequest(&hShared_Operation_Footer->Flags.Controller, &hShared_Operation_Footer->Flags.View))
	{
		return DShared_ErrorState_None;
	}

	hShared_Operation_Footer->Flags.ErrorState = DShared_ErrorState_None;

	/*--- Date ---*/
	static char date[20];
	sprintf(date, "2022/08/25");

    //Check for same string
    if (strncmp(lv_label_get_text(hView_Operation_Footer->Children[Subsectors_Date]), date, sizeof(date) - 1))
    {
        lv_label_set_text(hView_Operation_Footer->Children[Subsectors_Date], date);
    }

	/*--- Speed ---*/
	uint8_t major = hShared_Operation_Footer->Speed / 10;
	uint8_t minor = hShared_Operation_Footer->Speed % 10;
    char speed[20];
    sprintf(speed, "%d.%d km/h", major, minor);

    //Check for same string
    if (strncmp(lv_label_get_text(hView_Operation_Footer->Children[Subsectors_Speed]), speed, sizeof(speed) - 1))
    {
        lv_label_set_text(hView_Operation_Footer->Children[Subsectors_Speed], speed);
    }

	switch(hShared_Operation_Footer->SpeedStyle)
	{
	case DShared_Style_Default:
	{
		lv_obj_set_style_text_color(hView_Operation_Footer->Children[Subsectors_Speed], DVIEW_COLOR_WHITE, 0);
		break;
	}
	case DShared_Style_Warn:
	{
		lv_obj_set_style_text_color(hView_Operation_Footer->Children[Subsectors_Speed], DVIEW_COLOR_AMBER, 0);
		break;
	}
	case DShared_Style_Error:
	{
		lv_obj_set_style_text_color(hView_Operation_Footer->Children[Subsectors_Speed], DVIEW_COLOR_RED, 0);
		break;
	}
	}

	/*--- Time ---*/
    char time[20];
    sprintf(time, "%02d:%02d:%02d", hShared_Operation_Footer->Hours, hShared_Operation_Footer->Minutes, hShared_Operation_Footer->Seconds);

    //Check for same string
    if (strncmp(lv_label_get_text(hView_Operation_Footer->Children[Subsectors_Time]), time, sizeof(time) - 1))
    {
        lv_label_set_text(hView_Operation_Footer->Children[Subsectors_Time], time);
    }
    
	DShared_CompleteUpdate(&hShared_Operation_Footer->Flags.Controller, &hShared_Operation_Footer->Flags.View);

	return DShared_ErrorState_None;
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
