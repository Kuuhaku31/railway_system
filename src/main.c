
// main.c

#include "system_controller.h"
#include "system_view.h"
#include "utils.h"

extern bool system_is_running;

int
main()
{
    initConfig();
    initDb();
    SystemControlerInit();
    SystemViewInit();
    while(system_is_running)
    {
        SystemViewOnFrameBegin();
        SystemProcessEvent();
        SystemControllerUpdate();
        SystemViewShowWindows();
        SystemViewOnFrameEnd();
    }
    SystemQuitView();
    finalizeDb();
    return 0;
}
