//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-03-19 23:51:09
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-28 14:21:26
// @FilePath     : \STC16_V2\Project\USER\mycontrol\Servo_Control.c
// @coding       : UTF-8
// @Description  :
//============================================================================//
#include "SWK_Headerfile.h"

// uint8 Island_State = 0;
// uint16 Island_Integrate = 0; // 环岛转向积分
/**
 * @description: 环岛检测
 * @return <None>
 */
void Island_Judge(void) {
    switch (Island_State) {
    case IDLE_ISLAND:
        if (N_ADC[Left_Midle] >= 85 && N_ADC[Left_Inside] >= 95 && N_ADC[Right_Inside] >= 95 && (N_ADC[Right_Midle] <= 15 || N_ADC[Right_Midle] >= 50)) {
            Island_State = PASS_ISLAND; // 刚刚进环岛
        }
        break;
    case PASS_ISLAND:
        if (N_ADC[Left_Midle] <= 20 && N_ADC[Left_Inside] >= 95 && N_ADC[Right_Inside] >= 95 && N_ADC[Right_Midle] <= 12) {
            Island_State = READY_ISLAND; // 即将转向
        }
        break;
    case READY_ISLAND:
        if (N_ADC[Left_Midle] >= 30 && N_ADC[Left_Midle] <= 70 && N_ADC[Left_Inside] >= 90 && N_ADC[Right_Inside] >= 69 && N_ADC[Right_Midle] <= 20) {
            Island_State = EXECUTE_ISLAND; // 转向
        }
        break;
    case EXECUTE_ISLAND:
        BEEP_ON;
        Island_Integrate += speed;
        if (Island_Integrate < 2500) {
            pwm_duty(PWMA_CH4N_P17, 970); // 左转

        } else if (Island_Integrate >= 2500) {
            Island_Integrate = 0;
            Island_State = ENTER_ISLAND;
        }
        break;
    case ENTER_ISLAND:
        BEEP_OFF;
        Island_Integrate += speed;
        if (N_ADC[Left_Midle] >= 90 && (N_ADC[Left_Inside] > N_ADC[Right_Inside]) && N_ADC[Right_Midle] >= 95 && Island_Integrate > 1999) {
            Island_Integrate = 0;
            Island_State = 5; // 出环岛
        }
        break;
    case EXIT_ISLAND:
        BEEP_ON;
        Island_Integrate += speed;
        if (Island_Integrate < 3000) {
            pwm_duty(PWMA_CH4N_P17, 970);
        }
        // else if (Island_Integrate >= 2500 && Island_Integrate < 3500) {
        //     pwm_duty(PWMA_CH4N_P17, 750);
        // }
        else if (Island_Integrate > 3000) {
            BEEP_OFF;
            Island_Integrate = 0;
            Island_State = IDLE_ISLAND;
        }
        break;
    default:
        Island_State = Island_State;
        break;
    }
}

// uint8 Branch_State = 0;      // 三叉状态
// uint8 Branch_Turn_Dire = 0;   // 三叉转弯方向
// uint16 Branch_Integrate = 0; // 三叉转向积分
// uint16 speed;                // 编码器读取计数

/**
 * @description:
 * @return <None>
 */
void Branch_Road_Judge(void) {
    int judge;
    float O_compare;
    float I_compare;
    switch (Branch_State) {
    case 0:
        judge = ((N_ADC[Left_Inside] - N_ADC[Left_Midle]) * (N_ADC[Left_Inside] - N_ADC[Left_Midle]) + (N_ADC[Right_Inside] - N_ADC[Right_Midle]) * (N_ADC[Right_Inside] - N_ADC[Right_Midle])) /
                (N_ADC[Left_Inside] + N_ADC[Right_Inside]);

        O_compare = 1.26 * N_ADC[Left_Midle] / (float)(N_ADC[Right_Midle] + 1);
        I_compare = (float)N_ADC[Left_Inside] / (float)(N_ADC[Right_Inside] + 1);
        O_compare = (O_compare > 1.4) ? 0 : O_compare;
        I_compare = (I_compare > 1.3) ? 0 : I_compare;
        if (judge <= 20 && (O_compare > 0.60) && (I_compare > 0.70) && (N_ADC[Left_Inside] > N_ADC[Left_Midle]) && (N_ADC[Right_Inside] > N_ADC[Right_Midle]) && (N_ADC[Left_Inside] < 65) &&
            (N_ADC[Left_Midle] < 33) && (N_ADC[Right_Inside] < 65) && (N_ADC[Right_Midle] < 38) && (N_ADC[Left_Inside] > 28) && (N_ADC[Left_Midle] > 15) && (N_ADC[Right_Inside] > 28) &&
            (N_ADC[Right_Midle] > 15))
            Branch_State = 1;
        else
            Branch_State = Branch_State;

        // if (judge >= 35 && judge <= 60 && (I_compare > 0.70) && (N_ADC[Left_Inside] > 30+N_ADC   [Left_Midle]) && (N_ADC[Right_Inside] > 30+N_ADC[Right_Midle]) && (N_ADC[Left_Inside] < 80) &&
        // (N_ADC[Left_Midle] < 10) && (N_ADC[Right_Inside] < 80) && (N_ADC[Right_Midle] <10) && (N_ADC    [Left_Inside] > 50) && (N_ADC[Right_Inside] > 50))
        //     return 1;
        // else
        //     return 0;
        // if (judge <= 10 && abs(N_ADC[Left_Inside] - N_ADC[Right_Inside]) < 15 && abs(N_ADC[Left_Midle] -     N_ADC[Right_Midle]) < 15 && N_ADC[Left_Inside] > 5 &&N_ADC[Right_Inside] > 5)
        //     return 1;
        // else return 0;
        break;
    case 1:
        BEEP_ON;
        Branch_Integrate += speed;
        if (1 == Branch_Turn_Dire && Branch_Integrate < 2300) {
            pwm_duty(PWMA_CH4N_P17, 570); // 右转
        } else if (0 == Branch_Turn_Dire && Branch_Integrate < 2300) {
            pwm_duty(PWMA_CH4N_P17, 960); // 左转
        } else if (Branch_Integrate > 2300) {
            Branch_Integrate = 0;
            BEEP_OFF;
            Branch_State = 2;
        }
        break;
    case 2: // 强制转向结束，进入三叉
        BEEP_OFF;
        Branch_Integrate += speed;
        if (Branch_Integrate > 15000) {
            if (0x00 != CH573_Rec_Command) {
                CH573_Rec_Command = 0x00;
            }
            BEEP_ON;
        }
        if (Branch_Integrate > 17000) {
            Branch_Turn_Dire = (0 == Branch_Turn_Dire) ? 1 : 0;
            BEEP_OFF;
            Branch_Integrate = 0;
            Branch_State = 0;
        }
        break;
    default:
        Branch_State = Branch_State;
        break;
    }
}

