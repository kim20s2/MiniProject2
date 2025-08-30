// =============================
// weather_task.c
// =============================
#include "cmsis_os.h"
#include <stdio.h>
#include "esp.h"

#define WEATHER_TASK_PERIOD_MS 10000
extern osMutexId_t ESP_MutexHandle;
// TODO: 사용중인 HTTP GET 구현으로 교체 (현재 프로젝트에 맞춰 작성)

void Weather_Task(void *argument)
{
    for (;;)
    {
        // osMutexAcquire(ESP_MutexHandle, osWaitForever);
        // ESP_HTTP_GET(...);  // 구현되어 있다면 사용
        // osMutexRelease(ESP_MutexHandle);

        vTaskDelay(pdMS_TO_TICKS(WEATHER_TASK_PERIOD_MS));
    }
}
