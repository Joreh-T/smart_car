//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:42:13
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-25 15:07:31
// @FilePath     : \STC16_V2\Project\USER\src\SWK_Led.c
// @coding       : UTF-8
// @Description  : 
//============================================================================//

#include "SWK_Led.h"
/*
 * @brief  led初始化
 * @param  LEDx led_no 编号
 * @retval 无
 * @date   2022-01-18 13:13
 */
void Led_Init(LEDx LED_no) {
    switch (LED_no) {
    // 设置推挽输出 
    case LED0:
        P4M1 &= 0xFB;
        P4M1 |= 0x00;
        P4M0 &= 0xFB;
        P4M0 |= 0xF4;
        break;
    case LED1:
        P6M1 &= 0xFB;
        P6M1 |= 0x00;
        P6M0 &= 0xFB;
        P6M0 |= 0xF4;
        break;
    // case LED3:
    //     break;
    // case LED4:
    //     break;
    case LED_ALL:
        P4M1 &= 0xFB;
        P4M1 |= 0x00;
        P4M0 &= 0xFB;
        P4M0 |= 0xF4;

        P6M1 &= 0xFB;
        P6M1 |= 0x00;
        P6M0 &= 0xFB;
        P6M0 |= 0xF4;
        break;
    default:
        break;
    }
}

/*
 * @brief  LED灯控制
 * @param  LEDx LED_no编号，LED_state sta 状态亮/灭/翻转
 * @retval 
 * @date   2022-01-18 13:49
 */
void LED_Ctrl(LEDx LED_no, LED_state sta) {
    switch (LED_no)
    {
    case LED0:
        if (ON == sta)          LED0p = 0;
        else if (OFF == sta)    LED0p = 1;
        else if (RVS == sta)    (LED0p)?(LED0p = 0):(LED0p = 1);
        break;
    case LED1:
        if (ON == sta)          LED1p = 0;
        else if (OFF == sta)    LED1p = 1;
        else if (RVS == sta)    (LED1p)?(LED1p = 0):(LED1p = 1);
        break;
    // case LED2:
    //     /* code */
    //     break;
    // case LED3:
    //     /* code */
    //     break;
    case LED_ALL:
        if (ON == sta) {
            LED0p = 0;
            LED1p = 0;
            // LED2p = 0;
            // LED3p = 0;
        }
        else if (OFF == sta) {
            LED0p = 1;
            LED1p = 1;
            // LED2p = 1;
            // LED3p = 1;
        }
        else if (RVS == sta) {
            (LED0p)?(LED0p = 0):(LED0p = 1);
            (LED1p)?(LED1p = 0):(LED1p = 1);
            // (LED2p)?(LED2p = 0):(LED2p = 1);
            // (LED3p)?(LED3p = 0):(LED3p = 1);
        }
        break;
    default:
        break;
    }
}
