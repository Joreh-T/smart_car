//============================================================================//
// Author      : 孙雾崆
// Date        : 2021-07-03 12:12
// Editor      : VScode Tab size(4)
// Encode      : UTF-8
//============================================================================//

#include "SWK_Vofa.h"

extern uint8 busy[5];
uint8 length;
uint8 data_byte[4] = {0};
const uint8 tail[4] = {0X00, 0X00, 0X80, 0X7f};

/**
 * @brief  将浮点数f转化为4个字节数据存放在byte[4]中
 * @param  data--待转换浮点数  
 * @param  byte--存放浮点数的数组
 * @retval None
 */
void Float_to_Byte(float statistic, uint8 *data_byte) {
    FloatLongType fl;
    fl.f_data = statistic;
    data_byte[0] = (unsigned char)fl.l_data;
    data_byte[1] = (unsigned char)(fl.l_data >> 8);
    data_byte[2] = (unsigned char)(fl.l_data >> 16);
    data_byte[3] = (unsigned char)(fl.l_data >> 24);
}

void VOFA_Send_float(float dat, uint8 *data_byte) {
    Float_to_Byte(dat, data_byte);
    for (length = 0; length < 4; length++) {
        // UART1_Send_Char(data_byte[length]); 
        // while(tx_busy);
        uart_putchar(UART_1, data_byte[length]);
        while(busy[1]);
    }
}

void VOFA_Send_Tail(void) {
    for (length = 0; length < 4; length++) {
        // UART1_Send_Char(tail[length]); //向串口1发送数据
        // while(tx_busy);
        uart_putchar(UART_1, tail[length]);
        while(busy[1]);
    }
}