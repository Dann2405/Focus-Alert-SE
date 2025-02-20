#ifndef CONFIG_H
#define CONFIG_H

// Configurações e definições do projeto (pinos, tempos, limites, etc.)
#include "include/lib/bibliotecas.h"

// definição dos pinos do LED RGB
const uint LED_RED = 13;
const uint LED_GREEN = 11;
const uint LED_BLUE = 12;

// definição dos pinos dos botões
const uint BUTTON_A = 5;
const uint BUTTON_B = 6;

// definição dos pinos dos buzzer
const uint Buzzer_A = 21;
const uint Buzzer_B = 10;

// definição do pino da matrix 5x5
const uint MATRIX_LED = 7;

// definição dos pinos do joystick
const uint VRX_PIN = 27;
const uint VRY_PIN = 26;
const uint SW_PIN = 22;

// inicialização dos pinos leds
void leds_init()
{
    // inicializa os pinos dos LEDs
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);

    // define os pinos como saida
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
}

// inicialização dos pinos dos botões
void buttons_init()
{
    // inicializa os pinos dos botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);

    // define os pinos como entrada
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);

    // define os pinos como pull-up
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
}

// inicialização dos pinos do buzzer
void buzzer_init()
{
    // inicializa os pinos dos buzzers
    gpio_init(Buzzer_A);
    gpio_init(Buzzer_B);

    // define os pinos como saida
    gpio_set_dir(Buzzer_A, GPIO_OUT);
    gpio_set_dir(Buzzer_B, GPIO_OUT);
}

#endif // CONFIG_H