//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:40:39
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-28 15:10:48
// @FilePath     : \STC16_V2\Project\USER\src\main.c
// @coding       : UTF-8
// @Description  :
//============================================================================//
#include "SWK_Headerfile.h"
#include "headfile.h"

/*
 *关于内核频率的设定，可以查看board.h文件
 *在board_init中,已经将P54引脚设置为复位
 *如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */

pid_param_t motor_pid;
pid_param_t servo_pid;
Model_Param servo_erro_pram;

// uint16 targetSpeed = 25;
uint8 Island_State = 0;      // 环岛状态
uint16 Island_Integrate = 0; // 环岛编码器积分

uint8 Branch_State = 0;         // 三叉状态
uint8 Branch_Compare_Count = 0; // 竖直连续增大，水平连续减小次数
int judge = 0;                  // 三叉特征值
uint16 Branch_Integrate = 0;    // 三叉编码器积分
uint8 Branch_Turn_Dire = 0;     // 三叉转向方向

uint16 speed = 0;      // 编码器计数
float servo_error = 0; // 车头偏离误差

//************************多车控制************************//
uint8 CH573_Rec_Command = 0x00; // 接收到的命令
uint16 distance = 0;            // 两车之间的距离
// uint8 Analyzed_Command;

//************************ADC采样************************//
uint16 N_ADC[6] = {0};        // 归一化数据
uint16 Last_N_ADC[6] = {0};   // 上次归一化数据
uint16 Inductor_ADC[6] = {0}; // 实际采样值
uint16 Min_ADC[6] = {120, 120, 110, 120, 120, 120};
uint16 Max_ADC[6] = {550, 1000, 1300, 1300, 1000, 550};

// motor_pid.target = 25;
// servo_pid.target = 750;

