//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 20:30:41
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-26 21:30:51
// @FilePath     : \STC16_V2\Project\USER\mycontrol\Communication.h
// @coding       : UTF-8
// @Description  : 多车通信
//============================================================================//

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "SWK_Headerfile.h"

#define Car_1_Out           0x01    // 出库
#define Car_1_Enter_Branch  0x02    // 头车第一次入三叉
#define Car_1_GoOut_Branch  0x03    // 头车出三叉
#define Car_2_Start_Branch  0x04    // 2号车在三叉启动行驶一段距离

// extern uint8 CH573_Rec_Command;
// extern uint16 distance;

void CH573_Send_Commands(uint8 Command);
// void CH573_Receive_Commands(uint8 Command);
void Analysis_CH573_Receive_Commands(void);
void Split_Ultrasonic_callback(uint8 buf);



#endif