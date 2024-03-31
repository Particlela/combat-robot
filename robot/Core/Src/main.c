/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "move.h"
#include "game.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t TransmitData[200];//芯片发�?�给电脑的模块参数信�????
float MotorData = 0.0;//舵机的角度（左至右）
int EngineData[2] = {0,0};//电机转�?�（左至右）
int Data[200];//经过�????个交点后的运�????
//以上四个数组仅在传输信息时起效，1s更新�????次，不在实时测算中起�????

int cross_cnt = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim5.Instance) // 确认是正确的定时�????
    {
        MotorData = Motor_angel;

    	sprintf((char*)TransmitData, "灰度传感器：%d %d %d %d %d \n 已经经过%d个路口\n ", GrayData[0],GrayData[1],GrayData[2],GrayData[3],GrayData[4],cross_cnt);
    	HAL_UART_Transmit_DMA(&huart2, TransmitData, sizeof(TransmitData));//蓝牙
    	HAL_UART_Transmit_DMA(&huart3, TransmitData, sizeof(TransmitData));//传输信息
    }//此为TIM5触发的中�???? 1s�????�????

    if(htim->Instance == htim6.Instance)		              //10ms中断
    {
        EngineData[0] = Read_Rotational_Speed(0);         //0为左,转�?�单位为RPM
        EngineData[1] = Read_Rotational_Speed(1);         //1为右，转速单位为RPM
        if(HAL_GPIO_ReadPin(Gray_L2_GPIO_Port, Gray_L2_Pin) == GPIO_PIN_SET)
        {
        	GrayData[0] = 1;
        }
        else if(HAL_GPIO_ReadPin(Gray_L2_GPIO_Port, Gray_L2_Pin) == GPIO_PIN_RESET)
        {
        	GrayData[0] = 0;
        }
        if(HAL_GPIO_ReadPin(Gray_L1_GPIO_Port, Gray_L1_Pin) == GPIO_PIN_SET)
        {
        	GrayData[1] = 1;
        }
        else if(HAL_GPIO_ReadPin(Gray_L1_GPIO_Port, Gray_L1_Pin) == GPIO_PIN_RESET)
        {
        	GrayData[1] = 0;
        }
        if(HAL_GPIO_ReadPin(Gray_M_GPIO_Port, Gray_M_Pin) == GPIO_PIN_SET)
        {
        	GrayData[2] = 1;
        }
        else if(HAL_GPIO_ReadPin(Gray_M_GPIO_Port, Gray_M_Pin) == GPIO_PIN_RESET)
        {
        	GrayData[2] = 0;
        }
        if(HAL_GPIO_ReadPin(Gray_R1_GPIO_Port, Gray_R1_Pin) == GPIO_PIN_SET)
        {
        	GrayData[3] = 1;
        }
        else if(HAL_GPIO_ReadPin(Gray_R1_GPIO_Port, Gray_R1_Pin) == GPIO_PIN_RESET)
        {
        	GrayData[3] = 0;
        }
        if(HAL_GPIO_ReadPin(Gray_R2_GPIO_Port, Gray_R2_Pin) == GPIO_PIN_SET)
        {
        	GrayData[4] = 1;
        }
        else if(HAL_GPIO_ReadPin(Gray_R2_GPIO_Port, Gray_R2_Pin) == GPIO_PIN_RESET)
        {
        	GrayData[4] = 0;
        }//10ms刷新�????�????
    }//此为TIM6触发的中�????
	else if(htim->Instance == htim1.Instance)
	{
		if(COUNTERNUMLEFT < 10000)	EngineLeft.loopNum++;         //向上计数超过10000，正溢出+1
		else if(COUNTERNUMLEFT > 10000)	EngineLeft.loopNum--;     //向下计数小于0，负溢出+1
		__HAL_TIM_SetCounter(&htim1, 10000);             //重新设定初始�????
	}
	else if(htim->Instance == htim8.Instance)
	{
		if(COUNTERNUMRIGHT < 10000)	EngineRight.loopNum++;         //向上计数超过10000，正溢出+1
		else if(COUNTERNUMRIGHT > 10000)	EngineRight.loopNum--;     //向下计数小于0，负溢出+1
		__HAL_TIM_SetCounter(&htim8, 10000);             //重新设定初始�????
	}
}
//TIM5每秒溢出触发回调函数向计算机发�?�参�????
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM9_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  Engine_Init();
  Motor_Init();
  GraySensor_Init();
  HAL_TIM_Base_Start_IT(&htim5);//初始化TIM5，用于发送非阻塞信息

  HAL_Delay(3000);
  Motor_Position(2);

  int path[30] = {3, 3, 2, 3, 3, 1, 3, 3, 1, 3, 3, 0};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(path[cross_cnt] == 0){
		  Move_Twin_Wheels(-70, -70);
	      HAL_Delay(310);
		  Move_Twin_Wheels(0, 0);
		  while(1);
	  }else if(path[cross_cnt] == 1){
		  Move_Turn(1);
		  Position_Adj(1);
		  Motor_Position(2);

		  cross_cnt++;
		  for(int i = 0; i < 20; i++){
			  Move_Straight(1);
		  }
	  }else if(path[cross_cnt] == 2){
		  Move_Turn(2);
		  Position_Adj(2);
		  Motor_Position(2);

		  cross_cnt++;
		  for(int i = 0; i < 20; i++){
			  Move_Straight(1);
		  }
	  }else if(path[cross_cnt] == 3){
		  Move_Straight(1);
	  }

	  if(Judge_cross() == 1){
		  cross_cnt++;
          HAL_Delay(100);
	  }

      if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) // �????测到按键按下
      {
          HAL_Delay(50); // �????单延迟，以便于消�????
          if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) // 再次确认按键确实被按�????
          {
        	  HAL_PWR_EnterSTANDBYMode();
        	  break;
          }
      }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
