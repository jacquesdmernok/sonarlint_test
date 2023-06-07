/*
 * DView_Operation_RightEdge.h
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_RIGHT_H_
#define DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_RIGHT_H_

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
void DView_Operation_RightEdge_Init(lv_obj_t* parent, DView_Sector_t* hView_Operation_RightEdge);
DShared_ErrorState_t DView_Operation_RightEdge_Update(DView_Sector_t* hView_Operation_RightEdge, DShared_Operation_RightEdge_t* hShared_Operation_RightEdge);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_RIGHT_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
