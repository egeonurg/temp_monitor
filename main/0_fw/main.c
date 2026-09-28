#include "app_ifa.h"

/* On a host build the interrupts are raised by the simulator; on target they
   come from the hardware and the superloop never exits. Either way they are
   handled inside the drivers, and only the 1 ms event flag reaches here. */
#if defined(SIM_ENABLE)
#include "sim_ifa.h"
#define MAIN_SIM_START()  ((void)sim_start())
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
