/*
 * DView_Operation_Center.c
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "DShared.h"
#include "DView.h"
#include "Styles/DView_Styles.h"
#include "CustomWidgets/threatzone/c_lv_threatzone.h"
#include <string.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	SubSector_ThreatMap             = 0,
	SubSector_DebugTable            = 1,
	SubSector_OwnImage              = 2,
	Subsector_ProhibitZone          = 3,
	Subsector_CriticalZone          = 4,
	Subsector_InterventionSlow      = 5,
	Subsector_AdvisorySlow          = 6,
	Subsector_Presence              = 7,
	SubSector_ThreatStart           = 8,
	Subsector_ThreatEnd             = SubSector_ThreatStart + DSHARED_MAX_THREATS + 1,
	SubSector_ThreatImageStart,
	Subsector_ThreatImageEnd        = SubSector_ThreatImageStart + DSHARED_MAX_THREATS + 1,
	Subsector_Unplacablecontainer,
	Subsector_UnplacableThreatStart,
	Subsector_UnPlacableThreatEnd   = Subsector_UnplacableThreatStart + DSHARED_MAX_THREATS + 1,
	Subsector_ThreatLabelStart,
	Subsector_ThreatLabelEnd        = Subsector_ThreatLabelStart + DSHARED_MAX_THREATS + 1,
	SubSector_End
} DView_Operation_Center_SubSections_t;

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
lv_obj_t* DView_Operation_Threats[DSHARED_MAX_THREATS];
lv_obj_t* DView_Operation_ThreatImages[DSHARED_MAX_THREATS];

/* Private function prototypes -----------------------------------------------*/
DShared_ErrorState_t DView_Operation_Center_UpdateThreat(DShared_ThreatControl_t* hShared_threat, DView_Sector_t* hView_Operation_Center, uint8_t id);
void DView_Operation_Center_DynamicZoneInit(DView_Sector_t* hView_Operation_Center);

/* Function definitions ------------------------------------------------------*/

/**
 * @brief  Initialises the center window for operation
 * @param	Pointer to the operational screen (parent)
 * @param	This sectors object
 * @retval None
 */
