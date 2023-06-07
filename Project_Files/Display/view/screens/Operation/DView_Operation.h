/*
 * DView_Operation.h
 *
 * Created on: 	27 Apr 2023
 * Author: 		Jacques
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_H_
#define DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DShared.h"
#include "DView.h"

/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum DView_Operation_Sectors
{
	DView_Operation_Sector_Header 		= 0,
	DView_Operation_Sector_Footer 		= 1,
	DView_Operation_Sector_LeftEdge 	= 2,
	DView_Operation_Sector_LeftIcons 	= 3,
	DView_Operation_Sector_Center 		= 4,
	DView_Operation_Sector_RightIcons 	= 5,
	DView_Operation_Sector_RightEdge 	= 6,

	DView_Operation_SectorCount
} DView_Operation_Sectors_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DView_Operation_Init(DView_ScreenData_t* hView_Operation);
DView_UpdateStatus_t DView_Operation_SM(DView_ScreenData_t* hView_Operation, DShared_OperationScreen_t* hShared_Operation);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_VIEW_OPERATION_DView_VIEW_OPERATION_H_ */

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
