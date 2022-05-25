//============================================================================//
// @Author       : 孙雾崆 1489389972@qq.com
// @Date         : 2022-05-24 23:41:36
// @LastEditors  : 孙雾崆 1489389972@qq.com
// @LastEditTime : 2022-05-25 00:21:38
// @FilePath     : \STC16_V2\Project\USER\mycontrol\PID.c
// @coding       : UTF-8
// @Description  : 
//============================================================================//
#include "PID.h"

/*************************************************************************
*  函数名称：float constrain_float(float amt, float low, float high)
*  功能说明：限幅函数
*  参数说明：
* @param    pid_out   ： pid计算输出
* @param    low       ： 最低值
* @param    high      ： 最高值
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：
*************************************************************************/
float constrain_float(float pid_out, float low, float high)
{
	return ((pid_out)<(low)?(low):((pid_out)>(high)?(high):(pid_out)));
}

// pid参数初始化函数

void PidInit(pid_param_t * pid, float kp, float ki, float kd, float target)
{
	pid->kp        = kp;
	pid->ki        = ki;
	pid->kd        = kd;
    pid->target    = target;
	pid->out_p     = 0;
	pid->out_i     = 0;
	pid->out_d     = 0;
	pid->out       = 0;
	// pid->outmax    = outmax;
	pid->integrator= 0;
	pid->last_error= 0;
	pid->last_derivative   = 0;
	pid->last_t    = 0;
}



/*************************************************************************
*  函数名称：float PidLocCtrl(pid_param_t * pid, float error)
*  功能说明：pid位置式控制器输出
*  参数说明：
* @param    pid     pid参数
* @param    error   pid输入误差
*  函数返回：PID输出结果
*  修改时间：2020年4月1日
*  备    注：
*************************************************************************/
float PidLocCtrl(pid_param_t * pid, float error)
{
  /* 累积误差 */
  pid->integrator += error;
  
  /* 误差限幅 */
  pid->integrator=constrain_float(pid->integrator, -pid->imax, pid->imax);
  
  
  pid->out_p = pid->kp * error;
  

  pid->out_i = pid->ki * pid->integrator;
  pid->out_d = pid->kd * (error - pid->last_error);
  
  pid->last_error = error;
  
  pid->out = pid->out_p + pid->out_i + pid->out_d;
  /* 输出限幅 */
//   pid->out = constrain_float(pid->out,-pid->outmax,pid->outmax);
  // oled_printf_float(70,  0, pid->out, 5, 2);
  
  return pid->out;
}
/*************************************************************************
*  函数名称：float constrain_float(float amt, float low, float high)
*  功能说明：pid增量式控制器输出
*  参数说明：
* @param    pid     pid参数
* @param    error   pid输入误差
*  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
*  修改时间：2020年4月1日
*  备    注：
*************************************************************************/
float PidIncCtrl(pid_param_t * pid, float error)
{
  
  pid->out_p = pid->kp * (error - pid->last_error);
  pid->out_i = pid->ki * error;
  pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);
  
  pid->last_derivative = error - pid->last_error;
  pid->last_error = error;
  
  pid->out += pid->out_p + pid->out_i + pid->out_d;
  
  return pid->out;
}

