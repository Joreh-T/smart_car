//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:53:22
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-25 00:36:36
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

extern uint8 Island_State;
extern uint16 Island_Integrate;

extern uint8 Branch_State; 
extern uint8 Branc_Turn_Dire;
extern uint16 Branch_Integrate; 

extern uint16 speed;
extern float servo_error;

extern uint16 N_ADC[6];        // 归一化数据
extern uint16 Inductor_ADC[6]; // 实际采样值
extern uint16 Min_ADC[6];
extern uint16 Max_ADC[6];

#define BEEP_ON  P34 = 1; // beep
#define BEEP_OFF P34 = 0; // beep




#endif // !__SWK_HEADERFILE_H__
