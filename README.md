# combat-robot
# Robot_version_1.0
## 代码思路
基础运动底层模块写在move.c文件中(包含了对应函数)
巡线转弯等复杂模块写在game.c文件中
通过数组确定运动逻辑
该代码为电池电量较充足时可以较好地完成拿3分的任务,预赛因电量变化出现误差
### 灰度传感器
始终都要工作，通过gpio口发送信息给主板进行处理，以达到巡线目的
### 接口
PE7-11被设置为五个灰度传感器的信号输入(高电平1 低电平0)
PE7接R2 橙色
PE8接R1 黄色
PE9接M 绿色
PE10接L1 蓝色
PE11接L2 紫色
要接5V和GND接口供电

### 差速两轮驱动小车
用了定时器从模式，频率为10kHz，TIM2主 TIM3 TIM4从
#### 接口
PD2对应IN1(白色)  PD3对应IN2(灰色)  IN1高IN2低左电机正转反之反转
PD0对应IN3(紫色)  PD1对应IN4(蓝色)  IN3高IN4低右电机正转反之反转
PA6接通道A(黑色)  PD12接通道B(绿色)
右轮 棕色上黑色下
左轮 紫色上蓝色下

#### 调试
**读转速要让定时器进入编码器模式**
接5V GND(L298N) 两个GPIO口(两个轮子都要)
50是最低极限
巡航占空比为88
#### 转速
TIM1(左)和TIM8(右)开编码器模式
TIM6计时10ms,每10ms读一次数据，最后输出转速
520B编码器:绿接地 蓝接3.3V 黄白连在(暂时不知道顺序)


### 舵机控制钳子
控制舵机可以只使用一个PWM信号，因为两个舵机只需要相对角度确定即可(TIM9)
#### 接口
接两个5V(红色) GND(棕色) PE6(左黄色) PE5(右黄色)

### 看门狗
防止出现故障却停不下的问题
1s重启一次,需要一直喂狗,注意关注能否每1s内能够喂狗
version1.0中删除了这一功能,因为开发者未能很好的使看门狗达到预期效果

### 蓝牙模块
用于发送代码进行调试
用TIM5确保非阻塞运行,频率为1Hz
暂时无法使用
#### 接口
接板子上写了USART2的四个接口

### 串口
目前使用串口收发数据,每50ms发送一次信息

### 断电与供电
长按key0断电,reset重启
key1将在校赛中有新的规划

## 算法
运用数组确定运动逻辑
通过调参实际测试确定参数,保证巡线与转弯准度
暂未实现PID控制算法
暂未实现读取电机转速
暂未修改车底板及设计(初步想法为框住物块,这样在运动中同时打乱空心圆柱位置)

# Robot_version_2.0
# Robot
## 代码思路
基础运动底层模块写在move.c文件中(包含了对应函数) 巡线转弯等复杂模块写在game.c文件中 通过数组确定运动逻辑 该代码为电池电量较充足时可以较好地完成拿3分的任务,预赛因电量变化出现误差
### 灰度传感器
始终都要工作，通过gpio口发送信息给主板进行处理，以达到巡线目的
### 接口
PE7-11被设置为五个灰度传感器的信号输入(高电平1 低电平0)
PE7接R2 紫色
PE8接R1 蓝色
PE9接M 绿色
PE10接L1 黄色
PE11接L2 橙色
要接5V和GND接口供电

### 差速两轮驱动小车
用了定时器从模式，频率为10kHz，TIM2主 TIM3 TIM4从
#### 接口
PD2对应IN1(红色)  PD3对应IN2(橙色)  IN1高IN2低左电机正转反之反转
PD0对应IN3(黄色)  PD1对应IN4(绿色)  IN3高IN4低右电机正转反之反转
PA6接通道A(棕色)  PD12接通道B(蓝色)
右轮 棕色上黑色下
左轮 紫色上蓝色下

#### 调试
**读转速要让定时器进入编码器模式**
接5V GND(L298N) 两个GPIO口(两个轮子都要)
50是最低极限 巡航占空比为88

#### 转速
TIM1(左)和TIM8(右)开编码器模式
TIM6计时10ms,每10ms读一次数据，最后输出转速
520B编码器:绿接地 蓝接3.3V 本版本为连在无输出的接口

### 舵机控制钳子
控制舵机可以只使用一个PWM信号，因为两个舵机只需要相对角度确定即可(TIM9)
#### 接口
接两个5V(红色) GND(棕色) PE5(左黄色) PE6(右黄色)

### 蓝牙模块
用于发送代码进行调试
用TIM5确保非阻塞运行，频率为1Hz
#### 接口
接板子上写了USART2的那四个接口

### 串口
目前使用串口收发数据,每50ms发送一次信息

### 断电与供电
长按key0断电,reset重启 按下key1调整路径

## 算法
运用数组确定运动逻辑 通过调参实际测试确定参数,保证巡线与转弯准度
目前为二维数组，储存不同的路径
*(初步想法为框住物块,这样在运动中同时打乱空心圆柱位置)*
已修改，目前为框住物块并在万向轮前设有挡板

## 改进
姿态调整能否使用递归确保能够调整至理想状态，避免使用PID并完成读转速？*未实现*

# Robot_version_3.14
有时间就写！
