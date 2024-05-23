/*
 * move.c
 *
 *  Created on: Mar 15, 2024
 *      Author: 高恒斌
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "move.h"
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

int GrayData[5]={0};//这5个变量用于算法中使用的灰度传感器的状态 0-L2 1-L1 2-M 3-R1 4-R2
int BackData[5]={0};//这5个变量用于算法中使用的后置灰度传感器的状态，以小车前进为正方向 0-L2 1-L1 2-M 3-R1 4-R2
int MoveSituation = 0;//0代表直行，1代表转弯或后退，2代表左转，3代表右转，4代表后退

unsigned char Control_message[50][7]={
{0x55, 0x55, 0x05, 0x06, 0x00, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x01, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x02, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x03, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x04, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x05, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x06, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x07, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x08, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x09, 0x01, 0x00},
{0x55, 0x55, 0x05, 0x06, 0x0A, 0x01, 0x00}
};

//TIM3左轮 TIM4右轮
void Move_Left(int velocity)
{
		if(velocity >= 0){
			HAL_GPIO_WritePin(Engine_Left1_GPIO_Port, Engine_Left1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Engine_Left2_GPIO_Port, Engine_Left2_Pin, GPIO_PIN_RESET);
		    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, velocity);
		}
		else if(velocity < 0){
			HAL_GPIO_WritePin(Engine_Left1_GPIO_Port, Engine_Left1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Engine_Left2_GPIO_Port, Engine_Left2_Pin, GPIO_PIN_SET);
		    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, -1*velocity);
		}
}

void Move_Right(int velocity)
{
		if(velocity >= 0){
			HAL_GPIO_WritePin(Engine_Right1_GPIO_Port, Engine_Right1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Engine_Right2_GPIO_Port, Engine_Right2_Pin, GPIO_PIN_RESET);
		    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, velocity);
		}
		else if(velocity < 0){
			HAL_GPIO_WritePin(Engine_Right1_GPIO_Port, Engine_Right1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Engine_Right2_GPIO_Port, Engine_Right2_Pin, GPIO_PIN_SET);
		    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, -1*velocity);
		}
}
//分别控制左右轮 velocity 范围 -100 - -50  50 - 100

void Move_Twin_Wheels(int velocity_1 ,int velocity_2)
{
	Move_Left(velocity_1);
	Move_Right(velocity_2);
}
//velocity_1 是左轮， velocity_2 是右轮
//注意接线！！！
//65， 63几乎直行（偏右）


void Engine_Init()
{
	HAL_TIM_Base_Start_IT(&htim6);                       //开启10ms定时器中断
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
	Move_Twin_Wheels(0, 0);
}


void GraySensor_Init()
{
	GrayData[0] = 0;
	GrayData[1] = 0;
	GrayData[2] = 0;
	GrayData[3] = 0;
	GrayData[4] = 0;
}

void Robotic_Arm_Move(int action_group)
{
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)Control_message[action_group], sizeof(Control_message[action_group]));
}
