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
#define IN 0
#define OUT 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t TransmitData[200];//芯片发给电脑的模块参数信�?????????
//以上四个数组仅在传输信息时起效，1s更新1次，不在实时测算中起�?????????

int cross_cnt = 0;
int path_selection = 0;
int Timer_cnt = 0, Cross_fact = 0;
int Back_first = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim5.Instance) // 确认是正确的定时�?????????
    {
    	sprintf((char*)TransmitData, "巡线灰度传感器：%d %d %d %d %d \n"
    			, GrayData[1],GrayData[2],GrayData[3],BackData[1],BackData[3]);
    	HAL_UART_Transmit_DMA(&huart3, TransmitData, sizeof(TransmitData));//传输信息
    }//此为TIM5触发的中�?????????

    if(htim->Instance == htim6.Instance)		              //10ms中断
    {
    	//前置灰度传感器刷新数�???
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
        }

        //后置灰度传感器刷新数�???
		if(HAL_GPIO_ReadPin(Back_L2_GPIO_Port, Back_L2_Pin) == GPIO_PIN_SET)
		{
			BackData[0] = 1;
		}
		else if(HAL_GPIO_ReadPin(Back_L2_GPIO_Port, Back_L2_Pin) == GPIO_PIN_RESET)
		{
			BackData[0] = 0;
		}
		if(HAL_GPIO_ReadPin(Back_L1_GPIO_Port, Back_L1_Pin) == GPIO_PIN_SET)
		{
			BackData[1] = 1;
		}
		else if(HAL_GPIO_ReadPin(Back_L1_GPIO_Port, Back_L1_Pin) == GPIO_PIN_RESET)
		{
			BackData[1] = 0;
		}
		if(HAL_GPIO_ReadPin(Back_M_GPIO_Port, Back_M_Pin) == GPIO_PIN_SET)
		{
			BackData[2] = 1;
		}
		else if(HAL_GPIO_ReadPin(Back_M_GPIO_Port, Back_M_Pin) == GPIO_PIN_RESET)
		{
			BackData[2] = 0;
		}
		if(HAL_GPIO_ReadPin(Back_R1_GPIO_Port, Back_R1_Pin) == GPIO_PIN_SET)
		{
			BackData[3] = 1;
		}
		else if(HAL_GPIO_ReadPin(Back_R1_GPIO_Port, Back_R1_Pin) == GPIO_PIN_RESET)
		{
			BackData[3] = 0;
		}
		if(HAL_GPIO_ReadPin(Back_R2_GPIO_Port, Back_R2_Pin) == GPIO_PIN_SET)
		{
			BackData[4] = 1;
		}
		else if(HAL_GPIO_ReadPin(Back_R2_GPIO_Port, Back_R2_Pin) == GPIO_PIN_RESET)
		{
			BackData[4] = 0;
		}
		Timer_cnt++; // 1ms自增一次的定时器计数变量
    }//此为TIM6触发的中�?????????
}
//TIM5每秒溢出触发回调函数向计算机发�?�参�???????


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
	{
		HAL_Delay(50);
		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
		{
			if(path_selection <= 8)
			path_selection++;
			else if(path_selection == 9)
			path_selection = 0;
		}
	}
}
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Engine_Init();
  GraySensor_Init();
  HAL_TIM_Base_Start_IT(&htim5);//初始化TIM5，用于发送非阻塞信息
  Robotic_Arm_Move(0);
  HAL_Delay(2000);

  int path[10][80] = {{3,3,6,8,3,9,3,6,1,3,3,3,6,10,3,6,1,3,4,5,2,3,3,9,3,6,1,1,3,6,1,3,6,2,3,3,3,6,13,3,6,2,3,3,6,0}
  ,{3,6,1,3,11,3,6,2,3,3,6,1,12,5,1,1,3,3,11,5,1,3,3,3,3,6,1,3,12,0}
  ,{3,6,1,3,3,11,3,3,6,2,3,3,3,3,6,1,3,12,0}
  ,{3,3,6,1,3,9,3,6,2,3,3,6,1,3,6,10,3,3,4,5,1,3,3,6,1,3,3,11,5,1,3,3,3,6,1,3,12,0}
  ,{3,3,6,1 ,3,6,2,3,6,2,14,5,1,3,3,6,2,14,5,1,3,3,6,2,14,0}};
  //第一条路为最�?????????单三分，第二条路为去对面拿三分，第三条路为己方右边三分，第四条路为勾出对面物块，第五条路为挡住对面去�?????????
  //路径运动参数�????0 终止�???? 1 左转�???? 2 右转�???? 3 直行�???? 4前置判断停车 5 倒车�???? 6 停至路口（0前进后）； 7 首路左转； 8 首路右转
  //14
  //机械臂动 9 抓取物块;10 右方放置前物块； 11 抓环 ；12 套环； 13 左方放置前物块 ;14 偷环
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(path[path_selection][cross_cnt] == 0){
//		  Move_Twin_Wheels(70, 70);
//		  HAL_Delay(80);
		  Move_Twin_Wheels(0, 0);
		  while(1);
	  }else if(path[path_selection][cross_cnt] == 1){
		  Move_Turn(1,0);
		  cross_cnt++;

//		  for(int i = 0; i < 20; i++){
//			  Move_Straight(1);
//		  }
	  }else if(path[path_selection][cross_cnt] == 2){
		  Move_Turn(2,0);
		  cross_cnt++;

//		  for(int i = 0; i < 20; i++){
//			  Move_Straight(1);
//		  }
	  }else if(path[path_selection][cross_cnt] == 3){
		  Move_Straight(1);
	  }
