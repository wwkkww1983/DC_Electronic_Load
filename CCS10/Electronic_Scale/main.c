#include "msp430g2553.h"
#include "LCD_128.h"
#include "HT1621.h"
#include "TCA6416A.h"

int zhenshu = 0, xiaoshu = 0;
unsigned char key = 0;   //去皮按键标志位
unsigned int count = 1; //去皮数据保护位
void ADC10_init(void);
void LCD_Init();
void LCD_Display();
void WDT_init();
void I2C_IODect();
void display_qupi();
void display_my_info();

int main()
{
    WDTCTL = WDTPW + WDTHOLD; //关看门狗
    float Vin = 0, sum = 0,mv=0,g=0;
    int qupi = 0;
    int i = 0;

    ADC10_init();
    LCD_Init();
    WDT_init();
    display_my_info();
    _EINT();
    while (1)
    {
        ADC10CTL0 |= ENC + ADC10SC;     //在主循环中开启 ADC10 转换
        LPM0; //休眠开总中断，等待 ADC10 转换完成后，进入中断运行唤醒 CPU 代码
        //-----ADC转换完成中断唤醒CPU后才执行以下代码-----
        Vin = (ADC10MEM * 2.5) / 1023;
        sum = Vin + sum;
        i++;
        if (i == 170)
        {
            Vin = sum / 170.0;
            mv = Vin * 1000;
            if (key == 1) //去皮功能语句块
            {
                if (count == 1)
                {
                    display_qupi();
                    __delay_cycles(10000);
                    qupi = 0.00006*mv*mv+3.223*mv-2044.7; //记录产品包装的质量
                    count = 0;  //去皮质量第一次记录后就保护起来的标志
                }
                g = 0.00006*mv*mv+3.223*mv-2044.7-qupi;
            }
            else
                g = 0.00006*mv*mv+3.223*mv-2044.7;

            zhenshu = (int)g;
            xiaoshu = (g - zhenshu) * 10;
            LCD_Display();
            i = 0;
            sum = 0;
        }
    }
}

void display_my_info()
{
    LCD_DisplaySeg(4);
    LCD_DisplaySeg(9);
    LCD_DisplaySeg(10); //显示J
    LCD_DisplayDigit(8, 2);
    LCD_ClearSeg(12);
    LCD_ClearSeg(19); //显示H
    LCD_DisplayDigit(0, 3);
    LCD_ClearSeg(25);
    LCD_ClearSeg(26);
    LCD_ClearSeg(27); //显示L
    LCD_DisplayDigit(1, 4);
    LCD_DisplayDigit(0, 5);
    LCD_DisplayDigit(7, 6);
    HT1621_Reflash(LCD_Buffer);
    __delay_cycles(1000000);
    LCD_Clear();
    LCD_DisplayDigit(0, 5);
    LCD_DisplayDigit(0, 6);
    LCD_DisplaySeg(_LCD_DOT4);
    HT1621_Reflash(LCD_Buffer);
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    PinIN();
    I2C_IODect();
    LPM0_EXIT;
}

void I2C_IODect() //检测事件确实发生了
{
    static unsigned char keyEY_Past = 0, keyEY_Now = 0;
    keyEY_Past = keyEY_Now;
    //----判断I2C_IO10所连的keyEY1按键是否被按下------
    if ((TCA6416A_InputBuffer & BIT8) == BIT8)
        keyEY_Now |= BIT0;
    else
        keyEY_Now &= ~BIT0;
    if (((keyEY_Past & BIT0) == BIT0) && (keyEY_Now & BIT0) != BIT0)
    {
        PinOUT(0, 0); //LED1亮，LED234灭
        PinOUT(1, 0);
        PinOUT(2, 0);
        PinOUT(3, 0);
        PinOUT(4, 0);
        PinOUT(5, 0);
        PinOUT(6, 0);
        PinOUT(7, 0);
        PinOUT(8, 0);
        key = 1;
    }
    //----判断I2C_IO12所连的keyEY3按键是否被按下------
    if ((TCA6416A_InputBuffer & BITA) == BITA)
        keyEY_Now |= BIT2;
    else
        keyEY_Now &= ~BIT2;
    if (((keyEY_Past & BIT2) == BIT2) && (keyEY_Now & BIT2) == 0)
    {
        PinOUT(0, 1);
        PinOUT(1, 1);
        PinOUT(2, 1);
        PinOUT(3, 1);
        PinOUT(4, 1);
        PinOUT(5, 1);
        PinOUT(6, 1);
        PinOUT(7, 1);
        PinOUT(8, 1);
        count = 1;
        key = 3;
    }
}

void display_qupi()
{
    LCD_Clear();
    LCD_DisplayDigit(9, 3);
    LCD_ClearSeg(20);
    LCD_DisplayDigit(0, 4);
    LCD_ClearSeg(55);
    LCD_DisplayDigit(8, 5);
    LCD_ClearSeg(56);
    LCD_ClearSeg(61);
    LCD_DisplayDigit(1, 6);
    HT1621_Reflash(LCD_Buffer);
}

void WDT_init()
{
    //-----设定WDT为-----------
    WDTCTL = WDT_ADLY_16;
    //-----WDT中断使能----------------------
    IE1 |= WDTIE;
}

void LCD_Init()
{
    TCA6416A_Init();
    HT1621_init();
    //相关硬件的初始化，其中 I2C 模块的初始化由 TCA6416A 初始化函数在内部完成了， LCD_128 库函数由 HT1621 初始化函数在内部引用了
    //-----显示固定不变的LCD段-----
}

void LCD_Display()
{
    LCD_DisplaySeg(_LCD_DOT4);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 2);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 3);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 4);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 5);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 6);
    //-----根据zhenshu拆分并显示数字-----

    if (zhenshu > 999)
    {
        LCD_DisplayDigit(zhenshu / 1000, 2);
        LCD_DisplayDigit((zhenshu / 100) % 10, 3);
        LCD_DisplayDigit((zhenshu / 10) % 10, 4);
        LCD_DisplayDigit(zhenshu % 10, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else if (zhenshu > 99) //100~999（3位）
    {
        LCD_DisplayDigit(zhenshu / 100, 3);
        LCD_DisplayDigit((zhenshu / 10) % 10, 4);
        LCD_DisplayDigit(zhenshu % 10, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else if (zhenshu > 9) //2位
    {
        LCD_DisplayDigit(zhenshu / 10, 4);
        LCD_DisplayDigit(zhenshu % 10, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else if (zhenshu > 1) //1位
    {
        LCD_DisplayDigit(zhenshu, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else
    {
        LCD_DisplayDigit(0, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    HT1621_Reflash(LCD_Buffer); //-----更新缓存，真正显示-----
}

void ADC10_init(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = ADC10IE | ADC10ON | REFON | REF2_5V | ADC10SHT_0 | SREF_1;
    //启用内部2.5V参考电压，采样保持时间为64 x ADC10CLKs
    ADC10CTL1 = CONSEQ_0 | ADC10SSEL_0 | ADC10DIV_3 | SHS_0 | INCH_0;
    //ADC时钟分频为7分频
    ADC10AE0 = 0x1; //P1.0为ADC输入端
    __delay_cycles(20000);
    //ADC10CTL0 |= ENC;
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    LPM0_EXIT;
}
