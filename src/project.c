#include "include/lib/bibliotecas.h"
#include "interface.c"

int main()
{
    stdio_init_all();

    displayssd1306_init();
    
    
    while (true) {
        printf("Hello, world!\n");
        update_display();
        sleep_ms(1000);
    }
}
