#include "arch/arch_utils.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_attr.h>
#include <esp_partition.h>

#include <sys/time.h>

namespace squads {
    namespace arch {

        typedef struct critical_lock {
            portMUX_TYPE handle;
            int created : 1;
        } critical_lock_t;

        int critical_start(critical_lock_t* lock) { 
            if(lock == NULL) return 1;

            lock->handle = portMUX_INITIALIZER_UNLOCKED; 
            lock->created = true;

            return lock->created == 1 ? 0 : 1;

        }
        int critical_lock(critical_lock_t* lock, unsigned int tout) {
            BaseType_t ret;
            if (xPortInIsrContext()) {
                ret = portTRY_ENTER_CRITICAL_ISR(&lock->handle, tout);
            } else {
                ret = portTRY_ENTER_CRITICAL(&lock->handle, tout);
            }
            return ret;
        }
        void critical_unlock(critical_lock_t* lock) {
            if (xPortInIsrContext()) {
                portEXIT_CRITICAL_ISR(&lock->handle);
            } else {
                portEXIT_CRITICAL(&lock->handle);
            }
        }
        typedef struct spin_lock {
            spinlock_t* handle;
            bool created ;
        } spin_lock_t;

        int spinlock_start(spin_lock_t* lock) {
            if(lock == 0) return 1;
            spinlock_initialize(lock->handle);
            lock->created = lock->handle != 0;

            return lock->created ? 0 : 1;
        }
        int spinlock_aacquire(spin_lock_t* lock, unsigned int timeout) {
            if(lock == 0) return 1;
            if(lock->created == false ) return 2;

            return spinlock_acquire(lock->handle, timeout) ? 0 : 1;
        }
        void spinlock_release(spin_lock_t* lock) {
            if(lock == 0) return ;
            if(lock->created == false ) return ;

            spinlock_release(lock->handle);
        }
        void yield() {
            taskYIELD();
        }
        void task_panic() {
            printf("libsquads panic :!! ");
            for(;;) { taskYIELD(); }
        }
        unsigned long micros() {
            static portMUX_TYPE microsMux = portMUX_INITIALIZER_UNLOCKED;
            static unsigned long lccount = 0;
            static unsigned long overflow = 0;

            unsigned long ccount;
            portENTER_CRITICAL_ISR(&microsMux);
            __asm__ __volatile__ ( "rsr %0, ccount" : "=a" (ccount) );
            if(ccount < lccount){
                overflow += UINT32_MAX / CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ;
            }
            lccount = ccount;
            portEXIT_CRITICAL_ISR(&microsMux);
            return overflow + (ccount / CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ);
        }

        unsigned long millis() {
            if (xPortInIsrContext()) {
                return xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
            } else {
                return xTaskGetTickCount() * portTICK_PERIOD_MS;
            }
        }

        unsigned int get_ticks() {
            if (xPortInIsrContext()) {
                return xTaskGetTickCountFromISR();
            } else {
                return xTaskGetTickCount();
            }
        }
        void delay(const unsigned long& ts) {
            vTaskDelay( ts );
        }
        void disable_interrupts() {
            taskDISABLE_INTERRUPTS();
        }
        void enable_interrupts() {
            taskENABLE_INTERRUPTS();
        }
        void schedular_suspend() {
            vTaskSuspendAll();
        }
        void schedular_resume() {
            xTaskResumeAll();
        }

    }
}