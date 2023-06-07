/*
 * DView_Operation_Center.h
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_OPERATION_DVIEW_OPERATION_CENTER_H_
#define DISPLAY_VIEW_OPERATION_DVIEW_OPERATION_CENTER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lvgl.h"
#include "DShared.h"

/* Defines -------------------------------------------------------------------*/
#define THREATZONESPAN 	300	//Degrees * 10
#define EDGEWIDTH       25

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DView_Operation_Center_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_Center);
DShared_ErrorState_t DView_Operation_Center_Update(DView_Sector_t* hView_Operation_Center, DShared_Operation_Center_t* hShared_Operation_Center);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_OPERATION_DVIEW_OPERATION_CENTER_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
