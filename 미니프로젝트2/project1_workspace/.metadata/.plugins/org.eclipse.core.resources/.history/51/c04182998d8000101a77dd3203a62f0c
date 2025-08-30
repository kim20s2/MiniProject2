#include "lcd.h"   // LCD 라이브러리 헤더 (I2C LCD1602용)
#include "FreeRTOS.h"
#include "task.h"


void LCD_Task(void *argument)
{
    LCD_Init();
    LCD_Clear();

    LCD_SetCursor(0,0);
    LCD_Print("Hello, LCD!");

    LCD_SetCursor(1,0);
    LCD_Print("STM32 + FreeRTOS");

    for(;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
