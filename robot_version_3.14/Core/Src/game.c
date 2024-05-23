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

//控制电机直角转弯函数
//direction：1 前进； 2 后退
//Delay:560 空载90度左右；650 2物块90度左右
int first = 1;

void Move_Turn(int direction, int first)
{
	int Left_In = -74, Left_Out = 68;	//左初始速度 	-67, 70
	int Right_In = -74, Right_Out = 68;	//右初始速度	-67, 70
	int Left_Speed = 0, Right_Speed = 0;//实际转化参数
	int Adj_cnt = 1000;		//计数值，确定调整时长
	int fact = 1;

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

	for(int i = 0; i < Adj_cnt && fact == 1; i++)
	{
		Move_Twin_Wheels(Left_Speed, Right_Speed);
		HAL_Delay(1);
		if(GrayData[1] == OUT  && GrayData[2] == OUT && GrayData[3] == OUT)
		{
			 fact = 0;
		}
	}

	fact = 1;

	for(int i = 0; i < Adj_cnt && fact == 1; i++)
	{
		Move_Twin_Wheels(Left_Speed, Right_Speed);
		HAL_Delay(1);
//		if((GrayData[1] == IN  && BackData[1] == IN && BackData[2] == IN) || (GrayData[2] == IN  && BackData[2] == IN && GrayData[3] == IN  && BackData[3] == IN))
		if((BackData[1] == IN && GrayData[1] == IN && BackData[2] == IN) || (GrayData[3] == IN  && BackData[2] == IN && BackData[3] == IN))
		{
			if(first == 0)
			{
				if(direction == 1)
				{
					Move_Twin_Wheels(82, -80); //85, -82
					HAL_Delay(50);
				}
				else if(direction == 2)
				{
					Move_Twin_Wheels(-80,78); //-90, 87
					HAL_Delay(50);
				}
			}

			 Move_Twin_Wheels(0, 0);
			 HAL_Delay(100);
			 fact = 0;
		}
	}
}

//电机直行控制函数
//direction：1 前进； 2 后退; 3 低速前行

int turn_s = 0,error_s = 0;

void Move_Straight(int direction)
{
	int Left_ori = 0, Right_ori = 0;//53
	int Left_Adj = 0, Right_Adj = 0;
	int Left_Speed = 0, Right_Speed = 0;
	int Error_S[5] = {0};

	if(direction == 1)
	{
		 Left_ori = 70;//53
		 Right_ori = 72;//50
		 Left_Adj = 2;
		 Right_Adj = 2;
		 Left_Speed = 0, Right_Speed = 0;
		 int Error_set[5] = {1,4,3,5,10}; //1 5 4 5 5

		 for(int i = 0; i < 5; i++)
		 {
			 Error_S[i] = Error_set[i];
		 }

	    if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == OUT)
	    {
	    	error_s = Error_S[4];
	    }
	    else if(GrayData[1] == IN && GrayData[2] == OUT && GrayData[3] == OUT && BackData[1] == IN && BackData[3] == OUT)
		{
	    	turn_s = -1;
	    	error_s = Error_S[3];
		}
	    else if(GrayData[1] == IN && GrayData[2] == OUT && GrayData[3] == OUT && BackData[1] == OUT && BackData[3] == IN)
		{
			turn_s = -1;
			error_s = Error_S[2];
		}
	    else if(GrayData[1] == IN && GrayData[2] == OUT && GrayData[3] == OUT)
		{
			turn_s = -1;
			error_s = Error_S[1];
		}
		else if(GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == OUT)
		{
			turn_s = -1;
			error_s = Error_S[0];
		}
		else if(GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN)
		{
			turn_s = 0;
		}
		else if(GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == IN)
		{
			turn_s = 1;
			error_s = 3;
		}
		else if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == IN && BackData[1] == OUT && BackData[3] == IN)
		{
			turn_s = 1;
			error_s = Error_S[3];
		}
		else if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == IN && BackData[1] == IN && BackData[3] == OUT)
		{
			turn_s = 1;
			error_s = Error_S[2];
		}
		else if(GrayData[1] == OUT && GrayData[2] == OUT && GrayData[3] == IN)
		{
			turn_s = 1;
			error_s = Error_S[1];
		}

	    if(turn_s == -1)
		{
			Left_Speed = Left_ori;
			Right_Speed = (Right_ori + error_s * Right_Adj);
		}
		else if(turn_s == 1)
		{
			Left_Speed = (Left_ori + error_s * Left_Adj);
			Right_Speed = Right_ori;
		}
		else if(turn_s == 0)
		{
			Left_Speed = Left_ori;
			Right_Speed = Right_ori;
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
	}
	else if(direction == 2)
	{
		 Left_ori = 72;//53
		 Right_ori = 70;//50
		 Left_Adj = 2;
		 Right_Adj = 2;
		 Left_Speed = 0, Right_Speed = 0;
		 int Error_set[5] = {2,4,3,5,10};

		 for(int i = 0; i < 5; i++)
		 {
			 Error_S[i] = Error_set[i];
		 }

		if(BackData[3] == OUT && BackData[2] == OUT && BackData[1] == OUT)
		{
			error_s = Error_S[4];
		}
		else if(BackData[3] == IN && BackData[2] == OUT && BackData[1] == OUT && GrayData[3] == IN && GrayData[1] == OUT)
		{
			turn_s = -1;
			error_s = Error_S[3];
		}
		else if(BackData[3] == IN && BackData[2] == OUT && BackData[1] == OUT && GrayData[3] == OUT && GrayData[1] == IN)
		{
			turn_s = -1;
			error_s = Error_S[2];
		}
		else if(BackData[3] == IN && BackData[2] == OUT && BackData[1] == OUT)
		{
			turn_s = -1;
			error_s = Error_S[1];
		}
		else if(GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == OUT)
		{
			turn_s = -1;
			error_s = Error_S[0];
		}
		else if(GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN)
		{
			turn_s = 0;
		}
		else if(GrayData[1] == OUT && GrayData[2] == IN && GrayData[3] == IN)
		{
			turn_s = 1;
			error_s = 3;
		}
		else if(BackData[3] == OUT && BackData[2] == OUT && BackData[1] == IN && GrayData[3] == OUT && GrayData[1] == IN)
		{
			turn_s = 1;
			error_s = Error_S[3];
		}
		else if(BackData[3] == OUT && BackData[2] == OUT && BackData[1] == IN && GrayData[3] == IN && GrayData[1] == OUT)
		{
			turn_s = 1;
			error_s = Error_S[2];
		}
		else if(BackData[3] == OUT && BackData[2] == OUT && BackData[1] == IN)
		{
			turn_s = 1;
			error_s = Error_S[1];
		}

		if(turn_s == -1)
		{
//				Left_Speed = -Left_ori;
//				Right_Speed = -(Right_ori + error_s * Right_Adj);
			Left_Speed = 60;
			Right_Speed = -Right_ori;
		}
		else if(turn_s == 1)
		{
//				Left_Speed = -(Left_ori + error_s * Left_Adj);
//				Right_Speed = -Right_ori;
			Left_Speed = -Left_ori;
			Right_Speed = 60;
		}
		else if(turn_s == 0)
		{
			Left_Speed = -Left_ori;
			Right_Speed = -Right_ori;
		}

		if(Left_Speed > 100)
			Left_Speed = 100;
		else if (Left_Speed < -100)
			Left_Speed = -100;
		else if(Right_Speed > 100)
			Right_Speed = 100;
		else if (Right_Speed < -100)
			Right_Speed = -100;

		Move_Twin_Wheels(Right_Speed, Left_Speed);
	}
}

