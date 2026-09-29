#include "app_ifa.h"

#if defined(SIM_ENABLE)
#include "sim_ifa.h"
#define MAIN_SIM_START()  ((void)sim_start(app_get_counts_per_deg()))
#define MAIN_LOOP_RUNNING (sim_is_running() != 0u)
#else
#define MAIN_SIM_START()  ((void)0)
#define MAIN_LOOP_RUNNING 1
#endif

int main(void)
{
    app_init();

    MAIN_SIM_START();

    while (MAIN_LOOP_RUNNING)
    {
        if (app_get_1ms_flag() != 0u)
        {
            app_clear_1ms_flag();
            app_handle_1ms_event();
        }
    }

    return 0;
}
