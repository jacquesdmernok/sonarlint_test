/*
 * SDRAM.c
 *
 *  Created on: Nov 27, 2022
 *      Author: H. du Plessis
 */

#include "SDRAM.h"

#include "stm32h747i_discovery_sdram.h"
#include "is42s32800j_conf_template.h"

#define BUFFER_SIZE            ((uint32_t)0x0100)
#define WRITE_READ_ADDR        ((uint32_t)0x1000)

uint32_t sdram_aTxBuffer[BUFFER_SIZE];
uint32_t sdram_aRxBuffer[BUFFER_SIZE];


/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLength; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
  /* Clean Data Cache to update the content of the SDRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)pBuffer, uwBufferLength*4);
}


///**
//  * @brief  Compares two buffers.
//  * @param  pBuffer1, pBuffer2: buffers to be compared.
//  * @param  BufferLength: buffer's length
//  * @retval 1: pBuffer identical to pBuffer1
//  *         0: pBuffer differs from pBuffer1
//  */
//static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
//{
//  /* Invalidate Data Cache to get the updated content of the SRAM*/
//  SCB_CleanInvalidateDCache_by_Addr((uint32_t *)pBuffer2, BufferLength*4);
//
//  while (BufferLength--)
//  {
//    if (*pBuffer1 != *pBuffer2)
//    {
//      return 1;
//    }
//
//    pBuffer1++;
//    pBuffer2++;
//  }
//
//  return 0;
//}


void SDRAM_Init()
{
	  /* SDRAM device configuration */
	  if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
	  {
		  __NOP();
	  }

//	  /* Fill the buffer to write */
//	  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);
//
//	  /* Write data to the SDRAM memory */
//	  if(HAL_SDRAM_Write_32b(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR), (uint32_t*)sdram_aTxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
//	  {
//	  __NOP();
//	  }
//
//
//	  /* Read back data from the SDRAM memory */
//	  if(HAL_SDRAM_Read_32b(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR ), (uint32_t*)sdram_aRxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
//	  {
//__NOP();
//	  }

//	  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
//	  {
//	    __NOP();
//	  }


}


void SRAM_Read()
{
	  if(HAL_SDRAM_Read_32b(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR ), (uint32_t*)sdram_aRxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
	  {
__NOP();
	  }
}
