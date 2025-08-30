#include "cmsis_os.h"
#include "mfrc522.h"
#include <stdio.h>
#include <string.h>
#include "LED_FAN.h"
#include "FanHold.h"
#include "PIR_Task.h"

// 허용 UID (예: 5바이트, BCC 포함) — UART로 찍어본 뒤 그대로 넣으세요
static const uint8_t allowed[][5] = {
	{0x3D,0xD6,0xBA,0x01,0x50},
};

static int is_allowed(const uint8_t *uid5) {
  for (size_t i=0;i<sizeof(allowed)/sizeof(allowed[0]);++i)
    if (memcmp(uid5, allowed[i], 5)==0) return 1;
  return 0;
}

void RFID_Task(void *argument)
{
  MFRC522_Init();
  //int toggle = 0;
  for(;;){
    uint8_t atqa[2];
    if (MFRC522_Request(PICC_REQIDL, atqa) == MI_OK) {
      uint8_t uid[5];
      if (MFRC522_Anticoll(uid) == MI_OK) {
       // printf("[RFID] UID=%02X %02X %02X %02X %02X\r\n", uid[0],uid[1],uid[2],uid[3],uid[4]);

        if (is_allowed(uid)) {
          printf("RFID Sensored!\r\n");
          // 인증 성공 → PC4 LED, FAN ON (예: 5초)
          LED1_ON();
          LED2_ON();
          //fan_hold = 1;
          FAN_ON();
          PIR_ArmIdleTimer();
          //osDelay(5000);
          //LED_OFF();
          //FAN_OFF();
        }
      }
    }
    osDelay(100); // 폴링 주기
  }
}
