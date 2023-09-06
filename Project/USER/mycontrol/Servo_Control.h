//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:49:49
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-26 19:52:29
// @FilePath     : \STC16_V2\Project\USER\mycontrol\Servo_Control.h
// @coding       : UTF-8
// @Description  : 
//============================================================================//

#ifndef __SERVO_CONTROL_H__
#define __SERVO_CONTROL_H__

// 三叉各个状态
#define IDLE_BRANCH         0
#define READY_BRANCH        1
#define EXECUTE_BRANCH      2
#define ENTER_BRANCH        3 // 出三叉包含在该状态内


// 环岛各个状态
#define IDLE_ISLAND         0
#define PASS_ISLAND         1
#define READY_ISLAND        2
#define EXECUTE_ISLAND      3
#define ENTER_ISLAND        4
#define EXIT_ISLAND         5


void Branch_Road_Judge(void);
void Island_Judge(void);
void Branch_Road_Judge_2(void);
uint8 Drive_Mode(void);





#endif // !__SERVO_CONTROL_H__