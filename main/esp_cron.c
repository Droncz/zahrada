


#include "config.h"


void blink()
{
    int cnt = 0;

    while(1) {
        vTaskDelay(SECOND);
        cnt++;
        // printf("Blink count: %d\n", cnt);
        if (relays[0].enabled) {
            gpio_set_level(relays[0].gpio_num, cnt % 2);
            relays[0].state = cnt % 2;
        }
        if (relays[1].enabled) {
            gpio_set_level(relays[1].gpio_num, cnt % 2);
            relays[1].state = cnt % 2;
        }

        gpio_set_level(relays[6].gpio_num, cnt % 2); // onboard LED blink
        relays[6].state = cnt % 2;
    }
    
}
