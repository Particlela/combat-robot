/*
 * move.h
 *
 *  Created on: Mar 15, 2024
 *      Author: 高恒斌
 */

#ifndef INC_MOVE_H_
#define INC_MOVE_H_

extern int GrayData[5];
extern int BackData[5];
extern unsigned char Control_message[50][7];

void Move_Left();
void Move_Right();
void Move_Twin_Wheels(int velocity_1 ,int velocity_2);
void Engine_Init();
void GraySensor_Init();
void Robotic_Arm_Move(int action_group);

#endif /* INC_MOVE_H_ */
