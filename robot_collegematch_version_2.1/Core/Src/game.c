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
#include "game.h"

#define OUT 1
#define IN 0

int path1[30] ={3, 3, 2, 3, 0};

//舵机角度控制函数
//position:0 折叠； 1 收拢； 2 展开
//delay确保已转换完成

void Motor_Position(int position)
{
	if(position == 0)
	{
		Motor_Control(250);
	}
	else if(position == 1)
	{
		Motor_Control(195);
	}
	else if(position == 2)
	{
		Motor_Control(170);
	}

	HAL_Delay(800);
}

//控制电机直角转弯函数
//direction：1 前进； 2 后退
//Delay:560 空载90度左右；650 2物块90度左右

void Move_Turn(int direction)
{
	int Left_In = -67, Left_Out = 70;	//左初始速度
	int Right_In = -67, Right_Out = 70;	//右初始速度
	int Left_Speed = 0, Right_Speed = 0;//实际转化参数

    Move_Twin_Wheels(-70, -70);
	HAL_Delay(70);//310
	Move_Twin_Wheels(0, 0);
	HAL_Delay(100);

	Motor_Position(1);
    if(direction == 1 || direction == 2)
    {
	if(direction == 1)
	{
		Left_Speed = Left_In;
		Right_Speed = Right_Out;
	}
	else if(direction == 2)
	{
		Left_Speed = Left_Out;
		Right_Speed = Right_In;
	}

	if(Left_Speed > 100)
		Left_Speed = 100;
	else if (Left_Speed < -100)
		Left_Speed = -100;
	else if(Right_Speed > 100)
		Right_Speed = 100;
	else if (Right_Speed < -100)
		Right_Speed = -100;

	Move_Twin_Wheels(Left_Speed, Right_Speed);
	HAL_Delay(450);//22:32 400
    }
    else if(direction == 3)
    {
		Left_Speed = Left_In;
		Right_Speed = Right_Out;

		if(Left_Speed > 100)
			Left_Speed = 100;
		else if (Left_Speed < -100)
			Left_Speed = -100;
		else if(Right_Speed > 100)
			Right_Speed = 100;
		else if (Right_Speed < -100)
			Right_Speed = -100;

		Move_Twin_Wheels(Left_Speed, Right_Speed);
		HAL_Delay(2000);
    }
	Move_Twin_Wheels(0, 0);
	HAL_Delay(100);

}

//转弯姿态调整函数
//direction：1 左； 2 右； 3 掉头；
//基准速度设定60， 可带动3物块
//22:26 direction 1 2 300ms

void Position_Adj(int direction)
{
	int Adj_cnt = 0;		//计数值，确定调整时长
	int Left_Turn = 60;		//左基准初始速度
	int Right_Turn = 60;	//右基准初始速度
	int TurnLeft_adj = 1;	//左调整值
	int TurnRight_adj = 1;	//右调整值
	int Left_Speed = 0, Right_Speed = 0;
	int turn = 0, error = 0;

	if(direction == 1){
		turn = -1;
		Adj_cnt = 100;
	}else if(direction == 2){
		turn = 1;
		Adj_cnt = 100;
	}else if(direction == 3){
		turn = 1;
		Adj_cnt = 250;
	}

	for(int i = 0; i < Adj_cnt; i++)
	{
		if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == OUT)
		{
			error = 3;
		}
		else if(GrayData[1] == IN && GrayData[2] == OUT && GrayData[3] == OUT)
		{
			error = 2;
			turn = -1;
		}
		else if(GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == OUT)
		{
			error = 1;
			turn = -1;
		}
		else if((GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == OUT) || (GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN))
		{
			error = 0;
			turn = 0;
		}
		else if(GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == IN)
		{
			error = 1;
			turn = 1;
		}
		else if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == IN)
		{
			error = 2;
			turn = 1;
		}
		Left_Speed = turn * (Left_Turn + error * TurnLeft_adj);
		Right_Speed = (-1) * turn * (Right_Turn + error * TurnRight_adj);

		if(Left_Speed > 100)
				Left_Speed = 100;
		else if (Left_Speed < -100)
			Left_Speed = -100;
		else if(Right_Speed > 100)
			Right_Speed = 100;
		else if (Right_Speed < -100)
			Right_Speed = -100;

		Move_Twin_Wheels(Left_Speed, Right_Speed);
 		HAL_Delay(10); //是否延时由调试情况决定
	}
	Move_Twin_Wheels(0, 0);
	HAL_Delay(100);
}

