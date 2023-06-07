/*================================
Buttons.h
Created on: 	Dec 21, 2022
Company: 		Mernok Elektronik
Author: 		H. du Plessis
=================================*/

#ifndef BUTTONS_BUTTONS_H_
#define BUTTONS_BUTTONS_H_
#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------
 *Includes
 *------------------------------*/
#include "stdbool.h"
#include "FreeRTOS.h"
#include "task.h"
/*--------------------------------
 *Defines
 *------------------------------*/

/*--------------------------------
 *Enums
 *------------------------------*/

/*--------------------------------
 *Structs
 *------------------------------*/
typedef struct
{
	bool Button1;
	bool Button2;
	bool Button3;
	bool Button4;
}Button_HandlerTydedef;

extern Button_HandlerTydedef HButtons;
/*--------------------------------
 *Extern Variables
 *------------------------------*/

/*--------------------------------
 *Global Functions
 *------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*BUTTONS_BUTTONS_H_*/
