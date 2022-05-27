#include "headfile.h"
#include "SWK_Headerfile.h"

//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF

	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF
        busy[3] = 0;
        Split_Ultrasonic_callback(S3BUF);

	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;

		//接收数据寄存器为：S4BUF;
        CH573_Rec_Command = S4BUF;
        // Analysis_CH573_Receive_Commands();
        
	}
}

// // #define LED P52
// void INT0_Isr() interrupt 0
// {
// 	// LED = 0;	//点亮LED
// }
// void INT1_Isr() interrupt 2
// {

// }
// void INT2_Isr() interrupt 10
// {
// 	INT2_CLEAR_FLAG;  //清除中断标志
// }
// void INT3_Isr() interrupt 11
// {
// 	INT3_CLEAR_FLAG;  //清除中断标志
// }

// void INT4_Isr() interrupt 16
// {
// 	INT4_CLEAR_FLAG;  //清除中断标志
// }

extern pid_param_t motor_pid;
extern pid_param_t servo_pid;

void TM0_Isr() interrupt 1 {
    static uint16 State_Reset_Count = 0;
    int speed_duty;          // 电机占空比
    uint16 servo_duty;       // 舵机占空比

    // MagneticField_Data_refresh(); // 刷新数据
    // servo_error = Get_Err(N_ADC[Left_Midle], N_ADC[Left_Inside], N_ADC[Right_Midle], N_ADC[Right_Inside], servo_erro_pram);
	
    speed = ctimer_count_read(CTIM3_P04); // 读取编码器计数

    if (0 == Branch_State) {
        Island_Judge();
    }
    if (0 == Island_State) {
        // Branch_Road_Judge();
        Branch_Road_Judge_2();
    }
    
    // 超时复位
    if (EXIT_ISLAND == Island_State || ENTER_BRANCH == Branch_State) {
        State_Reset_Count = 0;
    }
    if (IDLE_ISLAND != Branch_State || IDLE_BRANCH != Island_State) {
        // LED_Ctrl(LED1, ON);
        
        State_Reset_Count += 1;
        if (State_Reset_Count > 400) {
            Island_State = ( Island_State > 1) ? Island_State : 0;
        }
        if (State_Reset_Count > 2500) {
            // CH573_Rec_Command = 0x00;
            Branch_State = (0 == Branch_State) ? Branch_State : 0;
            Island_State = (0 == Island_State) ? Island_State : 0;
            State_Reset_Count = 0;
        }
    } else {
        // LED_Ctrl(LED1, OFF);
    }
    
    ctimer_count_clean(CTIM3_P04); // 清空编码器计数

    // if (2 == Branch_State) {
    //     servo_error  = servo_error/3;
    // }

    // if (-30 < servo_error && servo_error < 30) {
    //     motor_pid.target  = 40;
    //     servo_pid.kp = 1.8 + 0.001 * fabs(servo_error);
    //     servo_pid.kd = servo_pid.kp / 50;
    // } else if (1 == Branch_State) {
    //     motor_pid.target  = 15;
    //     servo_pid.kp = 2.33 + 0.001 * (servo_error);
    //     servo_pid.kd = servo_pid.kp / 100;
    // } else if (3 == Island_State) {
    //     motor_pid.target  = 25;
    // } else {
    //     motor_pid.target  = 30;
    //     servo_pid.kp = 2.33 + 0.001 * fabs(servo_error);
    //     servo_pid.kd = servo_pid.kp / 100;
    // }
    
    /**************速度PID****************/
    // speed_error = motor_pid.target - speed;
    speed_duty  = PidLocCtrl(&motor_pid, motor_pid.target - speed);
    if (N_ADC[Left_Midle] < 3 && N_ADC[Right_Midle] < 3 && N_ADC[Left_Inside] < 3 && N_ADC[Right_Inside] < 3) {
        speed_duty = 0;
    }
    motor_duty(speed_duty);

    /**************舵机PID***************/
    if (2 != Branch_State && 3 != Island_State && 5 != Island_State) {
        servo_duty = PidLocCtrl(&servo_pid, servo_error) + 750;
        servo_duty = (servo_duty > 970) ? 970 : ((servo_duty < 560) ? 560 : servo_duty);
        pwm_duty(PWMA_CH4N_P17, servo_duty);
    }


}
// void TM1_Isr() interrupt 3
// {

// }
// void TM2_Isr() interrupt 12
// {
// 	TIM2_CLEAR_FLAG;  //清除中断标志
	
// }
// void TM3_Isr() interrupt 19
// {
// 	TIM3_CLEAR_FLAG; //清除中断标志
	
// }

// void TM4_Isr() interrupt 20
// {
// 	TIM4_CLEAR_FLAG; //清除中断标志
// //	ccd_collect();	 //CCD采集数据

// }

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;