/**
 * @description: 三叉判断 version-2
 * @return <None>
 */
void Branch_Road_Judge_2(void) {
    switch (Branch_State) {
    // 空闲状态
    case IDLE_BRANCH:
        // 竖直增，平行减
        if (Last_N_ADC[Left_Midle] < N_ADC[Left_Midle] && Last_N_ADC[Right_Midle] < N_ADC[Right_Midle] && Last_N_ADC[Left_Inside] > N_ADC[Left_Inside] &&
            Last_N_ADC[Right_Inside] > N_ADC[Right_Inside])
            Branch_Compare_Count++;
        if (Branch_Compare_Count > 8) {
            Branch_State = READY_BRANCH;
            Branch_Compare_Count = 0;
        }
        break;

    // 准备进入三叉
    case READY_BRANCH:
        judge = ((N_ADC[Left_Inside] - N_ADC[Left_Midle]) * (N_ADC[Left_Inside] - N_ADC[Left_Midle]) + (N_ADC[Right_Inside] - N_ADC[Right_Midle]) * (N_ADC[Right_Inside] - N_ADC[Right_Midle])) /
                (N_ADC[Left_Inside] + N_ADC[Right_Inside]);
        if (judge < 20 && N_ADC[Left_Midle] < 30 && N_ADC[Right_Midle] < 30 && N_ADC[Left_Midle] > 10 && N_ADC[Right_Midle] > 10 && N_ADC[Left_Inside] < 70 && N_ADC[Right_Inside] < 70 &&
            N_ADC[Left_Inside] > 40 && N_ADC[Right_Inside] > 40) {
            Branch_State = EXECUTE_BRANCH;
        }
        break;

    // 强制转向
    case EXECUTE_BRANCH:
        BEEP_ON;
        Branch_Integrate += speed;

        // 打角转向
        if (0 == Branch_Turn_Dire && Branch_Integrate < 2300) {
            pwm_duty(PWMA_CH4N_P17, 570 + 0.006 * Branch_Integrate); // 右转
        } else if (1 == Branch_Turn_Dire && Branch_Integrate < 2300) {
            pwm_duty(PWMA_CH4N_P17, 960 - 0.006 * Branch_Integrate); // 左转
        } else if (Branch_Integrate > 2300) {
            // 退出强制打角
            Branch_Integrate = 0;
            BEEP_OFF;
            Branch_State = ENTER_BRANCH;
        }
        break;

    // 强制转向结束，开始正常循迹（误差/3）
    case ENTER_BRANCH:
        BEEP_OFF;
        Branch_Integrate += speed;
        // 即将出三叉
        if (Branch_Integrate > 15000) {
            BEEP_ON;
        }
        if (Branch_Integrate > 17000) {
            Branch_Turn_Dire = !Branch_Turn_Dire;
            BEEP_OFF;
            Branch_Integrate = 0;
            Branch_State = IDLE_BRANCH;
        }
        break;
    default:
        Branch_State = Branch_State;
        break;
    }
}

uint8 Drive_Mode(void) {
    static uint8 mode = 0;
    // 1号车进入三叉减速且是第一圈
    if (Car_1_Enter_Branch == (CH573_Rec_Command & 0x3F) && IDLE_BRANCH == Branch_State && 0 == Branch_Turn_Dire) {
        mode = 1;
    }

    // 进入三叉且2号车还没发车继续保持低速且是第一圈
    if (ENTER_BRANCH == Branch_State && Car_2_Start_Branch != (CH573_Rec_Command & 0x3F) && 0 == Branch_Turn_Dire) {
        mode = 2;
    }

    // 2号车在三叉启动
    if (Car_2_Start_Branch == (CH573_Rec_Command & 0x3F)) {
        mode = 0;
    }

    return mode;

}
