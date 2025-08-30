#include "lcd.h"
#include <stdarg.h>

// 내부 함수 원형
static void LCD_SendCmd(uint8_t cmd);
static void LCD_SendData(uint8_t data);
static void LCD_Send(uint8_t data, uint8_t mode);
static void LCD_Write4Bits(uint8_t data);
static void LCD_ExpanderWrite(uint8_t data);
static void LCD_PulseEnable(uint8_t data);

// 전역 상태
static uint8_t backlight = 0x08; // 기본 백라이트 ON

// 초기화
void LCD_Init(void)
{
    HAL_Delay(50);  // 전원 안정화 대기

    // 4비트 모드 진입 시퀀스
    LCD_Write4Bits(0x30);
    HAL_Delay(5);
    LCD_Write4Bits(0x30);
    HAL_Delay(5);
    LCD_Write4Bits(0x30);
    HAL_Delay(5);
    LCD_Write4Bits(0x20);  // 4bit 모드

    // 기능 설정
    LCD_SendCmd(LCD_CMD_FUNCTION_SET | LCD_FUNCTION_2LINE | LCD_FUNCTION_5x8);
    // 표시 켜기
    LCD_SendCmd(LCD_CMD_DISPLAY_CTRL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF);
    // 화면 지우기
    LCD_Clear();
    // 엔트리 모드 설정
    LCD_SendCmd(LCD_CMD_ENTRY_MODE | LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DEC);

    HAL_Delay(100);
}

void LCD_Clear(void)
{
    LCD_SendCmd(LCD_CMD_CLEAR);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    static uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 1) row = 1; // 1602는 2라인
    LCD_SendCmd(LCD_CMD_SET_DDRAM | (col + row_offsets[row]));
}

void LCD_Print(const char *str)
{
    while (*str) {
        LCD_SendData((uint8_t)(*str));
        str++;
    }
}

void LCD_Printf(const char *fmt, ...)
{
    char buf[32];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    LCD_Print(buf);
}

// 내부 함수 ===========================
static void LCD_SendCmd(uint8_t cmd)
{
    LCD_Send(cmd, 0);
}

static void LCD_SendData(uint8_t data)
{
    LCD_Send(data, 1);
}

static void LCD_Send(uint8_t data, uint8_t mode)
{
    uint8_t highNib = data & 0xF0;
    uint8_t lowNib  = (data << 4) & 0xF0;

    LCD_Write4Bits(highNib | (mode ? 0x01 : 0x00));
    LCD_Write4Bits(lowNib  | (mode ? 0x01 : 0x00));
}

static void LCD_Write4Bits(uint8_t data)
{
    LCD_ExpanderWrite(data);
    LCD_PulseEnable(data);
}

static void LCD_ExpanderWrite(uint8_t data)
{
    uint8_t val = data | backlight;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &val, 1, 10);
}

static void LCD_PulseEnable(uint8_t data)
{
    LCD_ExpanderWrite(data | 0x04); // EN=1
    HAL_Delay(1);
    LCD_ExpanderWrite(data & ~0x04); // EN=0
    HAL_Delay(1);
}
