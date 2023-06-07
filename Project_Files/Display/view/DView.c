/*
 * DView.c
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 * Admin: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "Screens/Operation/DView_Operation.h"
#include "DView.h"
#include "Styles/DView_Styles.h"
#include "Styles/DView_Themes.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DView_t DView;

/* Private function prototypes -----------------------------------------------*/
void DView_Update(DView_t* hView, DShared_t* hShared);

/* Function definitions ------------------------------------------------------*/

#ifdef FREERTOS

TaskHandle_t hTask_DView;

/**
 * @brief  	Used if FREERTOS is enabled
 * @note  	Also initialises all screens (i.e. operation, settings etc.)
 * @param	void pointer that should point to the ViewData struct
 * @retval 	None
 */
void DView_Task(void* parameters)
{
	DView_t* hView = (DView_t*) parameters;
	DShared_t* hShared = &DShared;

	while(1)
	{
		DView_SM(hView, hShared);
	}

	vTaskDelete(NULL);
}
#endif

/**
 * @brief  Initialises the view and all screens, themes and styles
 * @param 	Pointer to the viewData struct
 * @note  	Also initialises all screens (i.e. operation, settings etc.)
 * @retval None
 */
void DView_Init(DView_t* hView)
{
	DView_Style_Init();

	DView_Theme_Init();

	DView_Images_Init();

	DView_Strings_Init(hView);

	DView_Operation_Init(&hView->Screens[DShared_Screen_Operation]);
}

/**
 * @brief  	The main SM for the View
 * @param 	Pointer to the viewData struct
 * @retval 	None
 */
void DView_SM(DView_t* hView, DShared_t* hShared)
{
	if(hShared->ActiveScreen != DShared_Screen_None && DView.Screens[hShared->ActiveScreen].State == DView_ScreenState_Active)
	{
		lv_timer_handler_run_in_period(2);
	}

	if(!DShared_Lock(&hShared->ViewLock, &hShared->ControlLock))
	{
		return;
	}

	if(hShared->Flags.ErrorState != DShared_ErrorState_None)
	{
		hView->SM = DVIEW_STATE_IDLE;
	}

	switch(hView->SM)
	{
	case DVIEW_STATE_NONE:
	{
		hView->SM = DVIEW_STATE_INIT;
		break;
	}
	case DVIEW_STATE_INIT:
	{
		DView_Init(hView);
		hView->SM = DVIEW_STATE_IDLE;
		break;
	}
	case DVIEW_STATE_IDLE:
	{
		if(DShared_CheckUpdateRequest(&hShared->Flags.Controller, &hShared->Flags.View))
		{
			hView->SM = DVIEW_STATE_UPDATE;
			hShared->Flags.ErrorState = DShared_ErrorState_None;
		}
		else
		{
			DShared_Unlock(&hShared->ViewLock);
		}

		break;
	}
	case DVIEW_STATE_UPDATE:
	{
		DView_Update(hView, hShared);
		break;
	}
	default:
	{
		hShared->Flags.ErrorState = DShared_ErrorState_InternalViewError;
		hView->SM = DVIEW_STATE_IDLE;
		break;
	}

	}
}

/**
 * @brief  	Parses the shared data struct to populate the view with new data
 * @retval 	None
 */
void DView_Update(DView_t* hView, DShared_t* hShared)
{
	DView_UpdateStatus_t updateStatus = DView_UpdateStatus_Complete;

	/*--- Force activate the active screen ---*/
	hShared->Flags.ErrorState = DView_ActivateScreen(hView, hShared->ActiveScreen);
	if(hShared->Flags.ErrorState != DShared_ErrorState_None)
	{
		DShared_CompleteUpdate(&hShared->Flags.Controller, &hShared->Flags.View);
		return;
	}

	/*--- Check for language ---*/
	if(hShared->Language == DShared_Language_None)
	{
		DShared_CompleteUpdate(&hShared->Flags.Controller, &hShared->Flags.View);
		hShared->Flags.ErrorState = DShared_ErrorState_LanguageNotDefined;
		return;
	}

	/*--- Operational Screen ---*/
	updateStatus |= DView_Operation_SM(&hView->Screens[DShared_Screen_Operation], &hShared->OperationalScreen);
	if(hShared->OperationalScreen.Flags.ErrorState != DShared_ErrorState_None)
	{
		hShared->Flags.ErrorState = DShared_ErrorState_CheckScreen;
	}

	/*--- Complete update ---*/
	if(updateStatus == DView_UpdateStatus_Complete)
	{
		DShared_CompleteUpdate(&hShared->Flags.Controller, &hShared->Flags.View);
		hView->SM = DVIEW_STATE_IDLE;
	}
}

