/*
 * DView_Operation_RightIcons.h
 *
 * Created on: 	15 May 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_SCREENS_OPERATION_DVIEW_OPERATION_RIGHTICONS_H_
#define DISPLAY_VIEW_SCREENS_OPERATION_DVIEW_OPERATION_RIGHTICONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lvgl.h"
#include "DView.h"

/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DView_Operation_RightIcons_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_RightIcons);
DShared_ErrorState_t DView_Operation_RightIcons_Update(DView_Sector_t* hView_Operation_RightIcons, DShared_Operation_RightIcons_t* hShared_Operation_RightIcons);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_SCREENS_OPERATION_DVIEW_OPERATION_RIGHTICONS_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
