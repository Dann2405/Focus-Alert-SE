#include "interface.c"

int main()
{
    stdio_init_all();

    leds_init();
    buttons_init();
    buzzer_init();




    displayssd1306_init();
    
    
    while (true) {
        printf("Hello, world!\n");
        update_display();
        sleep_ms(1000);
    }
}
