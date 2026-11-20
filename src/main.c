#include "APP/clock_app.h"

int main(void) {
    ClockApp_Init();
    
    while (1) {
        ClockApp_Update();
    }
    
    return 0;
}