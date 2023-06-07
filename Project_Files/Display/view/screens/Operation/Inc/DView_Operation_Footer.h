/*
 * DView_Operation_Footer.h
 *
 * Created on: 	03 May 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_OPERATION_DVIEW_OPERATION_FOOTER_H_
#define DISPLAY_VIEW_OPERATION_DVIEW_OPERATION_FOOTER_H_

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
void DView_Operation_Footer_Init(lv_obj_t* parent, 	DView_Sector_t* hView_Operation_Footer);
DShared_ErrorState_t DView_Operation_Footer_Update(DView_Sector_t*	hView_Operation_Footer, DShared_Operation_Footer_t* hShared_Operation_Footer);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_OPERATION_DVIEW_OPERATION_FOOTER_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
