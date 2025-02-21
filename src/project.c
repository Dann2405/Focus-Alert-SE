#include "interface.c"

int main()
{
    stdio_init_all();

    // inicializa os componentes de hardware
    leds_init();
    buttons_init();
    buzzer_init();
    joystick_init();

    // inicializa o display
    displayssd1306_init();
    
    
    while (true) {
        printf("Hello, world!\n");
        update_display();
        sleep_ms(1000);
    }
}