//电机直行控制函数
//direction：1 前进； 2 后退

void Move_Straight(int direction)
{
	int Left_ori = 88;		//左初始值
	int Right_ori = 88;		//右初始值
	int GoLeft_adj = 1;		//左调整值
	int GoRight_adj = 1;		//右调整值
	int Left_Speed = 0, Right_Speed = 0;
	int Left_Times = 1, Right_Times = 1;
	int turn = 0, error = 0;

	if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == OUT)
	{
		error = 4;
	}
	else if(GrayData[1] == IN && GrayData[2] == OUT && GrayData[3] == OUT)
	{
		error = 2;
		turn = -1;
	}
	else if(GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == OUT)
	{
		error = 1;
		turn = -1;
	}
	else if((GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == OUT) || (GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN))
	{
		error = 0;
		turn = 0;
	}
	else if(GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == IN)
	{
		error = 1;
		turn = 1;
	}
	else if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == IN)
	{
		error = 2;
		turn = 1;
	}
//	if(GrayData[0] == OUT && GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == OUT && GrayData[4] == OUT)
//	{
//		error = 3;
//	}
//	else if(GrayData[0] == OUT && GrayData[1] == IN && GrayData[2] == OUT && GrayData[3] == OUT && GrayData[4] == OUT)
//	{
//		error = 2;
//		turn = -1;
//	}
//	else if(GrayData[0] == OUT && GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == OUT && GrayData[4] == OUT)
//	{
//		error = 1;
//		turn = -1;
//	}
//	else if((GrayData[0] == OUT && GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == OUT && GrayData[4] == OUT) || (GrayData[0] == OUT && GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN && GrayData[4] == OUT))
//	{
//		error = 0;
//		turn = 0;
//	}
//	else if(GrayData[0] == OUT && GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == IN && GrayData[4] == OUT)
//	{
//		error = 1;
//		turn = 1;
//	}
//	else if(GrayData[0] == OUT && GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == IN && GrayData[4] == OUT)
//	{
//		error = 2;
//		turn = 1;
//	}

	if(turn == -1){
		Left_Times = 2;
		Right_Times = 3;
	}else if(turn == 1){
		Left_Times = 3;
		Right_Times = 2;
	}

	Left_Speed = Left_ori + Left_Times * turn * error * GoLeft_adj;
	Right_Speed = Right_ori - Right_Times * turn * error * GoRight_adj;

	if(Left_Speed > 100)
		Left_Speed = 100;
	else if (Left_Speed < -100)
		Left_Speed = -100;
	else if(Right_Speed > 100)
		Right_Speed = 100;
	else if (Right_Speed < -100)
		Right_Speed = -100;

	Move_Twin_Wheels(Left_Speed, Right_Speed);
	HAL_Delay(10); //是否延时由调试情况决定
}

//上层路径运动函数
//status（决定下一步运动方向）
//	0 停止；1 左转；2 右转；3 前进； 4 后退

//void Engine_Control(int status)
//{
//
//	if(status == 0)
//	{
//		Move_Twin_Wheels(0, 0);
//	}
//	else if(status == 1)
//	{
//		Motor_Position(1);
//		Move_Turn(1);
//		Position_Adj(1);
//
//		Motor_Position(2);
//		Move_Straight(1);
//	}
//	else if(status == 2){
//		Motor_Position(1);
//		Move_Turn(2);
//		Position_Adj(2);
//
//		Motor_Position(2);
//		Move_Straight(1);
//	}
//	else if(status == 3)
//	{
//		Move_Straight(1);
//	}
//	else if(status == 4)
//	{
//		Motor_Position(1);
//		Move_Straight(2);
//	}
//
//}

//岔路口判断
//return：0 非；1 是

int Judge_cross(void)
{
	if(GrayData[0] == IN && GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN && GrayData[4] == IN)
	{
		return 1;
	}
	return 0;
}
