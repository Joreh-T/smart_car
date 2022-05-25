//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:51:46
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-24 23:51:49
// @FilePath     : \STC16_V2\Project\USER\mycontrol\motor.h
// @coding       : UTF-8
// @Description  : 
//============================================================================//
#ifndef __CC_MOTOR_H
#define __CC_MOTOR_H
#include "SWK_Headerfile.h"

#define MOTOR1_P PWMB_CH4_P77
#define MOTOR1_N PWMB_CH3_P76

void motor_init(uint32 freq,uint32 dutyP,uint32 dutyN);
void motor_duty(int duty);

#endif