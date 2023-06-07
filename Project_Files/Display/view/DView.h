/*
 * DView.h
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_DVIEW_H_
#define DISPLAY_VIEW_DVIEW_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DShared.h"
#include "Images/DView_Images.h"
#include "lvgl.h"

#ifdef FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum DView_SM_State
{
	DVIEW_STATE_NONE		= 0,
	DVIEW_STATE_INIT		= 1,
	DVIEW_STATE_IDLE		= 2,
	DVIEW_STATE_UPDATE 		= 4

} DView_SM_State_t;

typedef enum DView_ScreenState
{
	DView_ScreenState_Uninitialised		= 0,
	DView_ScreenState_Inactive			= 1,
	DView_ScreenState_Active			= 2

} DView_ScreenState_t;

typedef enum DView_UpdateStatus
{
	DView_UpdateStatus_Complete 	= 0,
	DView_UpdateStatus_Busy 		= 1

} DView_UpdateStatus_t;

typedef struct DView_Sector
{
	lv_obj_t*		Object;
	lv_obj_t** 		Children;
	uint8_t			ChildrenCount;

} DView_Sector_t;

typedef struct DView_ScreenData
{
	DView_ScreenState_t			State;
	uint8_t						SMIndex;
	lv_obj_t*					Object;
	DView_Sector_t*				Sectors;
	uint8_t						SectorCount;

} DView_ScreenData_t;

typedef struct DView
{
	DView_SM_State_t		SM;
	DView_ScreenData_t		Screens[DShared_Screen_End];
	DShared_Screens_t		ActiveScreenIndex;
	char*					ConstantStrings[DShared_Language_Count][DShared_ConstantStrings_Count];
} DView_t;

/* Exported variables --------------------------------------------------------*/
extern DView_t DView;

#ifdef FREERTOS
extern TaskHandle_t hTask_DView;
#endif

/* Exported functions ------------------------------------------------------- */
void DView_Init(DView_t* hView );
void DView_Task(void* parameters);
void DView_SM(DView_t* hView, DShared_t* hShared);
DShared_ErrorState_t DView_ActivateScreen(DView_t* hView, DShared_Screens_t Shared_ActiveScreen);
DShared_ErrorState_t DView_UpdateImage(lv_obj_t* image, DShared_ImageControl_t* imageControl);
uint32_t DView_Elapsed(uint32_t timeStamp);
uint32_t DView_Timestamp();
void DView_ObjectFlashCB(lv_timer_t* timer);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_DVIEW_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
