//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-03-19 17:42:24
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-27 20:50:53
// @FilePath     : \STC16_V2\Project\USER\mycontrol\Error_Model.c
// @coding       : UTF-8
// @Description  : 
//============================================================================//

#include "Error_Model.h"
#include "headfile.h"

extern uint8 data_byte[4];
extern const uint8 tail[4];
// uint16 N_ADC[6] = {0};        // 归一化数据
// uint16 Inductor_ADC[6] = {0}; // 实际采样值
// uint16 Min_ADC[6] = {120, 120, 110, 120, 120, 120};
// uint16 Max_ADC[6] = {550, 1000, 1300, 1300, 1000, 550};

// 最小二乘法
POINTS L_V_points[POINT_NUM]; //左边竖直电磁存放点数据
POINTS L_P_points[POINT_NUM]; //左边平行电磁存放点数据

POINTS R_V_points[POINT_NUM]; //右边边竖直电磁存放点数据
POINTS R_P_points[POINT_NUM]; //右边平行电磁存放点数据

float L_V_K, L_P_K, R_P_K, R_V_K; // 左边竖直、左边平行、右边平行、右边竖直斜率
float L_V_B, L_P_B, R_P_B, R_V_B; // 左边竖直、左边平行、右边平行、右边竖直截距

uint8 KB_COUNT; // 最小二乘法采集数据次数计数

/*
 * @brief  电感采集磁场模型-1
 * @param  Model_Param* 待初始化参数结构体指针
 * @param  out_num      分子参数-外部电感
 * @param  ins_num      分子参数-内部电感
 * @param  out_den      分母参数-外部电感
 * @param  ins_den      分母参数-内部电感
 * @retval None
 * @date   2022-03-19 15: 19
 */
void Model_Param_init(Model_Param *P, float out_num, float ins_num, float out_den, float ins_den) {
    P->out_numerator = out_num;
    P->ins_numerator = ins_num;
    P->out_denominator = out_den;
    P->ins_denominator = ins_den;
}

/*
 * @brief  车头偏离赛道方向模型
 * @param  L_Outside        左边外部电感采集数据
 * @param  L_Inside         左边内部电感采集数据
 * @param  R_Outside        右边外部电感采集数据
 * @param  R_Inside         右边内部电感采集数据
 * @param  Model_Param      参数结构体
 * @retval
 * @date   2022-03-19 15: 26
 */

// version 1
// float Get_Err(float L_Outside, float L_Inside, float R_Outside, float R_Inside, Model_Param P) {
//     float err;
//     // oled_printf_float(50,  1, P.ins_numerator,5,1);
//     err = (P.ins_numerator*(L_Inside - R_Inside) + P.out_numerator*(L_Outside - R_Outside))
//         / (P.ins_denominator*(L_Inside - R_Inside) + P.out_denominator*(L_Outside - R_Outside));
//     return err;
// }

// version 2
float Get_Err(float L_Outside, float L_Inside, float R_Outside, float R_Inside, Model_Param P) {
    float err;
    // oled_printf_float(50,  1, P.ins_numerator,5,1);
    err = 100 * (P.ins_numerator * (L_Inside - R_Inside) + P.out_numerator * (L_Outside - R_Outside)) /
          (P.ins_denominator * (L_Inside + R_Inside) * (L_Inside + R_Inside) + P.out_denominator * (fabs(L_Outside - R_Outside)));
    return err;
}

/*
 * @brief  磁场数据刷新并归一化
 * @param  None
 * @retval None
 * @date   2022-03-19 18: 30
 */



