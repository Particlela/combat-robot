#ifndef __GAME_H_
#define __GAME_H_

extern int path1[30];

void Motor_Position(int position);
void Move_Turn(int direction);
void Position_Adj(int direction);
void Move_Straight(int direction);

void Engine_Control(int status);
int Judge_cross(void);

#endif
