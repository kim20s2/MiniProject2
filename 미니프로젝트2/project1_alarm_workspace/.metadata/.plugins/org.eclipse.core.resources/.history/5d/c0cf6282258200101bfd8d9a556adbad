#include "BT_Task.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "usart.h"

// ===== 외부 UART 핸들 (CubeMX 생성) =====
extern UART_HandleTypeDef huart6;  // BT 전용 (PC6/PC7)
extern UART_HandleTypeDef huart2;  // printf 출력(USART2)

// ===== 심플 전역 버퍼/플래그 =====
static volatile uint8_t  s_rx_byte;
static volatile uint8_t  s_line_ready = 0;
static char              s_line[64];
static size_t            s_idx = 0;

#define ARR_CNT 5
#define CMD_SIZE 50

// 1바이트 인터럽트 수신 시작
void BT_UART6_StartIT(void) {
  s_idx = 0;
  s_line_ready = 0;
  HAL_UART_Receive_IT(&huart6, (uint8_t*)&s_rx_byte, 1);
}

void MX_GPIO_LED_ON(int pin)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, pin, GPIO_PIN_SET);
}
void MX_GPIO_LED_OFF(int pin)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, pin, GPIO_PIN_RESET);
}

// IRQ 콜백(한 파일에서만 정의되도록 주의)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART6)
  {
  	static int i=0;
  	s_line[i] = s_rx_byte;
  	if((s_line[i] == '\n') || s_line[i] == '\r')
  	{
  		s_line[i] = '\0';
  		i = 0;
  		s_line_ready = 1;
  	}
  	else
  	{
  		i++;
  	}
  	HAL_UART_Receive_IT(&huart6, &s_rx_byte,1);
  }
}

// 간단 송신 헬퍼
static inline void bt_puts(const char *s) {
  HAL_UART_Transmit(&huart6, (uint8_t*)s, strlen(s), HAL_MAX_DELAY);
}

// 아주 단순한 커맨드 처리: LED, PING, LCD 알람 트리거 등

void bluetooth_Event()
{

  int i=0;
  char * pToken;
  char * pArray[ARR_CNT]={0};
  char recvBuf[CMD_SIZE]={0};
  char sendBuf[CMD_SIZE]={0};
  strcpy(recvBuf,s_line);

  printf("btData : %s\r\n",s_line);

  pToken = strtok(recvBuf,"[@]");
  while(pToken != NULL)
  {
    pArray[i] =  pToken;
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]");
  }

  if(!strcmp(pArray[1],"LED"))
  {
		if(!strcmp(pArray[2],"ON"))
		{
			MX_GPIO_LED_ON(LD2_Pin);
		}
		else if(!strcmp(pArray[2],"OFF"))
		{
			MX_GPIO_LED_OFF(LD2_Pin);
		}
  }
  else if(!strncmp(pArray[1]," New conn",sizeof(" New conn")))
  {
      return;
  }
  else if(!strncmp(pArray[1]," Already log",sizeof(" Already log")))
  {
      return;
  }
  else
      return;

  sprintf(sendBuf,"[%s]%s@%s\n",pArray[0],pArray[1],pArray[2]);
  HAL_UART_Transmit(&huart6, (uint8_t *)sendBuf, strlen(sendBuf), 0xFFFF);

}

void BT_Task(void *argument)
{
  (void)argument;
  bt_puts("BT Ready (USART6)\r\n");  // 시작 알림

  for (;;) {
    if (s_line_ready) {
      s_line_ready = 0;
      bluetooth_Event((const char*)s_line);
    }
    osDelay(10); // 가벼운 폴링
  }
}

