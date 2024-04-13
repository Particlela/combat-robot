/*
 * move.h
 *
 *  Created on: Mar 15, 2024
 *      Author: 高恒斌
 */

#ifndef INC_MOVE_H_
#define INC_MOVE_H_

typedef struct _Engine
{
	int lastAngle;        //上10ms转过的角度
	int totalAngle;       //总的角度
	int loopNum;          //溢出次数计数值
	float speed;              //电机输出轴目前转速,单位为RPM
}Engine;

extern int GrayData[5];
extern float Motor_angel;
extern Engine EngineLeft;
extern Engine EngineRight;


#define RR 56u    //电机减速比
#define RELOADVALUELEFT __HAL_TIM_GetAutoreload(&htim1)    //获取左轮自动装载值
#define RELOADVALUERIGHT __HAL_TIM_GetAutoreload(&htim8)    //获取左轮自动装载值
#define COUNTERNUMLEFT __HAL_TIM_GetCounter(&htim1)        //获取左轮编码器定时器中的计数值
#define COUNTERNUMRIGHT __HAL_TIM_GetCounter(&htim8)        //获取右轮编码器定时器中的计数值

void Move_Left();
void Move_Right();
float Read_Rotational_Speed(int n);
void Motor_Control(int duty_cycle);
void Move_Twin_Wheels(int velocity_1 ,int velocity_2);
void Motor_Init();
void Engine_Init();
void GraySensor_Init();


#endif /* INC_MOVE_H_ */
