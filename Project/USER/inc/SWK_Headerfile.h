//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:53:22
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-27 21:59:22
// @FilePath     : \STC16_V2\Project\USER\inc\SWK_Headerfile.h
// @coding       : UTF-8
// @Description  : 
//============================================================================//

#ifndef __SWK_HEADERFILE_H__
#define __SWK_HEADERFILE_H__

#include "headfile.h"
#include "SWK_Led.h"
#include "SWK_Vofa.h"
#include "Communication.h"
#include "Error_Model.h"
#include "motor.h"
#include "PID.h"
#include "Servo_Control.h"


#include <MATH.H>

extern uint8  Island_State;
extern uint16 Island_Integrate;

extern uint8  Branch_State; 
extern uint8  Branch_Compare_Count;
extern uint8  Branch_Turn_Dire;
extern uint16 Branch_Integrate; 
extern int    judge;

extern uint16 speed;
extern float servo_error;

extern uint16 N_ADC[6];        // 归一化数据
extern uint16 Last_N_ADC[6];
extern uint16 Inductor_ADC[6]; // 实际采样值
extern uint16 Min_ADC[6];
extern uint16 Max_ADC[6];

extern uint8  CH573_Rec_Command; // 接收到的命令
extern uint16 distance;         // 两车之间的距离
// extern uint8  Analyzed_Command;

#define BEEP_ON  P34 = 1; // beep
#define BEEP_OFF P34 = 0; // beep

#define STRAIGHT_SPEED          50
#define CORNER_SPEED            40

#define EXECUTE_BRANCH_SPEED    20
#define EXECUTE_ISLAND_SPEED    20
#define ENTER_BRANCH_SPEED      35




#endif // !__SWK_HEADERFILE_H__
