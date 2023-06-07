/*
 * DView_Operation_RightIcons.c
 *
 * Created on: 	15 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "DView_Operation.h"
#include "Inc/DView_Operation_RightIcons.h"
#include "Styles/DView_Styles.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
enum DView_Operation_LeftIcons_Subsectors
{
	Subsectors_ImageStart       = 0,
	Subsectors_ImageEnd    		= Subsectors_ImageStart + DShared_RightIcons_Count,

	Subsectors_ContainerStart,
	Subsectors_ContainerEnd		= Subsectors_ContainerStart + DShared_RightIcons_Count,

	Subsectors_CountersStart,
	Subsectors_CountersEnd 		= Subsectors_CountersStart + DShared_RightIcons_Count,

	Subsectors_count
};

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
 * @brief  	Initialises the right icons sector
 * @param	Pointer to the operational screen (parent)
 * @param	This sectors object
 * @retval 	None
 */
void DView_Operation_RightIcons_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_RightIcons)
{
	static lv_obj_t* children[Subsectors_count];
	hView_Operation_RightIcons->Children = children;

	hView_Operation_RightIcons->Object = lv_obj_create(parent);
	lv_obj_set_grid_cell(hView_Operation_RightIcons->Object, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_obj_set_flex_flow(hView_Operation_RightIcons->Object, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(hView_Operation_RightIcons->Object, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(hView_Operation_RightIcons->Object, 0, 0);
	lv_obj_set_style_pad_gap(hView_Operation_RightIcons->Object, 10, 0);

	for (uint8_t i = 0; i < DShared_RightIcons_Count; i++)
	{
		children[Subsectors_ContainerStart + i] = lv_obj_create(hView_Operation_RightIcons->Object);
		lv_obj_set_style_pad_all(children[Subsectors_ContainerStart + i], 5, 0);
		lv_obj_set_width(children[Subsectors_ContainerStart + i], LV_PCT(100));
		lv_obj_set_height(children[Subsectors_ContainerStart + i], 60);
		lv_obj_set_scrollbar_mode(children[Subsectors_ContainerStart + i], LV_SCROLLBAR_MODE_OFF);
		lv_timer_t* temptimer = lv_timer_create(DView_ObjectFlashCB, 1000, children[Subsectors_ContainerStart + i]);
		lv_timer_pause(temptimer);
		lv_obj_set_user_data(children[Subsectors_ContainerStart + i], temptimer);
		lv_obj_add_flag(children[Subsectors_ContainerStart + i], LV_OBJ_FLAG_HIDDEN);

		children[Subsectors_ImageStart + i] = lv_img_create(children[Subsectors_ContainerStart + i]);
		lv_obj_align(children[Subsectors_ImageStart + i], LV_ALIGN_TOP_MID, 0, 0);

		children[Subsectors_CountersStart + i] = lv_label_create(children[Subsectors_ContainerStart + i]);
		lv_obj_set_style_text_color(children[Subsectors_CountersStart + i], lv_color_white(), 0);
		lv_obj_align(children[Subsectors_CountersStart + i], LV_ALIGN_BOTTOM_RIGHT, 0, 0);
		lv_obj_set_style_text_font(children[Subsectors_CountersStart + i], &lv_font_montserrat_12, 0);
	}

	//Ped RF
	lv_img_set_src(children[Subsectors_ImageStart + DShared_RightIcons_VTP], DView_Images[DShared_Images_Icon_PedCount]);
	lv_obj_t* templabelVTP = lv_label_create(children[Subsectors_ContainerStart + DShared_RightIcons_VTP]);
	lv_obj_set_style_text_color(templabelVTP, lv_color_white(), 0);
	lv_obj_align(templabelVTP, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_style_text_font(templabelVTP, &lv_font_montserrat_12, 0);
	lv_label_set_text(templabelVTP, "P");

	//Vehicle RF
	lv_img_set_src(children[Subsectors_ImageStart + DShared_RightIcons_VTV], DView_Images[DShared_Images_Icon_VehicleCount]);
	lv_obj_t* templabelVTV = lv_label_create(children[Subsectors_ContainerStart + DShared_RightIcons_VTV]);
	lv_obj_set_style_text_color(templabelVTV, lv_color_white(), 0);
	lv_obj_align(templabelVTV, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_style_text_font(templabelVTV, &lv_font_montserrat_12, 0);
	lv_label_set_text(templabelVTV, "V");

	//Sat count
	lv_img_set_src(children[Subsectors_ImageStart + DShared_RightIcons_Sat], DView_Images[DShared_Images_Icon_SatCount]);

	//Excl count
	lv_img_set_src(children[Subsectors_ImageStart + DShared_RightIcons_Excl], DView_Images[DShared_Images_Icon_ExclCount]);

	//Connection
	lv_img_set_src(children[Subsectors_ImageStart + DShared_RightIcons_Connection], DView_Images[DShared_Images_Icon_Connection]);
}

/**
 * @brief  	Updates the right icons
 * @param	Handle to the local right icon data (View)
 * @param	Handle to the local right icon data (Shared)
 * @retval 	Error state that needs to propegate to the operational screen
 */
DShared_ErrorState_t DView_Operation_RightIcons_Update(DView_Sector_t* hView_Operation_RightIcons, DShared_Operation_RightIcons_t* hShared_Operation_RightIcons)
{
	if(!DShared_CheckUpdateRequest(&hShared_Operation_RightIcons->Flags.Controller, &hShared_Operation_RightIcons->Flags.View))
	{
		return DShared_ErrorState_None;
	}

	hShared_Operation_RightIcons->Flags.ErrorState = DShared_ErrorState_None;

	for(uint8_t i = 0; i < DShared_RightIcons_Count; i++)
	{
		if(hShared_Operation_RightIcons->Images[i].Source == DShared_Images_None)
		{
			lv_obj_add_flag(hView_Operation_RightIcons->Children[Subsectors_ContainerStart + i], LV_OBJ_FLAG_HIDDEN);
		}
		else if(hShared_Operation_RightIcons->Images[i].FlashEnabled == false)
		{
			lv_obj_clear_flag(hView_Operation_RightIcons->Children[Subsectors_ContainerStart + i], LV_OBJ_FLAG_HIDDEN);
		}

		if(lv_img_get_src(hView_Operation_RightIcons->Children[Subsectors_ImageStart + i]) != DView_Images[hShared_Operation_RightIcons->Images[i].Source])
		{
			lv_img_set_src(hView_Operation_RightIcons->Children[Subsectors_ImageStart + i], DView_Images[hShared_Operation_RightIcons->Images[i].Source]);
		}

		//Get handle to timer
		lv_timer_t* hTimer = lv_obj_get_user_data(hView_Operation_RightIcons->Children[Subsectors_ContainerStart + i]);
		if(hShared_Operation_RightIcons->Images[i].FlashEnabled)
		{
			lv_timer_set_period(hTimer, hShared_Operation_RightIcons->Images[i].FlashInterval);
			lv_timer_resume(hTimer);
		}
		else
		{
			lv_timer_pause(hTimer);

			if(hShared_Operation_RightIcons->Images[i].Source != DShared_Images_None)
			{
				lv_obj_clear_flag(hView_Operation_RightIcons->Children[Subsectors_ContainerStart + i], LV_OBJ_FLAG_HIDDEN);
			}
		}

		//Check if label is required
		if(!hShared_Operation_RightIcons->DisplayCount[i])
		{
			lv_obj_add_flag(hView_Operation_RightIcons->Children[Subsectors_CountersStart + i], LV_OBJ_FLAG_HIDDEN);
		}
		else
		{
			lv_obj_clear_flag(hView_Operation_RightIcons->Children[Subsectors_CountersStart + i], LV_OBJ_FLAG_HIDDEN);
		}
	}

	/*--- Pedestrian counts ---*/
	for(uint8_t i = 0; i < DShared_RightIcons_Count; i++)
	{
		lv_label_set_text_fmt(hView_Operation_RightIcons->Children[Subsectors_CountersStart + i], "%d", hShared_Operation_RightIcons->Counts[i]);
	}

	DShared_CompleteUpdate(&hShared_Operation_RightIcons->Flags.Controller, &hShared_Operation_RightIcons->Flags.View);

	return DShared_ErrorState_None;
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
