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





/* Decode the PWM signal to binary */
void frame_decoder(uint32_t radio_frame[], uint8_t startFrame)
{
 
  static uint32_t decoded_frame[40];
  uint16_t temp=0; 
  
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
  }
  
  //checking temperature
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
  static int hej;
  hej = temp;
  //printf("CRC: %d \n " ,HAL_CRC_Calculate(&hcrc,decoded_frame, 40));
  
  // CRC check (discards the frame if the ckecksum returns a non zero value)
  if(HAL_CRC_Calculate(&hcrc,decoded_frame, 40) == 0)
  {
    displayFunction(temp, true);
    UARTputty(temp);
  }    

}

//Display temperature and real time clock on 7-seg displays
void displayFunction(int temp, bool new_temp)
{
  uint16_t hours;
  uint16_t minutes;
  uint16_t seconds;
  static uint8_t number[8];

  
  //Fetch value for RTC
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
    
  int i = 0;
  while(i < 8)
  {

    for(int i = 0; i < 9000; i++)
    {
    }
    
    //Make the Kolon to blink with 1 second intervall
    uint32_t current_second = HAL_GetTick();
    static uint32_t last_second;
    if((current_second - last_second)> 500)
    {
      last_second = current_second;
      HAL_GPIO_TogglePin(GPIOC, Kolon_Pin);
    }

    switch(i) // selects which CLK 7-seg to be enabled and disables the rest
    {
      //----------------Select clock display-----------------------------------
      case 0: // enable DIG1clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
       
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        break;
        
      case 1: // enable DIG2clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        break;
        
      case 2: // enable DIG3clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);

        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        break;
        
      case 3: // enable DIG4clk
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        break;
        
        //--------------select temperature display--------------------------------
      case 4: // enable DIG1term
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        break;
        
      case 5: // enable DIG2term and prints DP_led
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        break;
        
      case 6: // enable DIG3term
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_RESET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        break;
        
      case 7: // enable DIG4term
        HAL_GPIO_WritePin(GPIOC, DIG1term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3term_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4term_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, DP_led_Pin, GPIO_PIN_SET);
        
        HAL_GPIO_WritePin(GPIOC, DIG1clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG2clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG3clk_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, DIG4clk_Pin, GPIO_PIN_RESET);
        break;
    default:
      
      
      break;
    }
        
        switch(number[i]) // select which character to be displayed
        {
          case 0:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
            break;
            
          case 1:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
            break;
            
          case 2:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            break;
            
          case 3:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            break;
            
          case 4:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            break;
            
          case 5:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
            break;
            
          case 6:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
   
            break;
            
          case 7:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
       
            break;
            
          case 8:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
  
            break;
            
          case 9:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
     
            break;
            
        case 10:
           HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_RESET);
   
            break;
            
        default:
            HAL_GPIO_WritePin(GPIOD, A_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, B_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, C_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, D_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, E_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, F_led_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, G_led_Pin, GPIO_PIN_SET);
          break;
        }
      i++;
  }
      
 
}