#include "PIR_Task.h"
#include "cmsis_os2.h"
#include "gpio.h"

volatile uint8_t g_pir_detected = 0;  // ★ 전역 정의(여기 1곳)

static inline uint32_t MS2T(uint32_t ms){
    return (uint32_t)((((uint64_t)ms) * osKernelGetTickFreq()) / 1000U);
}

void PIR_Task(void *argument)
{
    (void)argument;

    // PIR 모듈 워밍업이 필요한 경우 사용 (필요없으면 주석)
    // osDelay(30000);

    const uint32_t period = MS2T(50);  // 50ms 주기
    const uint8_t  HI_THR = 4;         // 200ms 이상 HIGH면 감지로 인정
    const uint8_t  LO_THR = 10;        // 500ms 이상 LOW면 미감지로 인정

    uint8_t hi_cnt = 0, lo_cnt = 0;

    for(;;){
        GPIO_PinState s = HAL_GPIO_ReadPin(PIR_GPIO_Port, PIR_Pin);

        if (s == GPIO_PIN_SET) {     // 보통 HIGH = 감지
            if (hi_cnt < 255) hi_cnt++;
            lo_cnt = 0;
            if (hi_cnt >= HI_THR) g_pir_detected = 1;
        } else {
            if (lo_cnt < 255) lo_cnt++;
            hi_cnt = 0;
            if (lo_cnt >= LO_THR) g_pir_detected = 0;
        }

        osDelay(period);
    }
}
