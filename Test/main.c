/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t aShowTime[50] = {0};
RTC_HandleTypeDef hrtc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void displayFunction(int, bool);
void RTC_TimeConfig(uint16_t[], uint16_t[]);
void RTC_TimeShow(uint16_t*, uint16_t*);
void UARTfunction(uint8_t[]);
void frame_decoder(uint32_t radio_frame[]);
void MX_CRC_Init(void);
ITStatus UartReady;

//void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
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
/* USER CODE END 0 */

#define BUFFERSIZE 6
#define WELCOMEBUFFER 66
#define DATEBUFFER 10
#define TIMEBUFFER 5
#define FRAME_SIZE 40
#define MINIMUM_DUTY_TICKS 500



int main(void)
{


  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  //MX_TIM1_Init();
  MX_CRC_Init();

  /* USER CODE BEGIN 2 */
  
  /* USER CODE END 2 */

/* UartReady = RESET;
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
*/

  /* Call init function for freertos objects (in freertos.c) */
  //MX_FREERTOS_Init();

  /* Start scheduler */
  //osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

 //UARTfunction(Buffer);
 
  // RTC_TimeConfig();
  /* Infinite loop */

  // Test preamble duty 7500 us to 8500 us
  /*  for(int i=0;i<5;i++)
    {
      HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
      HAL_Delay(7);
      HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
      HAL_Delay(50);
      if(i<4)
      {
        HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
        HAL_Delay(8);
        HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
        HAL_Delay(50);
      }
    }

*/
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
    
    //RTC_TimeShow();
    //HAL_Delay(1000);
  /* USER CODE BEGIN 3 */
 
     /* HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
      HAL_Delay(8);
      HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
      HAL_Delay(50);
      HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
      HAL_Delay(12);
      HAL_GPIO_TogglePin(GPIOE,LD8_Pin);
      HAL_Delay(50);
     */
      
    displayFunction(215, false);
      
   
        
       
        
        
   // }
    
    
    
  }
  /* USER CODE END 3 */

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
  static uint32_t duty_in_microseconds=0;
  static uint32_t radio_frame[FRAME_SIZE];
  static uint32_t preamble_bits[9];
  static uint8_t bit_counter=0;
  static uint8_t interrupt_counter=0;
  static bool preamble_flag=false;
  
  
  //Lock function
  HAL_TIM_IC_Stop_IT(&htim1, TIM_CHANNEL_2);
  
 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    duty_in_microseconds=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    
   
  
  //Checks for preamble bits and sets a flag when 9 "1" in a row
 
    if(duty_in_microseconds>MINIMUM_DUTY_TICKS && !preamble_flag)
    {
       if(duty_in_microseconds >= 6000 && duty_in_microseconds <9000)
       {
         preamble_bits[bit_counter]=duty_in_microseconds;
         bit_counter++;
       }
       else
         bit_counter=0;
       
       if(bit_counter==9)
       {
         preamble_flag=true;
       }
      
    } 
    
    
    if(preamble_flag)
    {
      
    
      if(bit_counter >9 && bit_counter <=49)
      {
        radio_frame[bit_counter-10]=duty_in_microseconds;
        bit_counter++;
        interrupt_counter++;
        printf("INNE FOR %d \n",interrupt_counter); 
        
      }
      else if(bit_counter > 49)
      {
        frame_decoder(radio_frame);
        bit_counter=0;
        preamble_flag=false;
      }
      else 
        bit_counter++;
      
    }
    
      
    
    
  }
  
  
  //Unlock function
   HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);

  return;
}
  
void frame_decoder(uint32_t radio_frame[])
{
  static uint32_t decoded_frame[FRAME_SIZE];
  static uint16_t temp=0; 
  
  
  
  printf("It's inside!!!!! ?? 8===(-)\n");
  for(int i=0;i<FRAME_SIZE;i++)
  {
    if(radio_frame[i]>=7500 && radio_frame[i] <=8500)
    {
      decoded_frame[i]=1;
    }
    else if(radio_frame[i]>=11500 && radio_frame[i] <=12500)
    {
      decoded_frame[i]=0;
    }
    printf("Index%d = %d \n ",i,decoded_frame[i]);
  }
  
  // CRC FUCK
  decoded_frame[39]=1;
  decoded_frame[38]=0;
  decoded_frame[37]=1;
  decoded_frame[36]=1;
  decoded_frame[35]=1;
  decoded_frame[34]=0;
  decoded_frame[33]=1;
  decoded_frame[32]=0;
  
  decoded_frame[31]=0;
  decoded_frame[30]=1;
  decoded_frame[29]=0;
  decoded_frame[28]=0;
  decoded_frame[27]=0;
  decoded_frame[26]=1;
  decoded_frame[25]=0;
  decoded_frame[24]=0;
  
  decoded_frame[23]=1;
  decoded_frame[22]=1;
  decoded_frame[21]=1;
  decoded_frame[20]=0;
  decoded_frame[19]=1;
  decoded_frame[18]=0;
  decoded_frame[17]=1;
  decoded_frame[16]=1;
  
  decoded_frame[15]=0;
  decoded_frame[14]=1;
  decoded_frame[13]=0;
  decoded_frame[12]=0;
  decoded_frame[11]=1;
  decoded_frame[10]=1;
  decoded_frame[9]=1;
  decoded_frame[8]=0;
  
  decoded_frame[7]=0;
  decoded_frame[6]=0;
  decoded_frame[5]=0;
  decoded_frame[4]=1;
  decoded_frame[3]=0;
  decoded_frame[2]=0;
  decoded_frame[1]=1;
  decoded_frame[0]=0;
  printf("CRC: %d \n" ,HAL_CRC_Calculate(&hcrc,decoded_frame, FRAME_SIZE));

  
  
  
  if(decoded_frame[14])
  {
    temp+=512;
  }
  if(decoded_frame[15])
  {
    temp+=256;
  }
  if(decoded_frame[16])
  {
    temp+=128;
  }
  if(decoded_frame[17])
  {
    temp+=64;
  }
  if(decoded_frame[18])
  {
    temp+=32;
  }
  if(decoded_frame[19])
  {
    temp+=16;
  }
  if(decoded_frame[20])
  {
    temp+=8;
  }
  if(decoded_frame[21])
  {
    temp+=4;
  }
  if(decoded_frame[22])
  {
    temp+=2;
  }
  if(decoded_frame[23])
  {
    temp+=1;
  }
  
  
  printf("Temp: %d \n " ,temp);
  displayFunction(temp, true);
  
  return; 
}