//直行停止函数
//direction: 0 0物块前进后停止；1 1物块前进后停止；2 2物块前进后停止；3 后退后停止

void Move_Stop(int direction)
{
	if(direction == 0)
	{
		int fact = 1;

		for(int i = 0; i < 1000 && fact == 1; i++)
		{
			Move_Straight(1);
			HAL_Delay(1);
			if(BackData[0] == IN && BackData[1] == IN && BackData[2] == IN && BackData[3] == IN && BackData[4] == IN )
			{
				Move_Twin_Wheels(-80, -80);			//0, 0
				HAL_Delay(50  );	//100			//50
				Move_Twin_Wheels(0, 0);			//0, 0
				HAL_Delay(100);	//100			//50
				fact = 0;
			}
		}
	}
	else if(direction == 1)
	{
		Move_Twin_Wheels(-72, -70);		//-70, -70
	    HAL_Delay(50);	//70			//120
		Move_Twin_Wheels(0, 0);			//0, 0
		HAL_Delay(100);	//100			//50
	}
	else if(direction == 2)
	{
		Move_Twin_Wheels(-67, -67);		//-70, -70
		HAL_Delay(19);	//70			//120
		Move_Twin_Wheels(0, 0);			//0, 0
		HAL_Delay(100);	//100			//50
	}
}

int Judge_cross(void)
{
	if(GrayData[0] == IN && GrayData[1] == IN && GrayData[2] == IN && GrayData[3] == IN && GrayData[4] == IN)
	{
		return 1;
	}
	return 0;
}

//
//const int Cross_Times = 10;
//int Cross_Check = 0, Cross_fact = 0;
//
//int Judge_cross(void)
//{
//	if(GrayData[0] == OUT && GrayData[1] == OUT)
//	{
//		Cross_Check++;
//		HAL_Delay(1);
//		if(Cross_Check >= Cross_Times && Cross_fact == 1)
//		{
//			Cross_fact = 0;
//			Cross_Check = 0;
//			return 1;
//		}
//	}
//	else if(GrayData[0] == IN && GrayData[1] == IN)
//	{
//		Cross_Check++;
//		HAL_Delay(1);
//		if(Cross_Check >= Cross_Times && Cross_fact == 0)
//		{
//			Cross_fact =1;
//			Cross_Check = 0;
//		}
//	}
//	else
//	{
//		Cross_Check = 0;
//	}
//	return 0;
//}