void MagneticField_Data_refresh(void) {
    uint8 times = 2;
    static uint8 compare_times = 0;
    // Inductor_ADC[Left_Outside] = 0;
    Inductor_ADC[Left_Midle] = 0;
    Inductor_ADC[Left_Inside] = 0;
    Inductor_ADC[Right_Inside] = 0;
    Inductor_ADC[Right_Midle] = 0;
    // Inductor_ADC[Right_Outside] = 0;
    while (times--) {
        // Inductor_ADC[Left_Outside]  += adc_once(ADC_P00, ADC_12BIT);
        Inductor_ADC[Left_Midle] += adc_once(ADC_P01, ADC_12BIT);
        Inductor_ADC[Left_Inside] += adc_once(ADC_P02, ADC_12BIT);
        Inductor_ADC[Right_Inside] += adc_once(ADC_P03, ADC_12BIT);
        Inductor_ADC[Right_Midle] += adc_once(ADC_P10, ADC_12BIT);
        // Inductor_ADC[Right_Outside] += adc_once(ADC_P11, ADC_12BIT);
        _nop_();
        _nop_();
    }
    // 除以2
    // Inductor_ADC[Left_Outside]  = Inductor_ADC[Left_Outside]>>1;
    Inductor_ADC[Left_Midle] = Inductor_ADC[Left_Midle] >> 1;
    Inductor_ADC[Left_Inside] = Inductor_ADC[Left_Inside] >> 1;
    Inductor_ADC[Right_Inside] = Inductor_ADC[Right_Inside] >> 1;
    Inductor_ADC[Right_Midle] = Inductor_ADC[Right_Midle] >> 1;
    // Inductor_ADC[Right_Outside] = Inductor_ADC[Right_Outside]>>1;

    // 限幅
    Inductor_ADC[Left_Midle] =
        (Inductor_ADC[Left_Midle] > Max_ADC[Left_Midle]) ? Max_ADC[Left_Midle] : ((Inductor_ADC[Left_Midle] < Min_ADC[Left_Midle]) ? Min_ADC[Left_Midle] : Inductor_ADC[Left_Midle]);

    Inductor_ADC[Left_Inside] =
        (Inductor_ADC[Left_Inside] > Max_ADC[Left_Inside]) ? Max_ADC[Left_Inside] : ((Inductor_ADC[Left_Inside] < Min_ADC[Left_Inside]) ? Min_ADC[Left_Inside] : Inductor_ADC[Left_Inside]);

    Inductor_ADC[Right_Inside] =
        (Inductor_ADC[Right_Inside] > Max_ADC[Right_Inside]) ? Max_ADC[Right_Inside] : ((Inductor_ADC[Right_Inside] < Min_ADC[Right_Inside]) ? Min_ADC[Right_Inside] : Inductor_ADC[Right_Inside]);

    Inductor_ADC[Right_Midle] =
        (Inductor_ADC[Right_Midle] > Max_ADC[Right_Midle]) ? Max_ADC[Right_Midle] : ((Inductor_ADC[Right_Midle] < Min_ADC[Right_Midle]) ? Min_ADC[Right_Midle] : Inductor_ADC[Right_Midle]);

    // oled_printf_float(0,  3, Inductor_ADC[Left_Outside],5,1);
    // oled_printf_float(50, 3, Inductor_ADC[Left_Inside],5,1);
    // oled_printf_float(50, 5, Inductor_ADC[Right_Outside],5,1);
    // oled_printf_float(0, 5, Inductor_ADC[Right_Inside],5,1);

    /*******************************不刷新最大最小值 ******************
     if (Inductor_ADC[Left_Outside] < Min_ADC[Left_Outside])
         Min_ADC[Left_Outside] = Inductor_ADC[Left_Outside];  // 刷新最小值
     else if (Inductor_ADC[Left_Outside] > Max_ADC[Left_Outside])
         Max_ADC[Left_Outside] = Inductor_ADC[Left_Outside];  // 刷新最大值

     // if (Inductor_ADC[Left_Midle] < Min_ADC[Left_Midle])
     //     Min_ADC[Left_Midle] = Inductor_ADC[Left_Midle];
     // else if (Inductor_ADC[Left_Midle] > Max_ADC[Left_Midle])
     //     Max_ADC[Left_Midle] = Inductor_ADC[Left_Midle];

     if (Inductor_ADC[Left_Inside] < Min_ADC[Left_Inside])
         Min_ADC[Left_Inside] = Inductor_ADC[Left_Inside];
     else if (Inductor_ADC[Left_Inside] > Max_ADC[Left_Inside])
         Max_ADC[Left_Inside] = Inductor_ADC[Left_Inside];

     if (Inductor_ADC[Right_Inside] < Min_ADC[Right_Inside])
         Min_ADC[Right_Inside] = Inductor_ADC[Right_Inside];
     else if (Inductor_ADC[Right_Inside] > Max_ADC[Right_Inside])
         Max_ADC[Right_Inside] = Inductor_ADC[Right_Inside];

     // if (Inductor_ADC[Right_Midle] < Min_ADC[Right_Midle])
     //     Min_ADC[Right_Midle] = Inductor_ADC[Right_Midle];
     // else if (Inductor_ADC[Right_Midle] > Max_ADC[Right_Midle])
     //     Max_ADC[Right_Midle] = Inductor_ADC[Right_Midle];

     if (Inductor_ADC[Right_Outside] < Min_ADC[Right_Outside])
         Min_ADC[Right_Outside] = Inductor_ADC[Right_Outside];
     else if (Inductor_ADC[Right_Outside] > Max_ADC[Right_Outside])
         Max_ADC[Right_Outside] = Inductor_ADC[Right_Outside];
     *******************************************************************/

    // 归一化到0--100
    // N_ADC[Left_Outside] = 102*(Inductor_ADC[Left_Outside] - Min_ADC[Left_Outside])
    //                              / (Max_ADC[Left_Outside] - Min_ADC[Left_Outside]);
    if (2 == compare_times) {
        Last_N_ADC[Left_Midle]   = N_ADC[Left_Midle];
        Last_N_ADC[Left_Inside]  = N_ADC[Left_Inside];
        Last_N_ADC[Right_Inside] = N_ADC[Right_Inside];
        Last_N_ADC[Right_Midle]  = N_ADC[Right_Midle];
        compare_times = 0;
    }
    else
        compare_times++;

    N_ADC[Left_Midle] = (100.0 * (Inductor_ADC[Left_Midle] - Min_ADC[Left_Midle]) / (Max_ADC[Left_Midle] - Min_ADC[Left_Midle]));
    N_ADC[Left_Inside] = (100.0 * (Inductor_ADC[Left_Inside] - Min_ADC[Left_Inside]) / (Max_ADC[Left_Inside] - Min_ADC[Left_Inside]));
    N_ADC[Right_Inside] = (100.0 * (Inductor_ADC[Right_Inside] - Min_ADC[Right_Inside]) / (Max_ADC[Right_Inside] - Min_ADC[Right_Inside]));
    N_ADC[Right_Midle] = (100.0 * (Inductor_ADC[Right_Midle] - Min_ADC[Right_Midle]) / (Max_ADC[Right_Midle] - Min_ADC[Right_Midle]));

    // 线性回归
    /*
    L_V_points[KB_COUNT].x = KB_COUNT;
    L_V_points[KB_COUNT].y = N_ADC[Left_Midle];

    L_P_points[KB_COUNT].x = KB_COUNT;
    L_P_points[KB_COUNT].y = N_ADC[Left_Inside];

    R_P_points[KB_COUNT].x = KB_COUNT;
    R_P_points[KB_COUNT].y = N_ADC[Right_Inside];

    R_V_points[KB_COUNT].x = KB_COUNT;
    R_V_points[KB_COUNT].y = N_ADC[Right_Midle];

    KB_COUNT++;

    if (KB_COUNT > 19) {
        Least_square_method(L_V_points, KB_COUNT-1, &L_V_K, &L_V_B);
        Least_square_method(R_V_points, KB_COUNT-1, &L_P_K, &L_P_B);
        Least_square_method(L_P_points, KB_COUNT-1, &R_P_K, &R_P_B);
        Least_square_method(R_P_points, KB_COUNT-1, &R_V_K, &R_V_B);

        VOFA_Send_float(L_V_K, data_byte);
        VOFA_Send_float(L_P_K, data_byte);
        VOFA_Send_float(R_P_K, data_byte);
        VOFA_Send_float(R_V_K, data_byte);
        VOFA_Send_Tail();
        KB_COUNT = 0;
    }
    */

    
    
    // N_ADC[Right_Outside] = 102*(Inductor_ADC[Right_Outside] - Min_ADC[Right_Outside])
    //                              / (Max_ADC[Right_Outside] - Min_ADC[Right_Outside]);

    // Inductor_ADC[Left_Outside]  = adc_once(ADC_P00, ADC_12BIT);
    // Inductor_ADC[Left_Midle]    = adc_once(ADC_P01, ADC_12BIT);
    // Inductor_ADC[Left_Inside]   = adc_once(ADC_P02, ADC_12BIT);
    // Inductor_ADC[Right_Inside]  = adc_once(ADC_P03, ADC_12BIT);
    // Inductor_ADC[Right_Midle]   = adc_once(ADC_P10, ADC_12BIT);
    // Inductor_ADC[Right_Outside] = adc_once(ADC_P11, ADC_12BIT);

    // oled_int16(0, 0, Inductor_ADC[Left_Outside]);
    // oled_int16(0, 0, Inductor_ADC[Left_Midle]);
    // oled_int16(0, 1, Inductor_ADC[Left_Inside]);
    // oled_int16(0, 2, Inductor_ADC[Right_Inside]);
    // oled_int16(0, 3, Inductor_ADC[Right_Midle]);
    // // oled_int16(0, 5, Inductor_ADC[Right_Outside]);

    // oled_int16(70, 0, N_ADC[Left_Midle]);
    // oled_int16(70, 1, N_ADC[Left_Inside]);
    // oled_int16(70, 2, N_ADC[Right_Inside]);
    // oled_int16(70, 3, N_ADC[Right_Midle]);

    VOFA_Send_float(N_ADC[Left_Midle], data_byte);
    VOFA_Send_float(N_ADC[Left_Inside], data_byte);
    VOFA_Send_float(N_ADC[Right_Inside], data_byte);
    VOFA_Send_float(N_ADC[Right_Midle], data_byte);
    VOFA_Send_Tail();
}

