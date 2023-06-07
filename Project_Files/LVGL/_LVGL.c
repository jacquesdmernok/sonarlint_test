/*
 * LVGL.c
 *
 *  Created on: Dec 2, 2022
 *      Author: hdupl
 */
#include "_LVGL.h"
#include "stdint.h"
#include "lvgl.h"
#include "ltdc.h"
#include "LCD.h"
#include "LCDControl.h"
#include "dma2d.h"

lv_disp_drv_t LCD_Driver;

void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	//#if LV_USE_GPU_STM32_DMA2D
	__HAL_DMA2D_ENABLE_IT(&hdma2d, DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE);
	//#else

	/*--- Source address ---*/
	uint16_t *sourceAddress = (uint16_t*) (color_p);

	/*--- Destination address ---*/
	uint16_t *destBuffer = (uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2 * (((area->y1) * 1024) + area->x1)));

	/*--- Fill buffer (copy row by row) ---*/
	uint16_t rowSize = (area->x2 - area->x1) + 1;			//size in pix
	uint16_t rowCount = (area->y2 - area->y1) + 1;			//Will be rows - 1

	hdma2d.Init.OutputOffset = LCDControl_GetXSize() - rowSize;

	//	while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS))
	//	{
	//		//Wait for the LTDC
	//	}

	/*--- Init the DMA ---*/
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
		{
			__NOP();
		}
	}

	/*--- Start the transfer ---*/
	if (HAL_DMA2D_Start(&hdma2d, (uint32_t) sourceAddress, (uint32_t) destBuffer, rowSize, rowCount) == HAL_OK)
	{
		/* Polling For DMA transfer */
		HAL_StatusTypeDef result = HAL_DMA2D_PollForTransfer(&hdma2d, 100);
		UNUSED(result);
	}

	lv_disp_flush_ready(&LCD_Driver);

	//#endif

}



void my_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t * area)
{
	/* Update the areas as needed.
	 * For example it makes the area to start only on 8th rows and have Nx8 pixel height.*/
	area->y1 = area->y1 & 0x07;
	area->y2 = (area->y2 & 0x07) + 8;
}

void my_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
	/* Write to the buffer as required for the display.
	 * For example it writes only 1-bit for monochrome displays mapped vertically.*/
	buf += buf_w * (y >> 3) + x;
	if(lv_color_brightness(color) > 128) (*buf) |= (1 << (y % 8));
	else (*buf) &= ~(1 << (y % 8));
}

void my_monitor_cb(lv_disp_drv_t * disp_drv, uint32_t time, uint32_t px)
{
	//printf("%d px refreshed in %d ms\n", time, ms);
}
lv_color_t buf[50 * 1024];

void LVGL_Init(void)
{
	lv_init();

	//#if LV_USE_GPU_STM32_DMA2D
	//
	//	/*A static or global variable to store the buffers*/
	//
	//	//static lv_color_t buf_1[1024 * 60];
	//	static lv_disp_draw_buf_t disp_buf;
	//	/*Initialize `disp_buf` with the buffer(s)
	//	 *
	//	 * . With only one buffer use NULL instead buf_2 */
	//	uint16_t* buffer1_adr = (uint16_t*) hltdc.LayerCfg[0].FBStartAdress;
	//	//uint16_t* buffer2_adr = (uint16_t*) hltdc.LayerCfg[1].FBStartAdress;
	//
	//	lv_disp_draw_buf_init(&disp_buf, buffer1_adr, NULL, 1024*600);
	//
	//	//static lv_disp_drv_t disp_drv;          /*A variable to hold the drivers. Must be static or global.*/
	//	lv_disp_drv_init(&LCD_Driver);            /*Basic initialization*/
	//	LCD_Driver.draw_buf = &disp_buf;          /*Set an initialized buffer*/
	//	LCD_Driver.flush_cb = my_flush_cb;        /*Set a flush callback to draw to the display*/
	//	LCD_Driver.hor_res = 1024;                 /*Set the horizontal resolution in pixels*/
	//	LCD_Driver.ver_res = 600;                 /*Set the vertical resolution in pixels*/
	//	LCD_Driver.full_refresh = 0;
	//	LCD_Driver.direct_mode = 1;
	//
	//	lv_disp_t * disp;
	//	disp = lv_disp_drv_register(&LCD_Driver); /*Register the driver and save the created display objects*/
	//	//disp->bg_opa = LV_OPA_TRANSP;

	//#else

	/*--- Create a display buffer ---*/
	static lv_disp_draw_buf_t My_Display_buf;
	uint16_t* buffer2_adr = (uint16_t*) LCD_FRAMEBUFFER2_ADDR;
	lv_disp_draw_buf_init(&My_Display_buf, buffer2_adr, NULL, 1024*600);

	/*--- Register display driver ---*/

	lv_disp_drv_init(&LCD_Driver);

	/*--- Set the resolution of the display ---*/
	LCD_Driver.hor_res = 1024;
	LCD_Driver.ver_res = 600;

	/*--- Used to copy the buffer's content to the display ---*/
	LCD_Driver.flush_cb = my_flush_cb;

	/*--- Set a display buffer ---*/
	LCD_Driver.draw_buf = &My_Display_buf;
	LCD_Driver.full_refresh = 0;

	/*--- Init the 2D DMA ---*/
	hdma2d.Init.Mode         = DMA2D_M2M;
	hdma2d.Init.ColorMode    = DMA2D_RGB565;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0xFF;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
	hdma2d.LayerCfg[1].InputOffset = 0;
	hdma2d.Instance = DMA2D;

	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
		{
			__NOP();
		}
	}

	/*--- Finally register the driver ---*/
	static lv_disp_t* myDisp;
	myDisp = lv_disp_drv_register(&LCD_Driver);
	UNUSED(myDisp);

}

void LVGL_DMA2D_TC()
{
	lv_disp_flush_ready(&LCD_Driver);
}
