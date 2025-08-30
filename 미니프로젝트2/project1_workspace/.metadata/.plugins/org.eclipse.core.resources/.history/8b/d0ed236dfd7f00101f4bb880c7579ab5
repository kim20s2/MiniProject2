// =============================
// aiot_task.c
// =============================
#include "cmsis_os.h"
#include <stdio.h>
#include "esp.h"

extern osMutexId_t ESP_MutexHandle;

int esp_is_wifi_ready(void);

void Aiot_Task(void *argument)
{
    AiotClient_Init();
    for(;;) {
        if (!esp_is_wifi_ready()) { vTaskDelay(pdMS_TO_TICKS(2000)); continue; }
        if (esp_get_status()!=0) {
            printf("server connecting ...\r\n");
            if (osMutexAcquire(ESP_MutexHandle, osWaitForever)==osOK) {
                esp_client_conn();
                osMutexRelease(ESP_MutexHandle);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

