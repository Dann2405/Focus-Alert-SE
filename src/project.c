#include "include/lib/bibliotecas.h"
#include "include/ssd1306/interface.h"
#include "include/alarm.h"

// Função principal
int main() {
    // Inicializa o stdio
    stdio_init_all();

    // Inicializa o ADC
    adc_init();
    
    // Inicializa os periféricos
    leds_init();
    buttons_init();
    joystick_init();
    
    // Inicializa o display OLED
    displayssd1306_init();
    
    // Inicializa o sistema de alarme
    alarm_init();
    
    // Mensagem de depuração
    printf("Alarme de Medicamentos inicializado!\n");
    
    // Loop principal
    while (true) {
        // Atualiza o estado atual
        switch (current_state) {
            case STATE_IDLE:
                state_idle_update();
                break;
            case STATE_PROGRAM:
                state_program_update();
                break;
            case STATE_COUNTDOWN:
                state_countdown_update();
                break;
            case STATE_ALARM:
                state_alarm_update();
                break;
        }
        
        // Atualiza o alarme
        alarm_update();
        
        // Pequeno delay para evitar uso intensivo da CPU
        sleep_ms(10);
    }
    
    return 0;
}