/*
 * DView_Operation_Header.h
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_HEADER_H_
#define DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DView.h"
#include "lvgl.h"

/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DView_Operation_Header_Init(lv_obj_t* parent, 	DView_Sector_t* hView_Operation_Header);
DShared_ErrorState_t DView_Operation_Header_Update(DView_Sector_t*	hView_Operation_Header, DShared_Operation_Header_t* hShared_Operation_Header);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_HEADER_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
