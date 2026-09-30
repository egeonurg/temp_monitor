#include "app_dat.hpp"
#include "AppOrchestrator.hpp"

#if defined(SIM_ENABLE)
extern "C"
{
#include "sim_ifa.h"
}
#define MAIN_SIM_START(counts_per_deg) ((void)sim_start(counts_per_deg))
#define MAIN_LOOP_RUNNING              (sim_is_running() != 0u)
#else
#define MAIN_SIM_START(counts_per_deg) ((void)(counts_per_deg))
#define MAIN_LOOP_RUNNING              true
#endif

int main()
{
    AppOrchestrator &app = getApp();

    app.init();

    MAIN_SIM_START(app.getCountsPerDeg());

    while (MAIN_LOOP_RUNNING)
    {
        app.performServices();
    }

    return 0;
}
