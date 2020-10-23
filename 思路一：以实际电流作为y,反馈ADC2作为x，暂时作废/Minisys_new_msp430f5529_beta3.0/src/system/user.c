/*
 * user.c
 *
 *  Created on: 2018-02-16 18:00
 *      Author: Jack Chen <redchenjs@live.com>
 *  Fixed on :  2018-05-25 11:14
 *      Author: art-j <jjw903427521@gmail.com>
 */
#include <msp430.h>
#include <inc/user/input.h>
#include <inc/user/measure.h>
#include <inc/user/control.h>
#include <inc/user/display.h>
#include <inc/driver/ssd1351.h>
#include <inc/driver/ads1118.h>
#include <inc/driver/dac8571.h>
#include <stdio.h>
#include <inc/system/fonts.h> //使用颜色的名称必须包含fonts.h的头文件，比如要使用White，Red....等颜色名称
#include <src/user/display_value.h>
#include <inc/system/user.h>
char jiantou = 1, jiemian = 1, confirm = 0;
char hold = 0;
/*
char *my_info[]={"Name:JHL","Num:Xb17610107","Date:2020.7.14","Class:17 EIE1"};
char *buttom[] = {"KEY1","KEY2","KEY3","KEY4"};//指针数组，每个元素指针，可以指向数组中的字符串
*/
void display_myinfo()
{
    ssd1351_clear_gram();
    ssd1351_display_string(0, 0, "Name:JHL", 1, White, Black);
    ssd1351_display_string(0, 20, "Num:Xb17610107", 1, White, Black);
    ssd1351_display_string(0, 40, "Date:2020.7.14", 1, White, Black);
    ssd1351_display_string(0, 60, "Class:17 EIE1", 1, White, Black);
}

void mode_choose_interface()
{
    ssd1351_display_string(0, 0, "> CC Mode", 1, White, Black);
    ssd1351_display_string(0, 20, "  CV Mode", 1, White, Black);
    ssd1351_display_string(0, 40, "  CR Mode", 1, White, Black);
    ssd1351_display_string(0, 60, "  Developer Mode", 1, White, Black);
}

void user_init(void)
{
    display_myinfo();
    __delay_cycles(10000000);
    ssd1351_clear_gram();
}

void user_loop(void)
{
    display_value(); //持续显示采样电压值
    /*    input_update();
    measure_update();
    control_update();
    display_update();*/
    //ssd1351_draw_64k_bitmap(0,0,gImage_123,100,100);
}