void main() {
    uint8 Engine_Power = 1;
    uint8 Mode = 0;
    // extern int speed_duty;
    motor_pid.target = 0;
    servo_pid.kp = 2.22;
    
    DisableGlobalIRQ(); //关闭总中断

    // sys_clk可选值:30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600。
    //设置系统频率，此频率需要跟STC-ISP软件中的 <输入用户程序运行的IRC频率>选项的频率一致。
    //如果频率设置不对，将会导致串口的数据不正常,PWM的工作不正常等等。
    sys_clk = 30000000; //设置系统频率为30MHz
    board_init();       //初始化寄存器

    // PID参数初始化
    PidInit(&motor_pid, 280, 20, 20, 20);
    PidInit(&servo_pid, 2.33, 0, 0.1, 0); // 750

    // 舵机偏差Model参数初始化
    Model_Param_init(&servo_erro_pram, 35, 45, 18, 0.35);

    // beep init
    gpio_mode(P3_4, GPO_PP);
    BEEP_OFF;

    // led init
    Led_Init(LED_ALL);
    // LED_Ctrl(LED1, ON);
    // LED_Ctrl(LED0, ON);

    // oled init
    oled_init();
    oled_fill(0x00);

    // 电机初始化
    pwm_init(MOTOR1_P, 150000, 0);
    pwm_init(MOTOR1_N, 150000, 0);

    // 舵机初始化
    pwm_init(PWMA_CH4N_P17, 50, 750);
    delay_ms(2000);

    // 编码器初始化
    ctimer_count_init(CTIM3_P04);

    // 串口初始化
    // uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 115200, TIM_2);
    uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 115200, TIM_2);
    uart_init(UART_4, UART4_RX_P52, UART4_TX_P53, 115200, TIM_2); // 接收距离

    // 定时器中断5ms
    pit_timer_ms(TIM_0, 5); //控制周期5ms

    // ADC初始化
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);
    adc_init(ADC_P10, ADC_SYSclk_DIV_2);
    adc_init(ADC_P03, ADC_SYSclk_DIV_2);
    adc_init(ADC_P02, ADC_SYSclk_DIV_2);
    adc_init(ADC_P01, ADC_SYSclk_DIV_2);
    adc_init(ADC_P00, ADC_SYSclk_DIV_2);

    //总中断最后开启
    EnableGlobalIRQ(); //开启总中断
    while (1) {
        MagneticField_Data_refresh();
        if ((CH573_Rec_Command & 0x3F) == Car_1_Out) {
            Engine_Power = 1;
        }

        if (1 == Engine_Power) {

            // Mode = Drive_Mode();
            Mode = 0;
            
            servo_error = Get_Err(N_ADC[Left_Midle], N_ADC[Left_Inside], N_ADC[Right_Midle], N_ADC[Right_Inside], servo_erro_pram);
            servo_error = (servo_error < -200) ? -200 : servo_error;

            // if (ENTER_BRANCH == Branch_State) {
            //     servo_error = servo_error / 3.5;
            // }
            servo_error = (ENTER_BRANCH == Branch_State) ? (servo_error / 3.5) : servo_error;

            switch (Mode) {
            case 0:
                if (-35 < servo_error && servo_error < 35 && IDLE_BRANCH == Branch_State && IDLE_ISLAND == Island_State) {
                    motor_pid.target = STRAIGHT_SPEED;
                    servo_pid.kp = 1.90;
                    servo_pid.kd = 0.001;
                }

                else if (EXECUTE_BRANCH == Branch_State || EXECUTE_ISLAND == Island_State) {
                    motor_pid.target = 20;
                }

                // 正常转向
                else {
                    motor_pid.target = 45 - 0.05 * fabs(servo_error);
                    motor_pid.target = (motor_pid.target < 25) ? 25 : motor_pid.target;
                    servo_pid.kp = 2.22;
                    servo_pid.kd = 0.01;
                }
                break;

            case 1:
                motor_pid.target = 25;
                if (EXECUTE_BRANCH == Branch_State || EXECUTE_ISLAND == Island_State) {
                    motor_pid.target = 20;
                }
                servo_pid.kp = 2.22;
                servo_pid.kd = 0.01;
                break;

            case 2:
                motor_pid.target = 25;
                if (EXECUTE_BRANCH == Branch_State || EXECUTE_ISLAND == Island_State) {
                    motor_pid.target = 20;
                }
                servo_pid.kp = 2.22;
                servo_pid.kd = 0.01;
                break;

            default:
                break;
            }

            if (distance < 500) {
                motor_pid.target = distance / 25;
            }
        }
        oled_int16(0, 0, N_ADC[Left_Midle]);
        oled_int16(0, 1, N_ADC[Left_Inside]);
        oled_int16(0, 2, N_ADC[Right_Inside]);
        oled_int16(0, 3, N_ADC[Right_Midle]);
        oled_int16(0, 4, servo_error);
        oled_uint16(0, 5, distance);
        oled_int16(0, 6, Branch_Compare_Count);
        oled_int16(0, 7, Mode);

        oled_int16(70, 0, Branch_State);
        oled_int16(70, 1, Island_State);
        // oled_int16(70, 2, Branch_Integrate);
        oled_int16(70, 3, (CH573_Rec_Command & 0x3F));
        oled_int16(70, 4, motor_pid.target);
        oled_int16(70, 4, motor_pid.target);

        //*********************** v2  *************************************//
        // if (1 == Engine_Power) {
        //     MagneticField_Data_refresh();
        //     servo_error = Get_Err(N_ADC[Left_Midle], N_ADC[Left_Inside], N_ADC[Right_Midle], N_ADC[Right_Inside], servo_erro_pram);

        //     if (ENTER_BRANCH == Branch_State) {
        //         servo_error = servo_error / 3.5;
        //     }

        //     // 1号车进入三叉减速且是第一圈
        //     if (Car_1_Enter_Branch == (CH573_Rec_Command & 0x3F) && IDLE_BRANCH == Branch_State && 0 == Branch_Turn_Dire) {
        //         motor_pid.target = 25;
        //     }
        //     // 2号车在三叉启动
        //     else if (Car_2_Start_Branch == (CH573_Rec_Command & 0x3F)) {
        //         BEEP_ON;
        //         motor_pid.target = (motor_pid.target < 35) ? (motor_pid.target += 0.1) : 35;
        //         if (motor_pid.target >= 34.5) {
        //             // CH573_Rec_Command = 0x00;
        //             BEEP_OFF;
        //         }
        //     }
        //     // 强制转向进入三叉或环岛减速
        //     else if (EXECUTE_BRANCH == Branch_State || EXECUTE_ISLAND == Island_State) {
        //         motor_pid.target = 20;
        //     }
        //     // 进入三叉且2号车还没发车继续保持低速且是第一圈
        //     else if (ENTER_BRANCH == Branch_State && Car_2_Start_Branch != (CH573_Rec_Command & 0x3F) && 0 == Branch_Turn_Dire) {
        //         motor_pid.target = 25;
        //     }
        //     // 直路行驶
        //     else if (-35 < servo_error && servo_error < 35 && IDLE_BRANCH == Branch_State) {
        //         motor_pid.target = STRAIGHT_SPEED;
        //         servo_pid.kp = 1.90;
        //         servo_pid.kd = 0.001;
        //     }
        //     // 正常转向
        //     else {
        //         motor_pid.target = 45 - 0.04 * fabs(servo_error);
        //         motor_pid.target = (motor_pid.target < 25) ? 25 : motor_pid.target;
        //         servo_pid.kp = 2.22;
        //         servo_pid.kd = 0.01;
        //     }

        //     if (distance < 500) {
        //         motor_pid.target = distance / 25;
        //     }
        // }

        //*********************** v1  *************************************//
        // if (1 == Engine_Power) {
        //     MagneticField_Data_refresh();
        //     servo_error = Get_Err(N_ADC[Left_Midle], N_ADC[Left_Inside], N_ADC[Right_Midle], N_ADC// // [Right_Inside], servo_erro_pram);

        //     // // 环岛判断
        //     // if (0 == Branch_State) {
        //     //     Island_Judge();
        //     // }
        //     // // 三叉判断
        //     // if (0 == Island_State) {
        //     //     Branch_Road_Judge();
        //     // }
        //     // 三叉误差缩小3倍
        //     if (ENTER_BRANCH == Branch_State) {
        //         servo_error = servo_error / 3.5;
        //     }
        //     // 头车进入三叉
        //     if (Car_1_Enter_Branch == (CH573_Rec_Command & 0x3F)) {
        //         motor_pid.target = 25;
        //     }
        //     // 2号车在三叉启动
        //     else if (Car_2_Start_Branch == (CH573_Rec_Command & 0x3F)) {
        //         BEEP_ON;
        //         motor_pid.target = (motor_pid.target < 35) ? (motor_pid.target += 0.1) : 35;
        //         if (motor_pid.target >= 34.5) {
        //             CH573_Rec_Command = 0x00;
        //             BEEP_OFF;
        //         }
        //     }
        //     // 直路行驶,没有进入三叉，头车没进入三叉
        //     else if (-35 < servo_error && servo_error < 35 && ENTER_BRANCH != Branch_State && Car_1_Enter_Branch != (CH573_Rec_Command & 0x3F)) {
        //         motor_pid.target = STRAIGHT_SPEED;
        //         servo_pid.kp = 1.9;
        //         // servo_pid.kp = 1.8 + 0.001 * fabs(servo_error);
        //         servo_pid.kd = 0.001;
        //     }
        //     // 进入三叉减速
        //     else if (EXECUTE_BRANCH == Branch_State || distance < 50 || EXECUTE_ISLAND == Island_State || Car_1_Enter_Branch == (CH573_Rec_Command & 0x3F)) {
        //         motor_pid.target = 20;

        //     }
        //     else if (ENTER_BRANCH == Branch_State) {
        //         motor_pid.target = (motor_pid.target < 30) ? (motor_pid.target += 0.2) : 30;
        //         servo_pid.kp = 2.1;
        //         servo_pid.kd = 0.02;
        //     }
        //     // else if (Car_1_GoOut_Branch == (CH573_Rec_Command & 0x3F)) {
        //     //     // BEEP_OFF;
        //     //     CH573_Rec_Command = 0x00;
        //     // }
        //     // 其他情况，转向
        //     else {
        //         motor_pid.target = 40- 0.018*fabs(servo_error);
        // 		motor_pid.target = (motor_pid.target < 16) ? 16 : motor_pid.target;
        //         servo_pid.kp = 2.1;
        //         servo_pid.kd = 0.02;
        //     }

        //     if (distance < 500) {
        //         motor_pid.target = distance / 20;
        //     }
        // }

       

        // Led_count++;
        // if (Led_count > 250) {
        //     LED_Ctrl(LED1, RVS);
        // }
    }
}