/**
 * @brief  	Activates the new screen
 * @note	The screen must be initialised for it to be activated
 * @retval	Error status of the activation
 */
DShared_ErrorState_t DView_ActivateScreen(DView_t* hView, DShared_Screens_t Shared_ActiveScreen)
{
	if(hView->ActiveScreenIndex == Shared_ActiveScreen)
	{
		return DShared_ErrorState_None;
	}

	if(hView->Screens[Shared_ActiveScreen].State == DView_ScreenState_Uninitialised)
	{
		return DShared_ErrorState_ScreenUninitialised;
	}

	//Set the previous screen to inactive
	if(hView->Screens[hView->ActiveScreenIndex].State == DView_ScreenState_Active)
	{
		hView->Screens[hView->ActiveScreenIndex].State = DView_ScreenState_Inactive;
	}

	lv_scr_load(hView->Screens[Shared_ActiveScreen].Object);
	hView->Screens[Shared_ActiveScreen].State = DView_ScreenState_Active;
	hView->ActiveScreenIndex = Shared_ActiveScreen;

	return DShared_ErrorState_None;
}

/**
 * @brief  	Updates all image parameters
 * @note	Also handles periodic flash etc.
 * @param	lv_obj* to the image
 * @param	The shared image parameters
 * @retval 	Error state that needs to propegate to the operational screen
 */
DShared_ErrorState_t DView_UpdateImage(lv_obj_t* image, DShared_ImageControl_t* imageControl)
{
	if (imageControl->Source == DShared_Images_None && lv_img_get_src(image) != NULL)
	{
		if(!lv_obj_has_flag(image, LV_OBJ_FLAG_HIDDEN))
		{
			lv_obj_add_flag(image, LV_OBJ_FLAG_HIDDEN);
		}

		void* userData = lv_obj_get_user_data(image);
		if(userData != NULL)
		{
			lv_timer_pause(userData);
		}

		return DShared_ErrorState_None;
	}

	if(lv_img_get_src(image) != DView_Images[imageControl->Source])
	{
		lv_img_set_src(image, DView_Images[imageControl->Source]);
		lv_obj_center(image);
	}

	if (imageControl->Zoom != 0)
	{
		lv_img_set_zoom(image, imageControl->Zoom);
	}

	lv_img_set_angle(image, imageControl->Rotation);

	void* hTimer = lv_obj_get_user_data(image);

	//We will use the image obj to store the timer and vica versa
	if(hTimer != NULL && imageControl->FlashEnabled)
	{
		lv_timer_set_period((lv_timer_t*) hTimer , imageControl->FlashInterval);
		lv_timer_resume(hTimer);
	}
	else if(hTimer != NULL && !imageControl->FlashEnabled)
	{
		lv_timer_pause(hTimer);
		if(lv_obj_has_flag(image, LV_OBJ_FLAG_HIDDEN))
		{
			lv_obj_clear_flag(image, LV_OBJ_FLAG_HIDDEN);
		}
	}

	return DShared_ErrorState_None;
}

void DView_ObjectFlashCB(lv_timer_t* timer)
{
	lv_obj_t* image = (lv_obj_t*) timer->user_data;

	if(lv_obj_has_flag(image, LV_OBJ_FLAG_HIDDEN))
	{
		lv_obj_clear_flag(image, LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
		lv_obj_add_flag(image, LV_OBJ_FLAG_HIDDEN);
	}
}


/**
 * @brief  	Simulator friendly timing function
 * @param	Timestamp in ms
 * @retval 	Difference between time_now and timestamp
 */
uint32_t DView_Elapsed(uint32_t timeStamp)
{
	return lv_tick_get() - timeStamp;
}

/**
 * @brief  	Simulator friendly timing function
 * @retval 	Current system runtime in ms
 */
uint32_t DView_Timestamp()
{
	return lv_tick_get();
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
