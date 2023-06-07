/*
 * DView_Styles.h
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 * Admin: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_DView_STYLES_H_
#define DISPLAY_VIEW_DView_STYLES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../lvgl/lvgl.h"

/* Defines -------------------------------------------------------------------*/
#define DVIEW_COLOR_BLUE			lv_color_make(12, 22, 64)
#define DVIEW_COLOR_BLACK			lv_color_black()
#define DVIEW_COLOR_WHITE			lv_color_white()
#define DVIEW_COLOR_BG 				lv_color_make(10, 10, 12)
#define DVIEW_COLOR_GRAY 			lv_color_make(134, 136, 143)
#define DVIEW_COLOR_GRAY_GR 		lv_color_make(114, 115, 120)
#define DVIEW_COLOR_GREEN 			lv_color_make(33, 156, 47)
#define DVIEW_COLOR_GREEN_GR 		lv_color_make(30, 110, 39)
#define DVIEW_COLOR_AMBER 			lv_color_make(245, 176, 66)
#define DVIEW_COLOR_AMBER_GR 		lv_color_make(156, 111, 40)
#define DVIEW_COLOR_RED 			lv_color_make(189, 13, 19)
#define DVIEW_COLOR_RED_GR 			lv_color_make(120, 13, 17)
#define DVIEW_COLOR_YELLOW 			lv_color_make(192, 204, 57)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern lv_style_t DView_Style_Container; 		//The basic container style, defining background, borders and alignment.

/* Exported functions ------------------------------------------------------- */
void DView_Style_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_DView_STYLES_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
