/*
 * ssd1351.h
 *
 *  Created on: 2016-10-20
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef SSD1351_H_
#define SSD1351_H_

enum ssd1351_scroll_direction {
    Left    = 0x01,
    Right   = 0x81
};

enum ssd1351_scroll_interval {
    Fast    = 0x00,
    Normal  = 0x01,
    Slow    = 0x02,
    Slowest = 0x03
};

extern void ssd1351_draw_point(unsigned char chXpos, unsigned char chYpos, unsigned int hwColor);//画点函数，传入参数（X坐标，Y坐标，颜色）
extern void ssd1351_draw_line(unsigned char chXpos0, unsigned char chYpos0, unsigned char chXpos1, unsigned char chYpos1, unsigned int hwColor);//画线函数，传入参数（X起始坐标，Y起始坐标，X终止坐标，Y终止坐标，颜色）
extern void ssd1351_draw_h_line(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned int hwColor);//水平线函数，传入参数（X坐标，Y坐标，长度，颜色）
extern void ssd1351_draw_v_line(unsigned char chXpos, unsigned char chYpos, unsigned char chHeight, unsigned int hwColor);//垂直线绘制函数，传入参数（X坐标，Y坐标，高度，颜色）

extern void ssd1351_draw_column(unsigned char chXpos, unsigned char chYpos, unsigned char chHeight, unsigned int hwColor0, unsigned int hwColor1);//垂直线混合颜色绘制函数，传入参数（X坐标，Y坐标，高度，颜色1-实际绘制的颜色，颜色2-列中剩下的）

extern void ssd1351_draw_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight, unsigned int hwColor);//无填充矩形绘制函数，传入参数（X坐标，Y坐标，宽度，高度，颜色）
extern void ssd1351_draw_circle(unsigned char chXpos, unsigned char chYpos, unsigned char chRadius, unsigned int hwColor);//无填充圆形绘制函数，传入参数（X坐标，Y坐标，半径，颜色）

extern void ssd1351_draw_mono_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight, unsigned int hwForeColor, unsigned int hwBackColor);
extern void ssd1351_draw_64k_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight);

extern void ssd1351_fill_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight, unsigned int hwColor);//填充矩形绘制函数，传入参数（X坐标，Y坐标，宽度，高度，颜色）
extern void ssd1351_fill_gram(unsigned int hwColor);//填充绘制函数，传入参数（颜色）

extern void ssd1351_clear_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight);//清楚指定大小的填充矩形，传入参数（X坐标，Y坐标，宽度，高度）
extern void ssd1351_clear_gram(void);//清屏

extern void ssd1351_display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor);
//显示字符函数，传入参数（X坐标，Y坐标，字符，字号（12*6填0，16*8填1，16*16填2，32*16填3），前景色，背景色）
extern void ssd1351_display_num(unsigned char chXpos, unsigned char chYpos, unsigned long chNum, unsigned char chLen, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor);
//显示数字函数，传入参数（X坐标，Y坐标，数字，数字长度，数字字号（12*6填0，16*8填1，16*16填2，32*16填3），前景色，背景色）
extern void ssd1351_display_string(unsigned char chXpos, unsigned char chYpos, const char *pchString, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor);
//显示数字函数，传入参数（X坐标，Y坐标，数字，数字长度，数组名，字号（12*6填0，16*8填1，16*16填2，32*16填3），前景色，背景色）
extern void ssd1351_horizontal_scrolling(unsigned char chYpos, unsigned char chHeight, unsigned char chDirection, unsigned char chInterval);
extern void ssd1351_deactivate_scrolling(void);

extern void ssd1351_show_checkerboard(void);
extern void ssd1351_show_rainbow(void);

extern void ssd1351_init(void);


#endif
