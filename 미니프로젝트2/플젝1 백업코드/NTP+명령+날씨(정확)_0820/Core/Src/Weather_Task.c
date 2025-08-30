// =============================
// weather_task.c
// =============================

#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp.h"

extern osMutexId_t ESP_MutexHandle;
extern UART_HandleTypeDef huart6;

// ★ esp.c 전역 버퍼 접근 (읽기 전용 용도)
extern cb_data_t cb_data;
extern char response[MAX_ESP_RX_BUFFER];


// ★ esp.c에 추가한 프로브 함수 원형
int esp_link1_tcp_probe(const char *ip_or_host, int port);

// 간단 JSON 파서: "category":"T1H" ... "obsrValue": 27  또는 "obsrValue":"27"
static int json_pick_value(const char *json, const char *cat, char *out, int out_sz)
{
    char key[64];
    snprintf(key, sizeof(key), "\"category\":\"%s\"", cat);
    const char *p = strstr(json, key);
    if (!p) return -1;

    const char *v = strstr(p, "\"obsrValue\"");
    if (!v) return -1;
    v = strchr(v, ':');
    if (!v) return -1;
    v++; // skip ':'

    // 공백/따옴표 스킵
    while (*v==' ' || *v=='\"') v++;

    // 숫자/텍스트 토큰 끝까지
    const char *e = v;
    while (*e && *e!='\"' && *e!='\r' && *e!='\n' && *e!=',' && *e!='}') e++;

    int len = (int)(e - v);
    if (len <= 0) return -1;
    if (len >= out_sz) len = out_sz - 1;
    memcpy(out, v, len);
    out[len] = 0;
    return 0;
}

static const char* pty_text(int code)
{
    switch (code) {
        case 0: return "강수없음";
        case 1: return "비";
        case 2: return "비/눈";
        case 3: return "눈";
        case 4: return "소나기";
        case 5: return "빗방울";
        case 6: return "빗방울눈날림";
        case 7: return "눈날림";
        default: return "?";
    }
}


void Weather_Task(void *argument)
{
    const char *KMA_HOST = "apihub.kma.go.kr";
    const char *KMA_IP   = "203.247.96.25";   // nslookup 결과
    uint16_t length;

    for (;;)
    {
        if (osMutexAcquire(ESP_MutexHandle, osWaitForever) == osOK) {
            // 1) 링크1 열기
            char cmd[128];
            sprintf(cmd, "AT+CIPSTART=1,\"TCP\",\"%s\",80\r\n", KMA_IP);
            if (esp_at_command((uint8_t*)cmd, (uint8_t*)response, &length, 5000) == 0) {
                printf("[WEATHER] Link1 TCP connect OK\r\n");

                // 2) HTTP GET 문자열 준비
                char http_req[512];
                sprintf(http_req,
                    "GET /api/typ02/openApi/VilageFcstInfoService_2.0/getUltraSrtNcst"
                    "?authKey=o6iIv1M5RHSoiL9TOaR0jw&pageNo=1&numOfRows=10"
                    "&dataType=JSON&base_date=20250822&base_time=0600&nx=55&ny=126 HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "Connection: close\r\n"
                    "\r\n", KMA_HOST);

                // 3) AT+CIPSEND=1,<len>
                sprintf(cmd, "AT+CIPSEND=1,%d\r\n", (int)strlen(http_req));
                if (esp_at_command((uint8_t*)cmd, (uint8_t*)response, &length, 2000) == 0) {
                    // (권장) 프롬프트 안정화
                    vTaskDelay(pdMS_TO_TICKS(50));

                    // 4) payload 송신 (단순 송신)
                    HAL_UART_Transmit(&huart6, (uint8_t*)http_req, strlen(http_req), 500);

                    // 5) 응답 수신: cb_data에서 rx로 누적 수집 (최대 8초, CLOSED 감지 시 조기 종료)
                    char rx[MAX_ESP_RX_BUFFER];
                    int  copied = 0;
                    int  elapsed = 0;
                    memset(rx, 0, sizeof(rx));

                    for (;;) {
                        // CLOSED 들어오면 서버가 응답 끝내고 소켓 닫았다는 의미
                        if (strstr((char*)cb_data.buf, "CLOSED")) break;

                        if (cb_data.length > 0) {
                            taskENTER_CRITICAL();
                            int n = cb_data.length;
                            if (n > (int)sizeof(rx) - 1 - copied) n = sizeof(rx) - 1 - copied;
                            memcpy(rx + copied, cb_data.buf, n);
                            cb_data.length = 0;   // 소비
                            taskEXIT_CRITICAL();
                            copied += n;
                        } else {
                            vTaskDelay(pdMS_TO_TICKS(20));
                            elapsed += 20;
                            if (elapsed >= 8000) break;  // 최대 8초 대기
                        }
                    }
                    /*
                    if (strstr(rx, "HTTP")) {
                        printf("[WEATHER] Got HTTP response (%dB):\r\n%.*s\r\n",
                               copied, (copied > 800) ? 800 : copied, rx);  // 로그 과다 방지
                    } else {
                        printf("[WEATHER] No HTTP response (copied=%d)\r\n", copied);
                    }
					*/
                    if (strstr(rx, "HTTP/1.1 200")) {
                        // JSON에서 값 뽑기
                        char t1h[16] = "?", reh[16] = "?", pty[16] = "?", rn1[16] = "?";
                        json_pick_value(rx, "T1H", t1h, sizeof(t1h));
                        json_pick_value(rx, "REH", reh, sizeof(reh));
                        json_pick_value(rx, "PTY", pty, sizeof(pty));
                        json_pick_value(rx, "RN1", rn1, sizeof(rn1));

                        int pty_code = atoi(pty);
                        printf("[WEATHER][Gangseo-gu] T=%s°C, RH=%s%%, RAIN=%s, RN1=%smm\r\n",
                               t1h, reh, pty_text(pty_code), rn1);
                    } else {
                        printf("[WEATHER] HTTP not 200 (len=%d)\r\n", copied);
                    }

                }

                // 6) 링크1 닫기
                esp_at_command((uint8_t*)"AT+CIPCLOSE=1\r\n", (uint8_t*)response, &length, 1000);
            } else {
                printf("[WEATHER] Link1 TCP connect FAIL\r\n");
            }

            osMutexRelease(ESP_MutexHandle);
        }

        vTaskDelay(pdMS_TO_TICKS(10000));  // 10초 주기
    }
}
