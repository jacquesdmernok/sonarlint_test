/*
 * DView_Styles.c
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 * Admin: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "../View/Styles/DView_Styles.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
lv_style_t DView_Style_Container;

/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Initialises the base styles used by the theme
  * @retval None
  */
void DView_Style_Init(void)
{
	/*--- Base container style ---*/
	lv_style_init(&DView_Style_Container);
	lv_style_set_radius(&DView_Style_Container, 3);
	lv_style_set_border_width(&DView_Style_Container, 0);
	lv_style_set_bg_color(&DView_Style_Container, DVIEW_COLOR_BG);
	lv_style_set_pad_gap(&DView_Style_Container, 3);
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