void DView_Operation_Center_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_Center)
{
	//Parent, object and children for left pane
	hView_Operation_Center->Object = lv_obj_create(parent);
	static lv_obj_t* children[SubSector_End];
	hView_Operation_Center->Children = children;
	hView_Operation_Center->ChildrenCount = SubSector_End;

	//Set object and grid position
	lv_obj_set_grid_cell(hView_Operation_Center->Object, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_obj_set_style_pad_all(hView_Operation_Center->Object, 0, 0);
	lv_obj_set_scrollbar_mode(hView_Operation_Center->Object, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(hView_Operation_Center->Object, 0, 0);
    lv_obj_set_style_pad_gap(hView_Operation_Center->Object, 0, 0);

	//Make sector transparent
	lv_obj_set_style_bg_color(hView_Operation_Center->Object, lv_obj_get_style_bg_color(parent, 0), 0);

	//Threat map
	children[SubSector_ThreatMap] = lv_obj_create(hView_Operation_Center->Object);
	lv_obj_set_size(children[SubSector_ThreatMap], LV_PCT(100), LV_PCT(100));
	lv_obj_set_style_bg_opa(children[SubSector_ThreatMap], LV_OPA_COVER, 0);
	lv_obj_set_scrollbar_mode(children[SubSector_ThreatMap], LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_all(children[SubSector_ThreatMap], 0, 0);

	//Debug table
	children[SubSector_DebugTable] = lv_table_create(hView_Operation_Center->Object);
	lv_table_set_row_cnt(children[SubSector_DebugTable], DSHARED_TABLE_MAX_ROW_COUNT);
	lv_table_set_col_cnt(children[SubSector_DebugTable], DSHARED_TABLE_MAX_COL_COUNT);
	lv_obj_add_flag(children[SubSector_DebugTable], LV_OBJ_FLAG_HIDDEN);

	//Myself
	children[SubSector_OwnImage] = lv_img_create(hView_Operation_Center->Object);
	lv_img_set_src(children[SubSector_OwnImage], DView_Images[DShared_Images_Vehicle_1]);
	lv_obj_center(children[SubSector_OwnImage]);

	//Threats
	for (uint8_t i = SubSector_ThreatStart; i < Subsector_ThreatEnd; i++)
	{
		children[i] = c_lv_threatzone_create(children[SubSector_ThreatMap]);
		lv_obj_add_flag(children[i], LV_OBJ_FLAG_HIDDEN);
	}

	//Unplacable threats
	children[Subsector_Unplacablecontainer] = lv_obj_create(children[SubSector_ThreatMap]);
	lv_obj_set_width(children[Subsector_Unplacablecontainer], LV_PCT(100));
	lv_obj_set_height(children[Subsector_Unplacablecontainer], LV_PCT(100));
	lv_obj_set_pos(children[Subsector_Unplacablecontainer], 85, 85);
	lv_obj_set_flex_flow(children[Subsector_Unplacablecontainer], LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(children[Subsector_Unplacablecontainer], LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_all(children[Subsector_Unplacablecontainer], 0, 0);
	lv_obj_set_style_pad_gap(children[Subsector_Unplacablecontainer], 5, 0);
	lv_obj_set_style_bg_opa(children[Subsector_Unplacablecontainer], LV_OPA_TRANSP, 0);

	for (uint8_t i = Subsector_UnplacableThreatStart; i < Subsector_UnPlacableThreatEnd; i++)
	{
		children[i] = lv_obj_create(children[Subsector_Unplacablecontainer]);
		lv_obj_add_flag(children[i], LV_OBJ_FLAG_HIDDEN);
		lv_obj_set_width(children[i], 150);
		lv_obj_set_height(children[i], 60);
		lv_obj_set_style_bg_color(children[i], lv_color_black(), 0);
		lv_obj_set_style_bg_opa(children[i], LV_OPA_30, 0);
		lv_obj_set_style_border_opa(children[i], LV_OPA_COVER, 0);
		lv_obj_set_style_border_width(children[i], 3, 0);
		lv_obj_set_style_pad_all(children[i], 5, 0);
	}

	//Threat images
	for (uint8_t i = SubSector_ThreatImageStart; i < Subsector_ThreatImageEnd; i++)
	{
		children[i] = lv_img_create(children[SubSector_ThreatMap]);
		lv_obj_add_flag(children[i], LV_OBJ_FLAG_HIDDEN);
	}

	//threat labels
	for (uint8_t i = Subsector_ThreatLabelStart; i < Subsector_ThreatLabelEnd; i++)
	{
		children[i] = lv_label_create(children[SubSector_ThreatMap]);
		lv_obj_add_flag(children[i], LV_OBJ_FLAG_HIDDEN);
		lv_obj_set_style_text_color(children[i], lv_color_white(), 0);
	}

	//Dynamic zone lines
	DView_Operation_Center_DynamicZoneInit(hView_Operation_Center);

	lv_obj_update_layout(parent);
}


/**
 * @brief  	Updates the center sector from the shared data struct
 * @note	Will first check if updates are available before updating
 * @param	Handle to the local center sector data
 * @param	Handle to the shared center sector data
 * @retval 	Error state that needs to propegate to the operational screen
 */
DShared_ErrorState_t DView_Operation_Center_Update(DView_Sector_t* hView_Operation_Center, DShared_Operation_Center_t* hShared_Operation_Center)
{
	if (!DShared_CheckUpdateRequest(&hShared_Operation_Center->Flags.Controller, &hShared_Operation_Center->Flags.View))
	{
		return DShared_ErrorState_None;
	}

	hShared_Operation_Center->Flags.ErrorState = DShared_ErrorState_None;

	//Update dynamic zones
	if(hShared_Operation_Center->ShowDynamicZones)
	{
		lv_obj_align_to(hView_Operation_Center->Children[Subsector_CriticalZone], hView_Operation_Center->Children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, - hShared_Operation_Center->DistanceCritical);
		lv_obj_align_to(hView_Operation_Center->Children[Subsector_InterventionSlow], hView_Operation_Center->Children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, - hShared_Operation_Center->DistanceInterventionSlow);
		lv_obj_align_to(hView_Operation_Center->Children[Subsector_AdvisorySlow], hView_Operation_Center->Children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, - hShared_Operation_Center->DistanceAdvisorySlow);
		lv_obj_align_to(hView_Operation_Center->Children[Subsector_Presence], hView_Operation_Center->Children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, - hShared_Operation_Center->DistancePresence);

		lv_obj_clear_flag(hView_Operation_Center->Children[Subsector_CriticalZone], LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(hView_Operation_Center->Children[Subsector_InterventionSlow], LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(hView_Operation_Center->Children[Subsector_AdvisorySlow], LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(hView_Operation_Center->Children[Subsector_Presence], LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(hView_Operation_Center->Children[Subsector_ProhibitZone], LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
		lv_obj_add_flag(hView_Operation_Center->Children[Subsector_CriticalZone], LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(hView_Operation_Center->Children[Subsector_InterventionSlow], LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(hView_Operation_Center->Children[Subsector_AdvisorySlow], LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(hView_Operation_Center->Children[Subsector_Presence], LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(hView_Operation_Center->Children[Subsector_ProhibitZone], LV_OBJ_FLAG_HIDDEN);
	}

	for (uint8_t i = 0; i < DSHARED_MAX_THREATS; i++)
	{
		if (hShared_Operation_Center->Threats[i].Type != DShared_ThreatType_None && hShared_Operation_Center->Threats[i].Zone != DShared_ThreatZone_None)
		{
			hShared_Operation_Center->Flags.ErrorState = DView_Operation_Center_UpdateThreat(&hShared_Operation_Center->Threats[i], hView_Operation_Center, i);

			if (hShared_Operation_Center->Flags.ErrorState != DShared_ErrorState_None)
			{
				return DShared_ErrorState_Screen_CheckSector;
			}
		}
		else
		{
			lv_obj_add_flag(hView_Operation_Center->Children[SubSector_ThreatStart + i], LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(hView_Operation_Center->Children[SubSector_ThreatImageStart + i], LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(hView_Operation_Center->Children[Subsector_UnplacableThreatStart + i], LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(hView_Operation_Center->Children[Subsector_ThreatLabelStart + i], LV_OBJ_FLAG_HIDDEN);
		}
	}

	//Reorder threats
	uint8_t objectCount = lv_obj_get_child_cnt(hView_Operation_Center->Children[SubSector_ThreatMap]);

	//First do the threat objects
	for(DShared_ThreatZone_t j = DShared_ThreatZone_None; j < DShared_ThreatZone_Count; j++)
	{
		for(uint8_t i = 0; i < DSHARED_MAX_THREATS; i++)
		{
			if(hShared_Operation_Center->Threats[i].Zone == j && hShared_Operation_Center->Threats[i].Type != DShared_ThreatType_None)
			{
				//Bring object to front
				lv_obj_move_to_index(hView_Operation_Center->Children[SubSector_ThreatStart + i], objectCount - 1);
			}
		}
	}

	//Now place the images, labels and borders for unplacable threats
	for(DShared_ThreatZone_t j = DShared_ThreatZone_None; j < DShared_ThreatZone_Count; j++)
	{
		for(uint8_t i = 0; i < DSHARED_MAX_THREATS; i++)
		{
			if(hShared_Operation_Center->Threats[i].Zone == j && hShared_Operation_Center->Threats[i].Type != DShared_ThreatType_None)
			{
				//Bring object to front
				lv_obj_move_to_index(hView_Operation_Center->Children[SubSector_ThreatImageStart + i], objectCount - 1);
				lv_obj_move_to_index(hView_Operation_Center->Children[Subsector_ThreatLabelStart + i], objectCount - 1);
				lv_obj_move_to_index(hView_Operation_Center->Children[Subsector_UnplacableThreatStart + i], objectCount - 1);
			}
		}
	}

	lv_obj_move_to_index(hView_Operation_Center->Children[Subsector_Unplacablecontainer], objectCount - 1);

	DShared_CompleteUpdate(&hShared_Operation_Center->Flags.Controller, &hShared_Operation_Center->Flags.View);

	return DShared_ErrorState_None;
}

/**
 * @brief  	Updates a specific threat zone and image
 * @param	A pointer to the threat control struct
 * @param   Pointer tp the threatmap parent object
 * @retval 	Error state for the center sector
 */
DShared_ErrorState_t DView_Operation_Center_UpdateThreat(DShared_ThreatControl_t* hShared_threat, DView_Sector_t* hView_Operation_Center, uint8_t id)
{
	//Get the threat object
	lv_obj_t* threatObject = hView_Operation_Center->Children[SubSector_ThreatStart + id];
	lv_obj_t* unplaceableObject = hView_Operation_Center->Children[Subsector_UnplacableThreatStart + id];
	lv_obj_t* imageObject = hView_Operation_Center->Children[SubSector_ThreatImageStart + id];
	lv_obj_t* labelObject = hView_Operation_Center->Children[Subsector_ThreatLabelStart + id];

	lv_point_t origin;
	lv_obj_t* threatMap = lv_obj_get_parent(threatObject);
	origin.x = lv_obj_get_content_width(threatMap) / 2;
	origin.y = lv_obj_get_content_height(threatMap) / 2;

	//Update distance, angle, confidence and origin
	c_lv_threatzone_set_angle(threatObject, hShared_threat->Angle);
	c_lv_threatzone_set_confidence(threatObject, hShared_threat->Confidence);
	c_lv_threatzone_set_origin(threatObject, origin);

	lv_color_t bg = { 0 };
	switch (hShared_threat->Zone)
	{
	case DShared_ThreatZone_None:
		bg = DVIEW_COLOR_GRAY;
		c_lv_threatzone_set_size_offset(threatObject, 0);
		break;

	case DShared_ThreatZone_Presence:
		bg = DVIEW_COLOR_GREEN;
		c_lv_threatzone_set_size_offset(threatObject, 0);
		break;

	case DShared_ThreatZone_Warn:
		bg = DVIEW_COLOR_YELLOW;
		c_lv_threatzone_set_size_offset(threatObject, 15);
		break;

	case DShared_ThreatZone_Slow:
		bg = DVIEW_COLOR_AMBER;
		c_lv_threatzone_set_size_offset(threatObject, 30);
		break;

	case DShared_ThreatZone_Stop:
		bg = DVIEW_COLOR_RED;
		c_lv_threatzone_set_size_offset(threatObject, 45);
		break;

	case DShared_ThreatZone_Prohibit:
		bg = DVIEW_COLOR_RED;
		c_lv_threatzone_set_size_offset(threatObject, 45);
		break;

	default:
		break;
	}

	lv_color_t current = lv_obj_get_style_bg_color(threatObject, LV_PART_INDICATOR);
	if (memcmp(&current, &bg, sizeof(lv_color_t)))
	{
		lv_obj_set_style_bg_color(threatObject, bg, LV_PART_INDICATOR);
	}

	//Update coords
	c_lv_threatzone_calculate_area((c_lv_threatzone_t*) threatObject);
	c_lv_threatzone_calculate_position((c_lv_threatzone_t*) threatObject);

    if (hShared_threat->Type == DShared_ThreatType_Person)
    {
        //Update the image
        if (lv_img_get_src(imageObject) != DView_Images[DShared_Images_Person])
        {
            lv_img_set_src(imageObject, DView_Images[DShared_Images_Person]);
            lv_img_set_zoom(imageObject, 256);
        }
    }
    else
    {
        //Update the image
        if (lv_img_get_src(imageObject) != DView_Images[DShared_Images_Vehicle_3])
        {
            lv_img_set_src(imageObject, DView_Images[DShared_Images_Vehicle_3]);
            lv_img_set_zoom(imageObject, 160);
        }
    }

	if (hShared_threat->Confidence > 180)
	{

		if (lv_obj_has_flag(unplaceableObject, LV_OBJ_FLAG_HIDDEN))
		{
			lv_obj_clear_flag(unplaceableObject, LV_OBJ_FLAG_HIDDEN);
		}

		lv_color_t current = lv_obj_get_style_border_color(unplaceableObject, 0);
		if (memcmp(&current, &bg, sizeof(lv_color_t)))
		{
			lv_obj_set_style_border_color(unplaceableObject, bg, 0);
		}

		//Image align
		lv_obj_align_to(imageObject, unplaceableObject, LV_ALIGN_TOP_LEFT, 0, 5);
		lv_obj_align_to(labelObject, imageObject, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
	}
	else
	{

        //Get the coordinate of the image
		uint16_t angleFromMe = hShared_threat->Angle % 360;
		double radians = angleFromMe * ((double)3.14159 / (double)180);
		double screenDistance;

		lv_img_set_angle(imageObject, hShared_threat->Heading * 10);

		screenDistance = hShared_threat->Distance;

		double x = screenDistance * sin(radians) + lv_obj_get_width(threatMap) / 2;
		double y = lv_obj_get_height(threatMap) / 2 - screenDistance * cos(radians);

		lv_obj_update_layout(imageObject);

		double x_offset = lv_obj_get_width(imageObject) / 2;
		double y_offset = lv_obj_get_height(imageObject) / 2;
		lv_obj_set_x(imageObject, x - x_offset);
		lv_obj_set_y(imageObject, y - y_offset);

        //Align label to image
        lv_obj_align_to(labelObject, imageObject, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	}

	char labelText[DSHARED_MAX_THREAT_NAME_LEN] = {0};
	sprintf(labelText, "%s", hShared_threat->Name);

	if (hShared_threat->Distance != 0xFFFF)
	{
		char distanceText[DSHARED_MAX_THREAT_NAME_LEN] = {0};
		sprintf(distanceText, "%d m", hShared_threat->Distance / 10);

		strcat(labelText, distanceText);
	}

	//Set the label text
	lv_label_set_text(labelObject, labelText);

	if (lv_obj_has_flag(threatObject, LV_OBJ_FLAG_HIDDEN))
	{
		lv_obj_clear_flag(threatObject, LV_OBJ_FLAG_HIDDEN);
	}
	if (lv_obj_has_flag(imageObject, LV_OBJ_FLAG_HIDDEN))
	{
		lv_obj_clear_flag(imageObject, LV_OBJ_FLAG_HIDDEN);
	}
	if (lv_obj_has_flag(labelObject, LV_OBJ_FLAG_HIDDEN))
	{
		lv_obj_clear_flag(labelObject, LV_OBJ_FLAG_HIDDEN);
	}

	return DShared_ErrorState_None;
}

/**
 * @brief  	Updates a specific threat zone and image
 * @param	A pointer to the threat control struct
 * @param   Pointer tp the threatmap parent object
 * @retval 	Error state for the center sector
 */
DShared_ErrorState_t DView_Operation_Center_UpdateUnplaceableThreat(DShared_ThreatControl_t* hShared_threat, DView_Sector_t* hView_Operation_Center, uint8_t id)
{
	//Get the threat object
	lv_obj_t* threatObject = hView_Operation_Center->Children[Subsector_UnplacableThreatStart + id];
	lv_obj_t* imageObject = hView_Operation_Center->Children[SubSector_ThreatImageStart + id];



	return DShared_ErrorState_None;
}

/**
 * @brief  	Draws the initial prohibit zone and dynamic zones
 * @param	A handle to the oeprational center view data
 * @note    After the init, these lines should be moved with lv_obj_align_to by moving the y coordinate (negative upward)
 * @retval 	None
 */
void DView_Operation_Center_DynamicZoneInit(DView_Sector_t* hView_Operation_Center)
{
	lv_obj_t** children = hView_Operation_Center->Children;
	static lv_point_t line_points[2] = { {0, 0}, {100, 0} };

	//Prohibit zone object
	children[Subsector_ProhibitZone] = lv_obj_create(hView_Operation_Center->Object);
	lv_obj_set_style_border_color(children[Subsector_ProhibitZone], DVIEW_COLOR_RED, 0);
	lv_obj_set_width(children[Subsector_ProhibitZone], 70);
	lv_obj_set_height(children[Subsector_ProhibitZone], 120);
	lv_obj_set_style_border_width(children[Subsector_ProhibitZone], 3, 0);
	lv_obj_align_to(children[Subsector_ProhibitZone], children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_bg_opa(children[Subsector_ProhibitZone], LV_OPA_TRANSP, 0);

	//Critical stop zone
	children[Subsector_CriticalZone] = lv_line_create(hView_Operation_Center->Object);
	lv_line_set_points(children[Subsector_CriticalZone], line_points, 2);
	lv_obj_set_style_line_color(children[Subsector_CriticalZone], DVIEW_COLOR_RED, 0);
	lv_obj_set_style_line_width(children[Subsector_CriticalZone], 3, 0);
	lv_obj_align_to(children[Subsector_CriticalZone], children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, -100);

	//Intervention
	children[Subsector_InterventionSlow] = lv_line_create(hView_Operation_Center->Object);
	lv_line_set_points(children[Subsector_InterventionSlow], line_points, 2);
	lv_obj_set_style_line_color(children[Subsector_InterventionSlow], DVIEW_COLOR_AMBER, 0);
	lv_obj_set_style_line_width(children[Subsector_InterventionSlow], 3, 0);
	lv_obj_align_to(children[Subsector_InterventionSlow], children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, -120);

	//Advisory
	children[Subsector_AdvisorySlow] = lv_line_create(hView_Operation_Center->Object);
	lv_line_set_points(children[Subsector_AdvisorySlow], line_points, 2);
	lv_obj_set_style_line_color(children[Subsector_AdvisorySlow], DVIEW_COLOR_YELLOW, 0);
	lv_obj_set_style_line_width(children[Subsector_AdvisorySlow], 3, 0);
	lv_obj_align_to(children[Subsector_AdvisorySlow], children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, -140);

	//Presence
	children[Subsector_Presence] = lv_line_create(hView_Operation_Center->Object);
	lv_line_set_points(children[Subsector_Presence], line_points, 2);
	lv_obj_set_style_line_color(children[Subsector_Presence], DVIEW_COLOR_GREEN, 0);
	lv_obj_set_style_line_width(children[Subsector_Presence], 3, 0);
	lv_obj_align_to(children[Subsector_Presence], children[SubSector_OwnImage], LV_ALIGN_CENTER, 0, -160);

	//Hide all
	lv_obj_add_flag(hView_Operation_Center->Children[Subsector_CriticalZone], LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(hView_Operation_Center->Children[Subsector_InterventionSlow], LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(hView_Operation_Center->Children[Subsector_AdvisorySlow], LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(hView_Operation_Center->Children[Subsector_Presence], LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(hView_Operation_Center->Children[Subsector_ProhibitZone], LV_OBJ_FLAG_HIDDEN);
}
/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
