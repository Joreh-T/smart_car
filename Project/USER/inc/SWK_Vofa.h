//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-03-09 10:49:58
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-20 21:55:10
// @FilePath     : \Seekfree_STC16F40K128\Project\USER\inc\SWK_Vofa.h
// @coding       : UTF-8
// @Description  : 
//============================================================================//
#ifndef __SWK_VOFA_H__
#define __SWK_VOFA_H__

#include "headfile.h"

typedef union {
    float f_data;
    unsigned long l_data;
} FloatLongType;

// uint8 data_byte[4];
// const uint8 tail[4];

void Float_to_Byte(float statistic, uint8 *data_byte);
void VOFA_Send_float(float statistic, uint8 *data_byte);
void VOFA_Send_Tail(void);

extern uint8 length;
extern uint8 data_byte[4];

#endif