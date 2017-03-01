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
#include "kulur_functions.h"






void frame_decoder(uint32_t radio_frame[], uint8_t startFrame)
{
 
  static uint32_t decoded_frame[40];
  uint16_t temp=0; 
  
  
  
  //printf("It's inside!!!!! ?? 8===(-)\n");
  for(int i=0;i<40;i++)
  {
    if(radio_frame[i+startFrame]>=400 && radio_frame[i+startFrame] <=1300)
    {
      decoded_frame[i]=1;
    }
    else if(radio_frame[i+startFrame]>=1300 && radio_frame[i+startFrame] <=1700)
    {
      decoded_frame[i]=0;
    }
  // printf("Index%d = %d \n ",i,decoded_frame[i]);
  }
  
  
    /* if(decoded_frame[14])
  {
    temp+=512;
  }
  if(decoded_frame[15])
  {
    temp+=256;
  }*/
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
  static int hej;
  hej = temp;
  printf("CRC: %d \n " ,HAL_CRC_Calculate(&hcrc,decoded_frame, 40));
  //if(HAL_CRC_Calculate(&hcrc,decoded_frame, 40) == 0)
  //{
    displayFunction(temp, true);
    UARTputty(temp);
  //}
    
  
  // CRC FUCK
  /*decoded_frame[39]=1;
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
  decoded_frame[0]=0;*/
  //printf("CRC: %d \n" ,HAL_CRC_Calculate(&hcrc,decoded_frame, FRAME_SIZE));

  
  
  
 
  
  //return; 
}

void displayFunction(int temp, bool new_temp)
{
  uint16_t hours;
  uint16_t minutes;
  uint16_t seconds;
  static uint8_t number[8];
  //uint32_t tempsek = seconds;

  
  
  RTC_TimeShow(&hours, &minutes, &seconds);
 
  
  number[4] = 11;
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
    
    //float temp2sek = seconds;
    //temp2sek = temp2sek-0.5;
    


   
    
  
  int i = 0;
  while(i < 8)
  {

    for(int i = 0; i < 9000; i++)
    {
    }
    
    uint32_t current_second = HAL_GetTick();
    static uint32_t last_second;
    if((current_second - last_second)> 500)
    {
      last_second = current_second;
      HAL_GPIO_TogglePin(GPIOC, Kolon_Pin);
    }

    //HAL_Delay(1);
    switch(i) // selectar vilken 7-seg som ska lysa
    {
      //----------------Select clock display-----------------------------------
      case 0: // enablar clk DIG1clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
       
        
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
        //HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
        
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
        //HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_SET);
        
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
        //HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
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
        //HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
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
        //HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
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
        //HAL_GPIO_WritePin(GPIOC, Kolon_Pin, GPIO_PIN_RESET);
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
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
            //printf("\n");
          break;
        }
      i++;
  }
      
 
}