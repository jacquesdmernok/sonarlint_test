/*
 * DView_Operation_Header.c
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "DShared.h"
#include "DView_Operation.h"
#include "DView.h"
#include "Inc/DView_Operation_Header.h"
#include "lvgl.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
enum DView_Operation_Header_Subsectors
{
	Subsectors_Label 	= 0,

	Subsectors_Count
};

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
 * @brief  	Initialises the header for operation
 * @param	Pointer to the operational screens object (parent)
 * @param	The header sector
 * @retval 	None
 */
void DView_Operation_Header_Init(lv_obj_t* parent, 	DView_Sector_t* hView_Operation_Header)
{
	//Parent, object and children for left pane
	hView_Operation_Header->Object = lv_obj_create(parent);
	static lv_obj_t* children[Subsectors_Count];
	hView_Operation_Header->Children = children;
	hView_Operation_Header->ChildrenCount = Subsectors_Count;

	//Set object and grid position
	lv_obj_set_grid_cell(hView_Operation_Header->Object, LV_GRID_ALIGN_STRETCH, 1, 3, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_style_pad_all(hView_Operation_Header->Object, 0, 0);

	lv_color_t bg = lv_color_make(10, 10, 12);
	lv_obj_set_style_bg_color(hView_Operation_Header->Object, bg, 0);
	lv_obj_set_style_bg_opa(hView_Operation_Header->Object, LV_OPA_COVER, 0);
	lv_obj_set_style_pad_hor(hView_Operation_Header->Object, 15, 0);
	lv_obj_set_scrollbar_mode(hView_Operation_Header->Object, LV_SCROLLBAR_MODE_OFF);

	//Create the labels
	children[Subsectors_Label] = lv_label_create(hView_Operation_Header->Object);
	lv_obj_set_align(children[Subsectors_Label], LV_ALIGN_LEFT_MID);
	lv_obj_set_style_text_color(children[Subsectors_Label], lv_color_white(), 0);
	lv_obj_set_style_text_font(children[Subsectors_Label], &lv_font_montserrat_22, 0);
}

/**
 * @brief  	Updates the header from the shared data struct
 * @note	Will first check if updates are available before updating
 * @param	Handle to the local header data
 * @param	Handle to the shared header data
 * @retval 	Error state that needs to propegate to the operational screen (Not the internal header error state)
 */
DShared_ErrorState_t DView_Operation_Header_Update(DView_Sector_t*	hView_Operation_Header, DShared_Operation_Header_t* hShared_Operation_Header)
{
	if(!DShared_CheckUpdateRequest(&hShared_Operation_Header->Flags.Controller, &hShared_Operation_Header->Flags.View))
	{
		return DShared_ErrorState_None;
	}

	hShared_Operation_Header->Flags.ErrorState = DShared_ErrorState_None;

	//Update the header with constant strings
	uint8_t cs = 0;
	char string[sizeof(hShared_Operation_Header->text)*2] = {0};
	while(hShared_Operation_Header->constantStrings[cs] != DShared_ConstantStrings_None)
	{
		strncat(string, DView.ConstantStrings[DShared.Language][hShared_Operation_Header->constantStrings[cs]], sizeof(string) - strlen(string) - 1);
		cs++;
	}

	//Add the custom string
	if(strlen(hShared_Operation_Header->text) != 0)
	{
		strncat(string, hShared_Operation_Header->text, sizeof(string) - strlen(string) - 1);
	}

	//Check for overflow
	if(strlen(string) == sizeof(string) - 1)
	{
		hShared_Operation_Header->Flags.ErrorState = DShared_ErrorState_Sector_StringOverflow;
		return DShared_ErrorState_Screen_CheckSector;
	}

	//Check for empty string
	if(strlen(string) == 0)
	{
		hShared_Operation_Header->Flags.ErrorState = DShared_ErrorState_Sector_EmptyString;
		return DShared_ErrorState_Screen_CheckSector;
	}

	//Check for same string
	if (strncmp(lv_label_get_text(hView_Operation_Header->Children[Subsectors_Label]), string, sizeof(string) - 1))
	{
		lv_label_set_text(hView_Operation_Header->Children[Subsectors_Label], string);
		lv_obj_set_style_text_color(hView_Operation_Header->Children[Subsectors_Label], lv_color_white(), 0);
	}

	DShared_CompleteUpdate(&hShared_Operation_Header->Flags.Controller, &hShared_Operation_Header->Flags.View);

	return DShared_ErrorState_None;
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