void P2_IODect() //检测事件确实发生了
{
    static unsigned char now = 0, i = 0; //position用来保存选择指针的位置,jiemian用来保存跳转的界面级数,confirm用来记录KEY3的按键次数,hold用来检测是否启用长按函数
    unsigned char past = 0, key = 0;     //key来区别那个按键按下,i用来记录长按的时间
    past = now;
    if ((P2IN & BIT0) == 0)
    {
        key = 1;
        now = 0;
    }
    else if ((P2IN & BIT2) == 0)
    {
        key = 2;
        now = 0;
    }
    else if ((P2IN & BIT4) == 0)
    {
        key = 3;
        now = 0;
    }
    else if ((P2IN & BIT5) == 0)
    {
        key = 4;
        now = 0;
    }
    else
        now = 1;
    if ((past == 1) && (now == 0))
    {

        switch (key)
        {
        case 1:
            if (jiemian == 1) //处于模式选择界面时，KEY1的功能
            {
                if (jiantou == 1) //箭头1按下KEY1
                {
                    jiantou = 4; //跳转到箭头4
                    ssd1351_display_char(0, 0, ' ', 1, White, Black);
                    ssd1351_display_string(0, 60, ">>", 1, White, Black);
                }
                else if (jiantou == 2) //箭头2按下KEY1
                {
                    jiantou = 1;
                    ssd1351_display_char(0, 20, ' ', 1, White, Black);
                    ssd1351_display_char(0, 0, '>', 1, White, Black);
                }
                else if (jiantou == 3) //箭头3按下KEY1
                {
                    jiantou = 2;
                    ssd1351_display_char(0, 40, ' ', 1, White, Black);
                    ssd1351_display_char(0, 20, '>', 1, White, Black);
                }
                else //箭头4按下KEY1
                {
                    jiantou = 3;
                    ssd1351_display_string(0, 60, "  ", 1, White, Black);
                    ssd1351_display_char(0, 40, '>', 1, White, Black);
                }
            }
            else //处于电量参数调节界面时，KEY1的功能
            {
                confirm = 1; //KEY1在界面2被按下，KEY3按键次数被初始化位1
                switch (jiantou)
                {
                case 1:
                    CC_value += 0.01;
                    if (CC_value > 1)
                        CC_value = 1;
                    break;
                case 2:
                    CV_value += 0.02;
                    if (CV_value > 20)
                        CV_value = 20;
                    break;
                case 3:
                    CR_value += 0.1;
                    if (CR_value > 20)
                        CR_value = 20;
                    break;
                case 4:
                    CV_value += 0.02;
                    if (CV_value > 20)
                        CV_value = 20;
                    break;
                default:
                    break;
                }
            }
            break;
        case 2:
            if (jiemian == 1)
            {
                if (jiantou == 1) //箭头1按下KEY2
                {
                    jiantou = 2;
                    ssd1351_display_char(0, 0, ' ', 1, White, Black);
                    ssd1351_display_char(0, 20, '>', 1, White, Black);
                }
                else if (jiantou == 2) //箭头2按下KEY2
                {
                    jiantou = 3;
                    ssd1351_display_char(0, 20, ' ', 1, White, Black);
                    ssd1351_display_char(0, 40, '>', 1, White, Black);
                }
                else if (jiantou == 3) //箭头3按下KEY2
                {
                    jiantou = 4;
                    ssd1351_display_char(0, 40, ' ', 1, White, Black);
                    ssd1351_display_string(0, 60, ">>", 1, White, Black);
                }
                else //箭头4按下KEY2
                {
                    jiantou = 1;
                    ssd1351_display_string(0, 60, "  ", 1, White, Black);
                    ssd1351_display_char(0, 0, '>', 1, White, Black);
                }
            }
            else
            {
                confirm = 1; //KEY2在界面2被按下，KEY3按键次数被初始化位1
                switch (jiantou)
                {
                case 1:
                    CC_value -= 0.02;
                    if (CC_value < 0.1)
                        CC_value = 0.1;
                    break;
                case 2:
                    CV_value -= 0.02;
                    if (CV_value <= 0)
                        CV_value = 0;
                    break;
                case 3:
                    CR_value -= 0.1;
                    if (CR_value > 20)
                        CR_value = 20;
                    break;
                case 4:
                    CV_value -= 0.02;
                    if (CV_value <= 0)
                        CV_value = 0;
                    break;
                default:
                    break;
                }
            }
            break;
        case 3:
            if (confirm == 0)
            {
                ssd1351_clear_gram();
                confirm = 1;
                jiemian = 2;
            }
            else if(confirm==1)
            {
                confirm = 2; //同时起到参数设置完成的标志位和KEY3的按键次数记录
                if(jiantou==1)
                {
                DAC8571_OUT(CC_value);
                CC_voltage = CC_value;
                }
            }
            else;
            break;
        case 4:
        {
            back = 0;
            jiemian = 1;
            confirm = 0;
            jiantou = 1;
            ssd1351_clear_gram();
        }
        break;
        default:
            break;
        }
    }
    else if ((past == 0) && (now == 0))
    {
        i++;
        if (i == 45)
            hold = 1;
        if (hold == 1)
        {
            switch (key)
            {
            case 1:
                if (jiemian == 2) //处于电量参数调节界面时，KEY1的功能
                {
                    confirm = 1; //KEY1在界面2被按下，KEY3按键次数被初始化位1
                    switch (jiantou)
                    {
                    case 1:
                        CC_value += 0.02;
                        if (CC_value > 1)
                            CC_value = 1;
                        break;
                    case 2:
                        CV_value += 0.02;
                        if (CV_value > 20)
                            CV_value = 20;
                        break;
                    case 3:
                        CR_value += 0.1;
                        if (CR_value > 20)
                            CR_value = 20;
                        break;
                    case 4:
                        CV_value += 0.02;
                        if (CV_value > 20)
                            CV_value = 20;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 2:
                if (jiemian == 2)
                {
                    confirm = 1; //KEY2在界面2被按下，KEY3按键次数被初始化位1
                    switch (jiantou)
                    {
                    case 1:
                        CC_value -= 0.02;

                        if (CC_value < 0.1)
                            CC_value = 0.1;
                        break;
                    case 2:
                        CV_value -= 0.02;

                        if (CV_value <= 0)
                            CV_value = 0;
                        break;
                    case 3:
                        CR_value -= 0.1;

                        if (CR_value > 20)
                            CR_value = 20;
                        break;
                    case 4:
                        CV_value -= 0.02;

                        if (CV_value <= 0)
                            CV_value = 0;
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        i = 0;    //剩下的两种情况不论是1、按键弹起还是2、按键一直没按，长按时间计数变量复位
        hold = 0; //长按标志变量复位
    }
}
