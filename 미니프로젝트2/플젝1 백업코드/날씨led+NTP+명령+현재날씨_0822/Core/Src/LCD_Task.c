// =============================
// LCD_Task.c  (for lcd_i2c.c API)
// =============================
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "lcd_i2c.h"

extern I2C_HandleTypeDef hi2c1;

// NTP 공유 (이미 프로젝트에 존재)
extern osMutexId_t NTP_MutexHandle;
extern time_t ntp_time;

// Weather 공유 (프로젝트에 맞춰 이름 사용)
extern osMutexId_t Weather_MutexHandle;   // NOTE: 기존에 이렇게 선언되어 있던 이름을 그대로 사용
extern volatile float g_t1h;              // 현재기온(°C)
extern volatile int   g_reh;              // 습도(%)
extern volatile int   g_pty;              // PTY 코드
/*
// PTY 간단 표기
static const char* pty_short(int code) {
    switch (code) {
        case 0: return "No";     // 없음
        case 1: return "Rain";   // 비
        case 2: return "R/S";    // 비/눈
        case 3: return "Snow";   // 눈
        case 4: return "Shwr";   // 소나기
        case 5: return "Drop";   // 빗방울
        case 6: return "D/Sf";   // 빗방울눈날림
        case 7: return "SnowF";  // 눈날림
        default: return "?";
    }
}

// 16컬럼 라인에 맞춰 깔끔하게 출력(잘라내고 공백으로 채움)
static void LCD_WriteLine16(uint8_t row, const char *text) {
    char buf[17];
    size_t n = strlen(text);
    if (n > 16) n = 16;
    memcpy(buf, text, n);
    if (n < 16) memset(buf + n, ' ', 16 - n);
    buf[16] = '\0';
    LCD_SetCursor(row, 0);
    LCD_Print(buf);   // lcd_i2c.c: char* 인자
}

void LCD_Task(void *argument)
{
    // LCD 초기화 (lcd_i2c.c의 API 사용)
    LCD_Init(&hi2c1);
    LCD_Clear();
    LCD_WriteLine16(0, "LCD Ready");
    LCD_WriteLine16(1, "Waiting data...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    for (;;) {
        // ---------- 1행: NTP 시간 ----------
        char line1[32];
        time_t snap = 0;

        if (osMutexAcquire(NTP_MutexHandle, pdMS_TO_TICKS(20)) == osOK) {
            snap = ntp_time;
            osMutexRelease(NTP_MutexHandle);
        }

        if (snap > 0) {
            struct tm *k = localtime(&snap);  // KST로 로컬 설정되어 있음
            // "HH:MM:SS KST" (최대 12자)
            snprintf(line1, sizeof(line1), "%02d:%02d:%02d KST",
                     k->tm_hour, k->tm_min, k->tm_sec);
        } else {
            snprintf(line1, sizeof(line1), "NTP syncing...");
        }

        // ---------- 2행: 온/습/강수 ----------
        char line2[32];
        float t = 0.f; int h = -1, p = -1;

        if (osMutexAcquire(Weather_MutexHandle, pdMS_TO_TICKS(20)) == osOK) {
            t = g_t1h;
            h = g_reh;
            p = g_pty;
            osMutexRelease(Weather_MutexHandle);
        }

        if (h >= 0 && p >= 0) {
            // 예: "T27.3 H63 Rain" (16자 이내로 자동 패딩)
            snprintf(line2, sizeof(line2), "T%.1f H%d %s", t, h, pty_short(p));
        } else {
            snprintf(line2, sizeof(line2), "Weather sync...");
        }

        // ---------- LCD 업데이트 ----------
        LCD_WriteLine16(0, line1);
        LCD_WriteLine16(1, line2);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/
void LCD_Task(void *argument)
{

}

