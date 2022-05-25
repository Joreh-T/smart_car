//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-03-19 18:06:24
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-25 00:32:31
// @FilePath     : \STC16_V2\Project\USER\mycontrol\Error_Model.h
// @coding       : UTF-8
// @Description  : 
//============================================================================//

#ifndef __ERROR_MODEL_H__
#define __ERROR_MODEL_H__

#include "SWK_Headerfile.h"
#include <MATH.H>

typedef struct {
    float out_numerator;    // 外侧电感系数，分子
    float ins_numerator;    // 内侧电感系数，分子

    float out_denominator;  // 外侧电感系数，分母
    float ins_denominator;  // 内侧电感系数，分母
}Model_Param;

#define POINT_NUM 30  //最多输入30个点进行线性拟和
// 线性回归点
typedef struct POINT {
    double x;
    double y;
}POINTS;

// ADC采集编号
typedef enum {
    Left_Outside = 0    ,
    Left_Midle          ,
    Left_Inside         ,

    Right_Inside        ,
    Right_Midle         ,
    Right_Outside       ,
} ADCx_Position;

extern uint16 N_ADC[6];
extern float L_V_K, L_P_K, R_P_K, R_V_K; // 左边竖直、左边平行、右边平行、右边竖直斜率
extern float L_V_B, L_P_B, R_P_B, R_V_B; // 左边竖直、左边平行、右边平行、右边竖直截距
void Model_Param_init(Model_Param* P, float out_num, float ins_num, float out_den, float ins_den);
float Get_Err(float L_Outside, float L_Inside, float R_Outside, float R_Inside, Model_Param P);
void MagneticField_Data_refresh(void);
void Least_square_method(POINTS P[], int N, float * k, float * b);

#endif // !__ERROR_MODEL_H__
