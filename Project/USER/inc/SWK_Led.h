//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 20:43:44
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-25 00:39:19
// @FilePath     : \STC16_V2\Project\USER\inc\SWK_Led.h
// @coding       : UTF-8
// @Description  : 
//============================================================================//
#ifndef __LED_H__
#define __LED_H__

#include "headfile.h"

//定义的管脚要对应实际按键
#define LED0p      P42   //核心板上LED0 翠绿
#define LED1p      P62   //核心板上LED1 蓝灯
// #define LED2p      P22   //母板上LED0
// #define LED3p      P23   //母板上LED1



typedef enum {
    LED0 = 0, //核心板上LED0 蓝灯
    LED1 = 1, //核心板上LED1 绿灯
    //   LED2=2,  //母板上LED0
    //   LED3=3,  //母板上LED1
    LED_ALL = 4
} LEDx;

typedef enum {
    ON  = 0,  //亮
    OFF = 1,  //灭
    RVS = 2,  //反转
} LED_state;

void Led_Init(LEDx LED_no);
void LED_Ctrl(LEDx LED_no, LED_state state);
#endif