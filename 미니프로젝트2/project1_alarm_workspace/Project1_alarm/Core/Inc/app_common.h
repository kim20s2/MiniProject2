#pragma once
#include "cmsis_os.h"
#include <stdint.h>

// app_common.h

typedef enum {
    EVT_NONE = 0,
    EVT_GAS_ALARM,
    EVT_VIB_ALARM,
    EVT_FLM_ALARM,
    EVT_PIR_ALARM
} app_event_t;

typedef struct {
    app_event_t type;   // (이전 호환용)
    uint16_t    gas;    // ADC
    uint16_t    vib;    // 0/1 (det)
    uint16_t    flm;    // 0/1 (det)
    uint8_t     pir;    // 0/1 (PIR 30초 래치 결과)
    uint8_t     line1_mask; // ★ 1행 표시용 래치 마스크 (bit0=GAS, bit1=VIB, bit2=FLM)
} app_msg_t;


extern osMessageQueueId_t g_lcdQueue;
extern volatile uint8_t g_pir_detected;   // PIR_Task가 갱신

