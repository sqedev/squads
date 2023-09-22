#include "arch/arch_utils.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace squads {
    namespace arch {
        void yield() {
            taskYIELD();
        }
    }
}