/*
要求的方程为: y=ax+b
          N∑xy-∑x∑y                 _    _
其中：a = ---------------        b = y - ax
          N∑(x^2)-(∑x)^2
设：A=∑xy  B=∑x  C=∑y  D=∑(x^2)
           NA-BC
则：  a = -------    b = C/N-aB/N
           ND-BB
注：N为要拟合的点数量
*/

/*
参数说明：
        P[POINT_NUM]：传入要线性拟合的点数据（结构体数组）
        N：线性拟合的点的数量
*/


/**
 * @description: 
 * @param  <POINTS> P 点数据数组
 * @param  <int>    N 点个数
 * @param  <float*> k 接收计算斜率结果指针
 * @param  <float*> b 接收计算截距结果指针
 * @return <None>
 */

void Least_square_method(POINTS P[], int N, float* k, float* b) {
    int i = 0;
    float k0 = 0, b0 =0;
    float A = 0, B = 0, C = 0, D = 0;
    for (i = 0; i < N; i++) {
        A += P[i].x * P[i].y;
        B += P[i].x;
        C += P[i].y;
        D += P[i].x * P[i].x;
    }
    k0 = (N * A - B * C) / (N * D - B * B);
    b0 = C / N - k0 * B / N;
    *k = 100*k0;
    *b = 100*b0;
}

