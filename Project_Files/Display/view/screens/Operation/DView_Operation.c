/*
 * DView_Operation.c
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 * Admin: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "DView_Operation.h"
#include "DShared.h"
#include "DView.h"
#include "Styles/DView_Styles.h"

#include "Inc/DView_Operation_Header.h"
#include "Inc/DView_Operation_Footer.h"
#include "Inc/DView_Operation_LeftEdge.h"
#include "Inc/DView_Operation_LeftIcons.h"
#include "Inc/DView_Operation_Center.h"
#include "Inc/DView_Operation_RightIcons.h"
#include "Inc/DView_Operation_RightEdge.h"
#include "lvgl.h"


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum DView_Operation_SM_Index
{
	SM_NONE,

	SM_HEADER,
	SM_FOOTER,
	SM_LEFTEDGE,
	SM_LEFTICONS,
	SM_CENTER,
	SM_RIGHTICONS,
	SM_RIGHTEDGE,

	SM_COMPLETE
} DView_Operation_SM_Index_t;

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void DView_Operation_Activate(void);

/* Function definitions ------------------------------------------------------*/

/**
 * @brief  Initialises the operational view
 * @retval None
 */
void DView_Operation_Init(DView_ScreenData_t* hView_Operation)
{
	/*--- Screen object ---*/
	hView_Operation->Object = lv_obj_create(NULL);
	lv_obj_set_style_bg_color(hView_Operation->Object, lv_color_make(94, 102, 115), 0);
	lv_obj_set_style_pad_all(hView_Operation->Object, 5, 0);
	lv_obj_set_style_pad_gap(hView_Operation->Object, 3, 0);
	lv_obj_set_style_border_width(hView_Operation->Object, 0, 0);

	/*--- Sector objects ---*/
	static DView_Sector_t OperationalSectors[DView_Operation_SectorCount];
	hView_Operation->Sectors = OperationalSectors;
	hView_Operation->SectorCount = DView_Operation_SectorCount;

	/*--- Grid definition ---*/
	static lv_coord_t column_dsc[] = {130, 50, LV_PCT(100), 50, 130, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {40, LV_PCT(100), 40, LV_GRID_TEMPLATE_LAST };
	lv_obj_set_grid_dsc_array(hView_Operation->Object, column_dsc, row_dsc);

	/*--- Sectors ---*/
	DView_Operation_Header_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_Header]);

	DView_Operation_Footer_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_Footer]);

	DView_Operation_LeftEdge_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_LeftEdge]);

	DView_Operation_RightEdge_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_RightEdge]);

	DView_Operation_LeftIcons_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_LeftIcons]);

	DView_Operation_RightIcons_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_RightIcons]);

    DView_Operation_Center_Init(hView_Operation->Object, &hView_Operation->Sectors[DView_Operation_Sector_Center]);

	hView_Operation->State = DView_ScreenState_Inactive;
}

/**
 * @brief  	Parses the operation update requests
 * @retval 	bool: true if update is complete (Does not mean it was succesful)
 */
DView_UpdateStatus_t DView_Operation_SM(DView_ScreenData_t* hView_Operation, DShared_OperationScreen_t* hShared_Operation)
{
	if(!DShared_CheckUpdateRequest(&hShared_Operation->Flags.Controller, &hShared_Operation->Flags.View))
	{
		hView_Operation->SMIndex = SM_NONE;
		return DView_UpdateStatus_Complete;
	}

	switch((DView_Operation_SM_Index_t) hView_Operation->SMIndex)
	{
	case SM_NONE:
	{
		hShared_Operation->Flags.ErrorState = DShared_ErrorState_None;
		break;
	}
	case SM_HEADER:
	{
		hShared_Operation->Flags.ErrorState = DView_Operation_Header_Update(&hView_Operation->Sectors[DView_Operation_Sector_Header], &hShared_Operation->Header);
		break;
	}
	case SM_FOOTER:
	{
		hShared_Operation->Flags.ErrorState = DView_Operation_Footer_Update(&hView_Operation->Sectors[DView_Operation_Sector_Footer], &hShared_Operation->Footer);
		break;
	}
	case SM_LEFTEDGE:
	{
		hShared_Operation->Flags.ErrorState = DView_Operation_LeftEdge_Update(&hView_Operation->Sectors[DView_Operation_Sector_LeftEdge], &hShared_Operation->LeftEdge);
		break;
	}
	case SM_LEFTICONS:
	{
		break;
	}
	case SM_CENTER:
	{
		hShared_Operation->Flags.ErrorState = DView_Operation_Center_Update(&hView_Operation->Sectors[DView_Operation_Sector_Center], &hShared_Operation->Center);
		break;
	}
	case SM_RIGHTICONS:
	{
		hShared_Operation->Flags.ErrorState = DView_Operation_RightIcons_Update(&hView_Operation->Sectors[DView_Operation_Sector_RightIcons], &hShared_Operation->RightIcons);
		break;
	}
	case SM_RIGHTEDGE:
	{
		hShared_Operation->Flags.ErrorState = DView_Operation_RightEdge_Update(&hView_Operation->Sectors[DView_Operation_Sector_RightEdge], &hShared_Operation->RightEdge);
		break;
	}
	case SM_COMPLETE:
	{
		DShared_CompleteUpdate(&hShared_Operation->Flags.Controller, &hShared_Operation->Flags.View);
		hView_Operation->SMIndex = SM_NONE;
		return DView_UpdateStatus_Complete;
		break;
	}
	default:
	{
		hShared_Operation->Flags.ErrorState = DShared_ErrorState_Sector_Undefined;
		break;
	}
	}

	//If any errors, skip to complete
	if(hShared_Operation->Flags.ErrorState != DShared_ErrorState_None)
	{
		hView_Operation->SMIndex = SM_COMPLETE;
	}
	else
	{
		hView_Operation->SMIndex++;
	}

	return DView_UpdateStatus_Busy;
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