//	  else if(path[path_selection][cross_cnt] == 4){
//		  Move_Straight(3);
//	  }
	  else if(path[path_selection][cross_cnt] == 5)
	  {
//		  Move_Straight(2);
		  if(Back_first == 1)
		  {
			  Move_Twin_Wheels(-72,-71);
			  HAL_Delay(800);
			  Back_first = 0;
		  }
		  Move_Twin_Wheels(-72,-71);
	      if(BackData[0] == IN && BackData[1] == IN && BackData[2] == IN && BackData[3] == IN && BackData[4] == IN)
	      {
			  Move_Twin_Wheels(80, 80);
			  HAL_Delay(30);
			  Move_Twin_Wheels(0, 0);
			  HAL_Delay(70);
			  Back_first = 1;
			  cross_cnt++;
	      }
	  }
	  else if(path[path_selection][cross_cnt] == 6)
	  {
		  cross_cnt++;
		  Move_Stop(0);
	  }
	  else if(path[path_selection][cross_cnt] == 7)
	  {
		  cross_cnt++;
		    Move_Turn(1,1);
	  }
	  else if(path[path_selection][cross_cnt] == 8)
	  {
		  Move_Turn(2,1);
		  cross_cnt++;
	  }
	  else if(path[path_selection][cross_cnt] == 9)
	  {
			//刹车至轮胎中心
		  cross_cnt++;
		  Move_Twin_Wheels(60, 60);			//0, 0
		  HAL_Delay(200);	//100			//50
		  Move_Twin_Wheels(0, 0);			//0, 0
		  HAL_Delay(100);	//100			//50
		  Robotic_Arm_Move(1);
		  HAL_Delay(2500);
	  }
	  else if(path[path_selection][cross_cnt] == 10)
	  {
		  Robotic_Arm_Move(2);
		  HAL_Delay(2500);//
		  Robotic_Arm_Move(0);
		  HAL_Delay(1000);//
		  cross_cnt++;
	  }
	  else if(path[path_selection][cross_cnt] == 11)
	  {
		  cross_cnt++;
		  Move_Twin_Wheels(60, 60);			//0, 0
		  HAL_Delay(200);	//100			//50
		  Move_Twin_Wheels(0, 0);			//0, 0
		  HAL_Delay(100);	//100			//50
		  Robotic_Arm_Move(3);
		  HAL_Delay(3000);//
	  }
	  else if(path[path_selection][cross_cnt] == 12)
	  {
		  for(int i = 0; i < 800; i++){
			  Move_Straight(1);
			  HAL_Delay(1);
		  }
		  Move_Twin_Wheels(0, 0);
		  HAL_Delay(50);
		  Robotic_Arm_Move(4);
		  HAL_Delay(1300);//
		  Robotic_Arm_Move(0);
		  HAL_Delay(1000);//
		  cross_cnt++;
	  }
	  else if(path[path_selection][cross_cnt] == 13)
	  {
		  Robotic_Arm_Move(6);
		  HAL_Delay(5000);//
		  Robotic_Arm_Move(0);
		  HAL_Delay(500);//
		  cross_cnt++;
	  }
	  else if(path[path_selection][cross_cnt] == 14)
	  {
		  for(int i = 0; i < 800; i++)
		  {
			  Move_Straight(1);
			  HAL_Delay(1);
		  }
		  Move_Twin_Wheels(0, 0);
		  HAL_Delay(50);
		  Robotic_Arm_Move(5);
		  HAL_Delay(8000);//
		  Robotic_Arm_Move(0);
		  HAL_Delay(1000);//
		  cross_cnt++;
	  }
	  if(Timer_cnt >= 600)
	  {
		  Cross_fact = 1;
	  }

	  if(Judge_cross() == 1 && Cross_fact == 1){
		  cross_cnt++;
		  Timer_cnt = 0;
		  Cross_fact = 0;
	  }

	    if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) // �?????????????测到按键按下
	    {
	        HAL_Delay(50); // 50ms单延迟，以便于消�?????????
	        if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) // 再次确认按键确实被按�?????????
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
