/*
 * DView_Images.h
 *
 * Created on: 	22 May 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_IMAGES_DVIEW_IMAGES_H_
#define DISPLAY_VIEW_IMAGES_DVIEW_IMAGES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lvgl.h"
#include "DShared.h"
#include "DView.h"
/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern lv_img_dsc_t* DView_Images[DShared_Images_End];

/* Exported functions ------------------------------------------------------- */
void DView_Images_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_IMAGES_DVIEW_IMAGES_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
