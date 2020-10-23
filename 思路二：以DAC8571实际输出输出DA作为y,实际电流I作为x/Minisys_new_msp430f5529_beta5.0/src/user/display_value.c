/*
 * display_value.c
 *
 *  Created on: 2020年7月17日
 *      Author: jianghanlin
 */
#include "msp430f5529.h"
#include <inc/user/input.h>
#include <inc/driver/ssd1351.h>
#include <inc/driver/ads1118.h>
#include <inc/driver/dac8571.h>
#include <stdio.h>
#include <inc/system/user.h>
#include <inc/system/fonts.h>//使用颜色的名称必须包含fonts.h的头文件，比如要使用White，Red....等颜色名称
float CC_value=0,CC_voltage=0,CV_value=0,CR_value=100,CR_voltage=0,CR_current=0;
unsigned char back=0;//变量back用来判断界面1的次数
char dianliu[10]={},dianya[10]={},dianzu[10]={};




void display_value()
{
    if((jiemian==2)&&(confirm==1))//KEY3被第1次按下，并且进入2级界面
    {
        switch (jiantou)//根据1级界面的箭头位置，决定显示的内容
        {
        case 1:
            sprintf(dianliu,"%.3f",CC_value);//显示用户设定的电流值
            ssd1351_display_string(0, 0, "Set CC Value:", 1, White, Black);
            ssd1351_display_string(0, 20, "I=", 1, White, Black);
            ssd1351_display_string(17,20,dianliu,1,Red,Black);
            ssd1351_display_string(0, 80, "ADC1=", 0, White, Black);//显示反馈电压ADC1
            ssd1351_display_string(78, 80, "V=", 0, White, Black);//显示反馈电压ADC1
            ssd1351_display_string(0, 92, "ADC2=", 0, White, Black); //显示反馈电压ADC2
            ssd1351_display_string(0, 104, "I=", 1, White, Black);   //显示实际的电流值
            break;
        case 2:
            sprintf(dianzu,"%.0f  ",CR_value);//显示用户设定的电阻值
            ssd1351_display_string(0, 0, "Set CR Value:", 1, White, Black);
            ssd1351_display_string(0, 20, "R=", 1, White, Black);
            ssd1351_display_string(17,20,dianzu,1,Red,Black);
            ssd1351_display_string(0,80,"Load Voltage=",0,White,Black);
            ssd1351_display_string(0, 92, "I=", 0, White, Black);   //显示实际的电流值
            ssd1351_display_string(0, 104, "Actual Res=", 0, White, Black);   //显示实际的电流值
            break;
        case 3:
            sprintf(dianya,"%.3f",CV_value);//开发者界面调节的是电压，显示用户调节的电压
            ssd1351_display_string(0, 0, "[[Set DA Value]]", 1, White, Black);
            ssd1351_display_string(0, 20, "DA=", 1, White, Black);
            ssd1351_display_string(25,20,dianya,1,Red,Black);
            ssd1351_display_string(0, 80, "ADC2=", 1, White, Black);
            break;
        default:
            break;
        }
        ssd1351_draw_h_line(0,38,127,White);//第一条分割线
        ssd1351_display_string(0,40,"DAC8571 Output:",0,White,Black);
        ssd1351_draw_h_line(0,65,127,White);//第二条分割线
        ssd1351_display_string(0,67,"Monitor Window:",0,White,Black);
        get_ads1118_sample_voltage();//显示4通道采样得到的模拟电压
    }
    else if((jiemian==2)&&(confirm==2))//参数确定按下一瞬间
    {
        switch (jiantou)
        {
        case 1:
            sprintf(dianliu,"%.3f",CC_value);//显示用户按下KEY3后确定的电流值，锁死，颜色变为蓝色
            ssd1351_display_string(78,20,dianliu,1, DeepSkyBlue,Black);
            cc_mode_adc2_feedback(a2);//对ADC2的电压进行闭环，通过实际电流值与用户设定电流值进行比较，最终进行CC_voltage的增减
            DAC8571_OUT(CC_voltage);//将调节的电压进行输出
            break;
        case 2:
            sprintf(dianzu,"%.0f  ",CR_value);
            ssd1351_display_string(80,20,dianzu,1, DeepSkyBlue,Black);
            cr_mode_adc2_feedback(a2);
            DAC8571_OUT(CR_voltage);//将调节的电压进行输出
            //DAC8571_OUT(CV_value);//设置DAC8751的输出电压
            break;
        case 3:
            sprintf(dianya,"%.3f",CV_value);
            ssd1351_display_string(80,20,dianya,1, DeepSkyBlue,Black);
            //DAC8571_OUT(CV_value);//设置DAC8751的输出电压
            break;
        default:
            break;
        }
        get_ads1118_sample_voltage();//显示4通道采样得到的模拟电压
    }
    else if((jiemian==1)&&(confirm==0))
    {
        if(back==0)
        {
            ssd1351_clear_gram();
            mode_choose_interface();
            back=1;
        }
    }
    else;
}




