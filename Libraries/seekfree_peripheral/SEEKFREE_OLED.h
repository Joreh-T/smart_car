/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		OLED
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					���߶��壺
					------------------------------------ 
					    OLEDҺ��    ��Ƭ��                        
    					D0          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SCL_PIN   �궨��
    					D1          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SDA_PIN   �궨��            
    					RES         �鿴SEEKFREE_OLED�ļ��ڵ�OLED_RST_PIN   �궨��    
    					DC          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_DC_PIN    �궨��
						CS          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_CS_PIN    �궨��
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _SEEKFREE_OELD_h
#define _SEEKFREE_OELD_h

#include "common.h"
#include "board.h"


//--------------------����SPI--------------------


#define OLED_SCL_PIN  P64  // P25		
#define OLED_SDA_PIN  P63  // P23
#define OLED_REST_PIN P62  // P20		//液晶复位引脚定义
#define OLED_DC_PIN   P61  // P21   	//液晶命令位引脚定义
#define OLED_CS_PIN   P60  // P22   	//定义SPI_CS引脚


#define OLED_SCL(x)  OLED_SCL_PIN = x
#define OLED_SDA(x)  OLED_SDA_PIN = x
#define OLED_RST(x)  OLED_REST_PIN = x
#define OLED_DC(x)   OLED_DC_PIN = x
#define OLED_CS(x)   OLED_CS_PIN = x

//--------------------Ӳ��SPI--------------------


#define OLED_SPI_N 			SPI_CH2
#define OLED_SPI_SCLK_PIN   SPI_CH2_SCLK_P25
#define OLED_SPI_MOSI_PIN   SPI_CH2_MOSI_P23
#define OLED_SPI_MISO_PIN   SPI_CH2_MISO_P24	//����SPI_MISO����  OLED��Ļû��MISO���ţ�����������Ȼ��Ҫ���壬��spi�ĳ�ʼ��ʱ��Ҫʹ��
#define OLED_SPI_REST_PIN 	P20				//Һ����λ���Ŷ���
#define OLED_SPI_DC_PIN   	P21   			//Һ������λ���Ŷ���
#define OLED_SPI_CS_PIN     P22				//����Ӳ��SPI_CS����



#define OLED_SPI_RST(x)   	OLED_SPI_REST_PIN = x
#define OLED_SPI_DC(x)    	OLED_SPI_DC_PIN = x
#define OLED_SPI_CS(x) 		OLED_SPI_CS_PIN = x          

//-----------------------------------------------    

//������ʾ����
//0 ����ģʽ
//1 ����ģʽ  ��ת180
#define OLED_DISPLAY_DIR 0

#if (0==OLED_DISPLAY_DIR || 1==OLED_DISPLAY_DIR)
#define X_WIDTH         128
#define Y_WIDTH         64

#else
#error "OLED_DISPLAY_DIR �������"
     
#endif
                            
#define	Brightness	    0x7f    //����OLED����  Խ��Խ��    ��Χ0-0XFF                            
#define XLevelL		    0x00
#define XLevelH		    0x10
#define XLevel		    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	    128
#define Max_Row		    64





//--------------------����SPI--------------------
void oled_init(void);     
void oled_fill(uint8 dat);
void oled_set_pos(uint8 x, uint8 y);
void oled_putpixel(uint8 x,uint8 y,uint8 data1);
void oled_clrpixel(uint8 x,uint8 y);
void oled_p6x8str(uint8 x,uint8 y,const int8 ch[]);
void oled_p8x16str(uint8 x,uint8 y,const int8 ch[]);
void oled_uint16(uint8 x, uint8 y, uint16 num);
void oled_int16(uint8 x, uint8 y, int16 num);
void oled_printf_int32(uint16 x,uint16 y,int32 dat,uint8 num);
void oled_printf_float(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum);
void oled_dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);
void oled_print_chinese(uint8 x, uint8 y, uint8 size, const uint8 *p, uint8 len);

//--------------------Ӳ��SPI--------------------
void oled_init_spi(void);     
void oled_fill_spi(uint8 dat);
void oled_set_pos_spi(uint8 x, uint8 y);
void oled_putpixel_spi(uint8 x,uint8 y,uint8 data1);
void oled_clrpixel_spi(uint8 x,uint8 y);
void oled_p6x8str_spi(uint8 x,uint8 y,const int8 ch[]);
void oled_p8x16str_spi(uint8 x,uint8 y,const int8 ch[]);
void oled_uint16_spi(uint8 x, uint8 y, uint16 num);
void oled_int16_spi(uint8 x, uint8 y, int16 num);
void oled_printf_int32_spi(uint16 x,uint16 y,int32 dat,uint8 num);
void oled_printf_float_spi(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum);
void oled_dis_bmp_spi(uint16 high, uint16 width, uint8 *p,uint8 value);
void oled_print_chinese_spi(uint8 x, uint8 y, uint8 size, const uint8 *p, uint8 len);
/********************************************************************/

#endif
