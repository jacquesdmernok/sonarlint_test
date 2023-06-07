/*
 * DView_Themes.h
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 * Admin: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_STYLES_DView_THEMES_H_
#define DISPLAY_VIEW_STYLES_DView_THEMES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../lvgl/lvgl.h"

/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern lv_theme_t DView_Theme_Dark;

/* Exported functions ------------------------------------------------------- */
void DView_Theme_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_STYLES_DView_THEMES_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
