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
int MoveSituation = 0;//0代表直行，1代表转弯或后退，2代表左转，3代表右转，4代表后退  MoveSituation改变后首先要HAL_TIM_PWM_Stop( , );
float Motor_angel = 0;//舵机角度
Engine EngineLeft;
Engine EngineRight;

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
	Move_Left(velocity_2);
	Move_Right(velocity_1);
}
//velocity_1 是左轮， velocity_2 是右轮
//这里存在错误！Left Right 反了！但是上面那行注释正确
//65， 63几乎直行（偏右）

float Read_Rotational_Speed(int n)
{
	float rotational_speed = 0;
	if(n == 0)
	{
		int16_t pluse = COUNTERNUMLEFT - RELOADVALUELEFT/2;
        //从开始到现在当前10ms的总脉冲数
		EngineLeft.totalAngle = pluse + EngineLeft.loopNum * RELOADVALUELEFT/2;
        //进行速度计算,4倍频,编码器11位,减速比56,再乘以6000即为每分钟输出轴多少转
        //motor.totalAngle - motor.lastAngle为当前10ms内的增量，即脉冲数
		EngineLeft.speed = (float)(EngineLeft.totalAngle - EngineLeft.lastAngle)/(4*11*RR)*6000;
		rotational_speed = EngineLeft.speed;
		EngineLeft.lastAngle = EngineLeft.totalAngle;              //更新转过的圈数
		return rotational_speed;
	}

    //如果是编码器更新中断,即10ms内,脉冲数超过了计数范围,需要进行处理
	else if(n == 1)
	{
		int16_t pluse = COUNTERNUMRIGHT - RELOADVALUERIGHT/2;
        //从开始到现在当前10ms的总脉冲数
		EngineRight.totalAngle = pluse + EngineRight.loopNum * RELOADVALUERIGHT/2;
        //进行速度计算,4倍频,编码器11位,减速比56,再乘以6000即为每分钟输出轴多少转
        //motor.totalAngle - motor.lastAngle为当前10ms内的增量，即脉冲数
		EngineRight.speed = (float)(EngineRight.totalAngle - EngineRight.lastAngle)/(4*11*RR)*6000;
		rotational_speed = EngineRight.speed;
		EngineRight.lastAngle = EngineRight.totalAngle;              //更新转过的圈数
		return rotational_speed;
	}
	else
		return 0;
}
//n=0读左轮转速，n=1读右轮转速


void Motor_Control(int duty_cycle)
{
    // 舵机控制占空比范围2.5% ~ 12.5%
    // Counter Period设置的是2000，因此占空比设置范围是50 ~ 250
	Motor_angel = duty_cycle * 0.9 - 45;
    __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 50 + duty_cycle);
    __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 250 - duty_cycle);
}
//要先初始化再装在车上
//50-250表示0-180度，duty_cycle的范围是0-200
//输入0为初始状态（完全张开），输入65为完全收缩，输入35为装载物块后的收紧值

void Engine_Init()
{

	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
	HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
    __HAL_TIM_ENABLE_IT(&htim8,TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim6);                       //开启10ms定时器中断
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
	__HAL_TIM_SET_COUNTER(&htim1, 10000);
	__HAL_TIM_SET_COUNTER(&htim8, 10000);
	EngineLeft.loopNum = 0;
	EngineRight.loopNum = 0;
	Move_Twin_Wheels(0, 0);
}


void Motor_Init()
{
	HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_2);
	Motor_Control(65);
	HAL_Delay(1000);
}
//延时0.8s保证姿态调整正常

void GraySensor_Init()
{
	GrayData[0] = 0;
	GrayData[1] = 0;
	GrayData[2] = 0;
	GrayData[3] = 0;
	GrayData[4] = 0;
}

