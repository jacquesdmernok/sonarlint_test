/*
 * DView_Themes.c
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 * Admin: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "../Styles/DView_Styles.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
lv_theme_t DView_Theme_Dark;

/* Private function prototypes -----------------------------------------------*/
static void DView_Theme_Dark_Apply_CB(lv_theme_t * th, lv_obj_t * obj);

/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Initialises the dark theme
  * @retval None
  */
void DView_Theme_Init(void)
{
	DView_Theme_Dark = *(lv_disp_get_theme(NULL));

	/*--- Set parent and theme apply callback ---*/
	lv_theme_set_parent(&DView_Theme_Dark, lv_disp_get_theme(NULL));	//The parents styles will be applied to all objects first, then the added styles in the custom theme are applied after
	lv_theme_set_apply_cb(&DView_Theme_Dark, DView_Theme_Dark_Apply_CB);

	/*--- Assign the theme to the display ---*/
	lv_disp_set_theme(NULL, &DView_Theme_Dark);
}

/**
  * @brief  The apply callback for new objects belonging to the dark theme
  * @retval None
  */
static void DView_Theme_Dark_Apply_CB(lv_theme_t * th, lv_obj_t * obj)
{
	lv_obj_add_style(obj, &DView_Style_Container, 0);
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
