/*
 * DView_Images.c
 *
 * Created on: 	22 May 2023
 * Author: 		Jacques
 */

/* Includes ------------------------------------------------------------------*/
#include "DView_Images.h"
#include "DView.h"

/* External variables --------------------------------------------------------*/
extern lv_img_dsc_t Gear_First;
extern lv_img_dsc_t Gear_Neutral;
extern lv_img_dsc_t Gear_Parked;
extern lv_img_dsc_t Gear_Reverse;
extern lv_img_dsc_t Gear_Second;
extern lv_img_dsc_t Gear_Third;
extern lv_img_dsc_t Gear_Third_Dark;
extern lv_img_dsc_t GNSS;
extern lv_img_dsc_t GNSS_Dark;
extern lv_img_dsc_t Green_Smiley;
extern lv_img_dsc_t Green_Smiley_Shaded;
extern lv_img_dsc_t Red_Smiley;
extern lv_img_dsc_t Red_Smiley_Shaded;
extern lv_img_dsc_t Yellow_Smiley;
extern lv_img_dsc_t Yellow_Smiley_Shaded;
extern lv_img_dsc_t Person;
extern lv_img_dsc_t Person_Exclusion_Count;
extern lv_img_dsc_t Person_Exclusion_Count_Dark;
extern lv_img_dsc_t RF_VTP;
extern lv_img_dsc_t RF_VTP_Dark;
extern lv_img_dsc_t RF_VTV;
extern lv_img_dsc_t RF_VTV_Dark;
extern lv_img_dsc_t Server_Connected;
extern lv_img_dsc_t Server_Connected_Dark;
extern lv_img_dsc_t Speed_Sign_White_0;
extern lv_img_dsc_t Speed_Sign_White_20;
extern lv_img_dsc_t Speed_Sign_White_60;
extern lv_img_dsc_t Stop_Sign;
extern lv_img_dsc_t Stop_Sign_Shaded;
extern lv_img_dsc_t VI_Motion_Inhibit;
extern lv_img_dsc_t VI_Presence;
extern lv_img_dsc_t VI_Presence_Shaded;
extern lv_img_dsc_t VI_Prohibit;
extern lv_img_dsc_t VI_Slow;
extern lv_img_dsc_t VI_Slow_Shaded;
extern lv_img_dsc_t VI_Stop;
extern lv_img_dsc_t VI_Stop_Shaded;
extern lv_img_dsc_t VI_Warning;
extern lv_img_dsc_t VI_Warning_Shaded;
extern lv_img_dsc_t Error_CAN;
extern lv_img_dsc_t License_Missing;
extern lv_img_dsc_t License_Valid;
extern lv_img_dsc_t License_Processing;
extern lv_img_dsc_t Reload;
extern lv_img_dsc_t Switch_Display_Button;
extern lv_img_dsc_t Vehicle_1;
extern lv_img_dsc_t Vehicle_2;
extern lv_img_dsc_t Vehicle_3;
extern lv_img_dsc_t Exclusion;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
lv_img_dsc_t* DView_Images[DShared_Images_End];

/* Private function prototypes -----------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
 * @brief 	Initialises the constant strings for each language
 * @retval 	None
 */
void DView_Strings_Init(DView_t* hView)
{
    // For each string, define the English, Spanish, Swedish, and Afrikaans translations
    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_None] = "None";
    hView->ConstantStrings[DShared_Language_Spanish][DShared_ConstantStrings_None] = "Ninguno";
    hView->ConstantStrings[DShared_Language_Swedish][DShared_ConstantStrings_None] = "Inget";

    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_ForceSlow] = "Force Slow: ";
    hView->ConstantStrings[DShared_Language_Spanish][DShared_ConstantStrings_ForceSlow] = "Forzar Lento: ";
    hView->ConstantStrings[DShared_Language_Swedish][DShared_ConstantStrings_ForceSlow] = "Tvinga Långsamt: ";

    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_ForceStop] = "Force Stop: ";
    hView->ConstantStrings[DShared_Language_Spanish][DShared_ConstantStrings_ForceStop] = "Forzar Detener: ";
    hView->ConstantStrings[DShared_Language_Swedish][DShared_ConstantStrings_ForceStop] = "Tvinga Stop: ";

    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_Warning] = "Warning: ";
    hView->ConstantStrings[DShared_Language_Spanish][DShared_ConstantStrings_Warning] = "Advertencia: ";
    hView->ConstantStrings[DShared_Language_Swedish][DShared_ConstantStrings_Warning] = "Varning: ";

    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_Error] = "Error: ";
    hView->ConstantStrings[DShared_Language_Spanish][DShared_ConstantStrings_Error] = "Error: ";
    hView->ConstantStrings[DShared_Language_Swedish][DShared_ConstantStrings_Error] = "Fel: ";

    //English only
    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_Caution] = "Caution: ";
    hView->ConstantStrings[DShared_Language_English][DShared_ConstantStrings_AdviseSlow] = "Slow advised: ";
}

/**
 * @brief 	Initialises image pointers
 * @retval 	None
 */
void DView_Images_Init(void)
{
		
	DView_Images[DShared_Images_Vehicle_1] = &Vehicle_1;
	DView_Images[DShared_Images_Vehicle_2] = &Vehicle_2;
	DView_Images[DShared_Images_Vehicle_3] = &Vehicle_3;
    DView_Images[DShared_Images_Person] = &Person;

    /*--- Smileys ---*/
    DView_Images[DShared_Images_Face_Green] = &Green_Smiley;
    DView_Images[DShared_Images_Face_Warn] = &Yellow_Smiley;
    DView_Images[DShared_Images_Face_Error] = &Red_Smiley;

	DView_Images[DShared_Images_Speed_20] = &Speed_Sign_White_20;
	DView_Images[DShared_Images_Speed_60] = &Speed_Sign_White_60;
	DView_Images[DShared_Images_Speed_Stop] = &Stop_Sign;

	DView_Images[DShared_Images_VI_Presence] = &VI_Presence_Shaded;
	DView_Images[DShared_Images_VI_Warn] = &VI_Warning_Shaded;
	DView_Images[DShared_Images_VI_Slow] = &VI_Slow_Shaded;
	DView_Images[DShared_Images_VI_Stop] = &VI_Stop_Shaded;
	DView_Images[DShared_Images_VI_MotionInhibit] = &VI_Motion_Inhibit;

	/*--- Icons ---*/
	DView_Images[DShared_Images_Icon_PedCount] = &RF_VTP;
	DView_Images[DShared_Images_Icon_VehicleCount] = &RF_VTV;
	DView_Images[DShared_Images_Icon_SatCount] = &GNSS;
	DView_Images[DShared_Images_Icon_ExclCount] = &Person_Exclusion_Count;
	DView_Images[DShared_Images_Icon_Connection] = &Server_Connected;

	DView_Images[DShared_Images_CANError] = &Error_CAN;
	DView_Images[DShared_Images_Exclusion] = &Exclusion;

	DView_Images[DShared_Images_LicenseValid] = &License_Valid;
	DView_Images[DShared_Images_LicenseLoading] = &License_Processing;
	DView_Images[DShared_Images_LicenseMissing] = &License_Missing;

	//This just ensures new images are added here as well
	for(uint8_t i = DShared_Images_None + 1; i < DShared_Images_End; i++)
	{
		if(DView_Images[i] == NULL)
		{
			//Image definition not found
			while(1);;
		}
	}
}

/************************ (C) COPYRIGHT Mernok Elektronik ******END OF FILE****/