void UARTfunction(uint8_t Buffer[])
{
  
  

  static bool dateCheck = false;
  static bool timeCheck = false;
  uint8_t TimeString[] = "\r\nTime: ";
  //static uint8_t testBuff[DATEBUFFER];

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
    //RTC_TimeConfig(Date);
   // RTC_TimeShow();
           
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
        printf("%d\n", time[i]);
        token = strtok(NULL, s);
        
        i=i+1;
     }
     
     
    time[1] = time[1]/100;
    
    timeCheck = true;
  }
  RTC_TimeConfig(Date, time);
   
}

void displayFunction(int temp, bool new_temp)
{
  uint16_t hours = 14 ;
  uint16_t minutes = 10;
  static uint8_t number[8];
  
 // RTC_TimeShow(&hours, &minutes);
 
  if(new_temp)
  {
    if(temp > 512)
    {
      number[4] = 10; 
      temp = temp - 512;
    }
    else
      number[4] = 11;

    number[5] = temp / 100;
    number[6] = (temp % 100) / 10;
    number[7] = (temp % 100) % 10;   
  }
    number[0] = hours / 10;
    number[1] = hours % 10;
    number[2] = minutes / 10;
    number[3] = minutes % 10;
  
  
  
  
  int i = 0;
  while(i < 8)
  {
    HAL_Delay(1);
    switch(i) // selectar vilken 7-seg som ska lysa
    {
      //----------------Select clock display-----------------------------------
      case 0: // enablar clk DIG1clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        //printf("DIG1clk: ");
        break;
        
      case 1: // enablar clk DIG2clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        //printf("DIG2clk: ");
        break;
        
      case 2: // enablar clk DIG3clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        //printf("DIG3clk: ");
        break;
        
      case 3: // enablar clk DIG4clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        //printf("DIG4clk: ");
        break;
        
        //--------------select temperature display--------------------------------
      case 4: // enablar term DIG1term
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
        //printf("DIG1term: ");
        break;
        
      case 5: // enablar term DIG2term och skriver ut "punkten" för decimaltal
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
        //printf("DIG2term: ");
        break;
        
      case 6: // enablar term DIG3term
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_RESET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
        //printf("DIG3term: ");
        break;
        
      case 7: // enablar term DIG4term
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
        //printf("DIG4term: ");
        break;
    default:
      break;
    }
        
    
    //number = number % 10;
        switch(number[i]) // selectar vilket nummer som ska visas på displayen
        {
          case 0:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
            //HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 0\n");
            break;
            
          case 1:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
            //HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 1\n");
            break;
            
          case 2:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 2\n");
            break;
            
          case 3:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 3\n");
            break;
            
          case 4:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 4\n");
            break;
            
          case 5:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 5\n");
            break;
            
          case 6:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 6\n");
            break;
            
          case 7:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
            //printf("Number = 7\n");
            break;
            
          case 8:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 8\n");
            break;
            
          case 9:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = 9\n");
            break;
            
        case 10:
           HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("Number = -\n");
            break;
            
        default:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            //printf("\n");
          break;
        }
      i++;
  }
      
 
}

void RTC_TimeConfig(uint16_t Date[], uint16_t time[])
{
  printf("minutes1 = %d\n", time[1]); 
  RTC_TimeTypeDef stimestructure;
  RTC_DateTypeDef sdatestructure;
  
  sdatestructure.Year = Date[0];
  sdatestructure.Month = Date[1];
  sdatestructure.Date = Date[2];
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  stimestructure.Hours = time[0];
  printf("minutes2 = %d\n", time[1]); 
  stimestructure.Minutes = time[1];
  stimestructure.Seconds = 0x00;
  
 HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD);
  HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
}
  
void RTC_TimeShow(uint16_t *hours, uint16_t *minutes)
{
  RTC_TimeTypeDef stimestructureget;
  RTC_DateTypeDef sdatestructureget;
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BCD);
  
  *hours = stimestructureget.Hours;
  *minutes = stimestructureget.Minutes;
  //displayFunction(stimestructureget.Seconds);
  printf("Time = %d:%d:%d\n", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  //printf("Date = %d-%d-%d\n", sdatestructureget.Year, sdatestructureget.Month, sdatestructureget.Date);
         
}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}








#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
