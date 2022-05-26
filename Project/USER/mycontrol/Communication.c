//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 20:30:39
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-26 14:53:37
// @FilePath     : \STC16_V2\Project\USER\mycontrol\Communication.c
// @coding       : UTF-8
// @Description  : 多车通信
//============================================================================//

#include "Communication.h"

// uint8 CH573_Rec_Command; // 接收到的命令
// uint16 distance = 0;         // 两车之间的距离

void CH573_Send_Commands(uint8 Command) { uart_putchar(UART_4, Command); }

// void CH573_Receive_Commands(uint8 Command) {
//     busy[4] = 1;
//     CH573_Rec_Command = Command;
//     busy[4] = 0;
//     while (busy[4]);
// }

void Analysis_CH573_Receive_Commands(void) {
    uint8 Command_Object; // 接收命令对象

    Command_Object = (CH573_Rec_Command & 0xC0) >> 6;

    // 接收广播命令或发送给尾车的命令
    if (0x00 == Command_Object || 0x03 == Command_Object) {
        CH573_Rec_Command = (CH573_Rec_Command & 0x3F);
    } else {
        CH573_Rec_Command = 0x00;
    }

}

//该函数在isr.c文件中的UART3_Isr函数进行回调
void Split_Ultrasonic_callback(uint8 buf) {
    static uint8 dat[3];
    static uint8 num = 0;
    busy[3] = 1;
    dat[num] = buf;
    
    if (dat[0] != 0xa5)
        num = 0; //检查头帧是否正确，不正确就重新接收
    else
        num++;
    //接收完成，开始处理数据
    if (num == 3) {
        num = 0;
        //将数据的高八位与低八位进行组合，得到距离信息单位MM
        distance = dat[1] << 8 | dat[2];
        
    }
    busy[3] = 0;
    while (busy[3]);
}