/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "cmsis_os.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f3_discovery.h"
#include "crc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart3;
ITStatus UartReady;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN USART3_MspInit 1 */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE END USART3_MspInit 1 */
  }
   
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

  }
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
} 

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
   //Set transmission flag: transfer complete 
  UartReady = SET;
}

/** @brief Rx Transfer completed callback
* @param UartHandle: UART handle.
* @note This example shows a simple way to report end of IT Rx transfer,
*       and you can add your own implementation.
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  // Set transmission flag: transfer complete 
  UartReady = SET;
}

void UARTputty(uint16_t temp)
{
   uint8_t newLine[] = "\r\n";
   uint8_t tiotal = (temp / 100) +48;
   uint8_t ental = ((temp % 100) / 10)+48;
   uint8_t decimaltal   = ((temp % 100) % 10)+48; 
  
    uint8_t tempbuffer[] = {tiotal, ental, ',', decimaltal};
    if(HAL_UART_Transmit(&huart3, (uint8_t *)newLine, 3, 5000) != HAL_OK)
    {
      Error_Handler();
    } 
  
    if(HAL_UART_Transmit(&huart3, (uint8_t *)tempbuffer, 6, 5000) != HAL_OK)
    {
      Error_Handler();
    } 
  memset(tempbuffer, 0, sizeof(tempbuffer)); 
  
}

// Takes time and date as input through UART communication and stores the time
void UARTfunction()
{
  
  UartReady = RESET;
   static uint8_t Buffer[] = "Welcome to KULUR, please set date (YYYY-MM-DD) and time (HH:MM)\r\n";
  uint8_t DateString[] = "Date: ";

  
  
  
    if(HAL_UART_Transmit(&huart3, (uint8_t *)Buffer, WELCOMEBUFFER, 5000) != HAL_OK)
    {
      Error_Handler();
    } 
  
    
    memset(Buffer, 0, sizeof(Buffer)); 
    
    if(HAL_UART_Transmit(&huart3, (uint8_t *)DateString, BUFFERSIZE,5000) != HAL_OK)
    {
      Error_Handler();
    }
  
  

  static bool dateCheck = false;
  static bool timeCheck = false;
  uint8_t TimeString[] = "\r\nTime: ";

  static uint16_t Date[3] = {0};
  static uint16_t time[2] = {0};
  
  if(!dateCheck)
  {
   

    
    if(HAL_UART_Receive_IT(&huart3, (uint8_t *)Buffer, DATEBUFFER) != HAL_OK)  
      {
        Error_Handler();
      }
      
      while(UartReady != SET)
      {
       
      }

      
      UartReady = RESET;
    
              if(HAL_UART_Transmit_IT(&huart3, (uint8_t *)Buffer, DATEBUFFER) != HAL_OK)
      {
        Error_Handler();
      } 
      while(UartReady != SET)
      {
        
      }
      UartReady = RESET;
      

      
      const char s[2] = "-";
     char *token;
     
     /* get the first token */
     token = strtok(Buffer, s);
     int i = 0;
     /* walk through other tokens */
     while( token != NULL ) 
     {
        Date[i] = atoi(token);
        //printf("%d\n", Date[i]);
        token = strtok(NULL, s);
        
        i=i+1;
     }
     
     Date[0] = Date[0] % 100;
           
     dateCheck = true;
    
  }
      
    if(!timeCheck)
  {
    
    memset(Buffer, 0, sizeof(Buffer)); 
    
    if(HAL_UART_Transmit_IT(&huart3, (uint8_t *)TimeString, BUFFERSIZE+2) != HAL_OK)
    {
      Error_Handler();
    } 
    while(UartReady != SET)
    {
      
    }
    UartReady = RESET;
    
    
    if(HAL_UART_Receive_IT(&huart3, (uint8_t *)Buffer, TIMEBUFFER) != HAL_OK)  
      {
        Error_Handler();
      }
      
      while(UartReady != SET)
      {
       
      }

      
      UartReady = RESET;
      
         if(HAL_UART_Transmit_IT(&huart3, (uint8_t *)Buffer, TIMEBUFFER) != HAL_OK)
      {
        Error_Handler();
      } 
      while(UartReady != SET)
      {
        
      }
      UartReady = RESET;
      
      const char s[2] = ":";
     char *token;
     
      //get the first token 
     token = strtok(Buffer, s);
     int i = 0;
     // walk through other tokens 
     while( token != NULL ) 
     {
        time[i] = atoi(token);
        token = strtok(NULL, s);
        
        i=i+1;
     }
  
    timeCheck = true;
  }
 
  RTC_TimeConfig(Date, time);
   
}






/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
