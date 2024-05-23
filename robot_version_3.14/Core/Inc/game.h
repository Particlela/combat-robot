#ifndef __GAME_H_
#define __GAME_H_

extern int path1[30];

void Move_Turn(int direction, int first);
void Position_Adj(int direction);
void Move_Straight(int direction);
void Move_Stop(int direction);
int Judge_cross(void);

#endif
